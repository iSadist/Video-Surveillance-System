// =============================================================================
// Includes
// =============================================================================

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "socket_writer.h"

// =============================================================================
// Private variables
// =============================================================================

static volatile mode_handler_mode_t mode = MODE_IDLE;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static bool is_initialized = false;
static char motion_bit = 0;

// =============================================================================
// Private functions declarations
// =============================================================================


// =============================================================================
// Public functions
// =============================================================================

void * stream_callback(void * arg)
{
    extern int jpeg_socket;
	int stream_socket = SOCKET_INACTIVE;
    socklen_t clilen;
    struct sockaddr_in cli_addr;
	pthread_t listener_thread;

    printf("entering stream callback\n");

	while ( 1  ){

		if ( jpeg_socket == SOCKET_INACTIVE ){
						
			// waiting time granuality for socket initialization is 5 seconds
			sleep(SLEEPING_TIME);
			open_jpeg_socket();

		}

		listen(jpeg_socket, 5);

		stream_socket = accept(jpeg_socket, (struct sockaddr *) &cli_addr, &clilen); // block until recieved request
		printf("accepting on socket: %d\n", stream_socket);

		pthread_create(&listener_thread, NULL, stream_listener_callback, &stream_socket);

		while ( stream_socket != SOCKET_INACTIVE ){

			capture_and_output_socket(stream_socket);
			if ( mode == MODE_IDLE ){
				sleep(SLEEPING_TIME);
			}

		}

		close_jpeg_socket();
		
	}

    return NULL;
}

void output_start_flag(int stream_socket)
{
	char * msg = "STARTFRAME";
	write(stream_socket,  msg, strlen(msg));	
}

void capture_and_output_socket(int stream_socket)
{
	media_stream * stream; 
	media_frame * frame;
	size_t size;
	void * data;
	extern int jpeg_socket;

	
//printf("capture and output on socket %d\n", stream_socket);

	stream = capture_open_stream(IMAGE_JPEG, "fps=25&resolution=640x480");
	frame = capture_get_frame(stream);

	size = capture_frame_size(frame);
 	// timestamp = capture_frame_timestamp(frame); -> This timestamp was jibberish!!
	data = capture_frame_data(frame);

	// outputting 'startframe'
	output_start_flag(stream_socket);
	output_header(stream_socket, size);

	//usleep(50000);

	output_jpeg(stream_socket, size, data);

//	printf("headers and jpg outputed on socket %d\n", stream_socket);
	
	capture_frame_free(frame);

	capture_close_stream(stream);
}

void output_header(int stream_socket, size_t size)
{
	extern int jpeg_socket;
	unsigned long long time_unix;

	struct timeval time_micro;


	// mode of operation
	int md = 0;
	int i;
	uint8_t data;

	if (mode == MODE_MOVIE){
		md = 1;
	}

	gettimeofday(&time_micro, NULL);

	time_unix = time_micro.tv_sec*1000LLU + time_micro.tv_usec / 1000LLU;

	for (i = 7; i != -1; --i) {
		data = (uint8_t)(time_unix >> (i * 8)) & 0x00000000000000FF;
		//printf("%02X", data);
		write(stream_socket, &data, 1);
	}

	for (i = 3; i != -1; --i) {
		data = (uint8_t)(size >> (i * 8)) & 0x000000FF;
		//printf("%02X", data);
		write(stream_socket, &data, 1);
	}
	
	md = md | motion_bit;

    write(stream_socket, (char*)&md, 1);
	//printf("%02X\n", md);
}

void output_jpeg(int stream_socket, size_t size, void * data)
{
	long long checksum = 0;

	size_t i = 0;
	while(i < size){
		checksum += (signed char) ((char*) data)[i];
		i++;
	}

	//printf("JPEG CHECKSUM %lld\n", checksum); 

	write(stream_socket, data, size);
} 


void socket_set_mode(mode_handler_mode_t new_mode, int motion_detected)
{
	if ( mode == new_mode )
		return;

	pthread_mutex_lock(&lock);

	if ( motion_detected )
		motion_bit = 2;	

	if ( new_mode == MODE_IDLE ) {
		motion_bit = 0;	
	}

	mode = new_mode;
	
   	pthread_mutex_unlock(&lock);
}

void socket_writer_init()
{
	if ( !is_initialized ) {
		is_initialized = true;
	}
}

void socket_writer_close()
{
	if ( is_initialized ){
		is_initialized = false;
	}
}

// =============================================================================
// Private functions definitions
// =============================================================================

