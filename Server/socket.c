#include "socket.h"
#include "logger.h"

#ifndef SOCKET_C
#define SOCKET_C

int jpeg_socket = SOCKET_INACTIVE;
int web_socket = SOCKET_INACTIVE;
bool web_running = true;

static pthread_mutex_t mutex;

static bool is_initialized = false;

void open_socket(int * socketfd, int portno)
{
	printf("%s.%s port: %d\n", __FILE__, __func__, portno);
	struct sockaddr_in serv_addr;

	pthread_mutex_lock(&mutex);

	*socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*socketfd < 0){ 
		log_error("ERROR: opening socket on port %d\nProgram aborted.\n", portno);
		exit(EXIT_FAILURE);
		return;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(*socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("ERROR on binding socket to port %d\nProgram aborted.\n", portno);
		exit(EXIT_FAILURE);
		return;
	}

	pthread_mutex_unlock(&mutex);
}

void open_jpeg_socket()
{
	extern int jpeg_socket;
	open_socket(&jpeg_socket, PORT_STREAM);
} 

void open_web_socket()
{
	extern int web_socket;
	open_socket(&web_socket, PORT_WEB);
} 

void close_jpeg_socket()
{
	extern int jpeg_socket;
	pthread_mutex_lock(&mutex);
	if ( jpeg_socket == SOCKET_INACTIVE )
		return;
	close(jpeg_socket);
	jpeg_socket = SOCKET_INACTIVE;
	pthread_mutex_unlock(&mutex);
}

void close_web_socket()
{
	extern int web_socket;
	pthread_mutex_lock(&mutex);
	if ( web_socket == SOCKET_INACTIVE )
		return;
	close(web_socket);
	web_socket = SOCKET_INACTIVE;
	pthread_mutex_unlock(&mutex);
}

void sockets_init()
{
	if ( !is_initialized ){
		pthread_mutex_init(&mutex, NULL);
		is_initialized = true;
	}
}

void sockets_close()
{
	if ( is_initialized ){
		pthread_mutex_destroy(&mutex);
		is_initialized = false;
	}
}	

#endif

