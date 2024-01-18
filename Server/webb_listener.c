#include "socket.h"
#include <capture.h>
#include "webb_listener.h"
#include <string.h>

#ifndef WEBB_LISTENER_C
#define WEBB_LISTENER_C

// Synchonization lock
pthread_mutex_t lock;



static int newsockfd = SOCKET_INACTIVE;

void * web_callback( void * data )
{

	printf("%s %s\n", __DATE__, __TIME__);

	while ( web_socket == SOCKET_INACTIVE && web_running )
		;

	// register for listeneing

	while ( web_running ){

		read_socket();

	}

	return NULL;
}

void output_web_jpeg()
{
	char * msg;

	msg = "HTTP/1.1 200 OK\r\n";
	write(newsockfd, msg, strlen(msg));
	msg = "Content-Description: File Transfer\r\n";
	write(newsockfd, msg, strlen(msg));
	msg = "Content-Transfer-Encoding: binary\r\n";
	write(newsockfd, msg, strlen(msg));
	msg = "Content-Disposition: attachment; filename=\"image.jpg\"\r\n";
	write(newsockfd, msg, strlen(msg));
	msg = "Content-Type: image/jpeg\r\n\r\n";
	write(newsockfd, msg, strlen(msg));

	media_stream * stream; 
	media_frame * frame;
	size_t size;

	stream = capture_open_stream(IMAGE_JPEG, "fps=25&resolution=640x480");
	frame = capture_get_frame(stream);
	size = capture_frame_size(frame);

	printf("size of jpg: %zu\n", size);
	
	write(newsockfd, (char*)capture_frame_data(frame), size);

	capture_frame_free(frame);
	capture_close_stream(stream);

}

void output_web_page()
{
	char * msg;

	printf("output error... socket: %d\n", newsockfd);
	msg = "HTTP/1.1 200 OK\r\n";
	write(newsockfd, msg, strlen(msg));
	msg = "Content-Type: text/html; chartset =\r\n\r\n";
	write(newsockfd, msg, strlen(msg));

	msg = "<!doctype html><html><body style='background-color:powderblue'><center><img src='image.jpg'><p id='date'></p></img><a href='image.jpg'>Download image</a></center><script>document.getElementById('date').innerHTML = 'Snapshot taken at: ' + Date();</script></body></html>";
	write(newsockfd, msg, strlen(msg));

}


void read_socket()
{
	extern int web_socket;
	extern bool web_running;

	socklen_t clilen;
	struct sockaddr_in cli_addr;

	int n;

	char quick_msg[1024], * req_type, * req_path; // Only read max 30 chars (for a simple GET req with arg /img.jpg)

#ifdef DEBUG
	char * req_stop;
#endif	

	memset(quick_msg, '\0', sizeof(quick_msg));

	listen(web_socket, 5);

	newsockfd = accept(web_socket, (struct sockaddr *) &cli_addr, &clilen); // block until recieved request
	
	n = read(newsockfd, quick_msg, sizeof(quick_msg) - 1);
	
#ifdef DEBUG
	printf("read: %s from socket %d\n", quick_msg, newsockfd);
#endif

#ifdef DEBUG	
	req_stop = strstr(quick_msg, "STOP");

	if ( req_stop != NULL ){
		web_running = false;
	}

#endif	

	req_type = strstr(quick_msg, "GET");
	req_path = strstr(quick_msg, "image.jpg");

	if ( req_path != NULL && req_type != NULL ){

		output_web_jpeg();

	} else {

		output_web_page();

	}

	printf("closing socket: %d\n", newsockfd);
	if ( close(newsockfd) ) {
		printf("close failed!\n");
	}

}

#endif
