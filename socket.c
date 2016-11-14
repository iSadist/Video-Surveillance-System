#include "socket.h"

extern int jpeg_socket = SOCKET_INACTIVE;
extern int web_socket = SOCKET_INACTIVE;

pthread_mutex_t mutex;

void open_socket(int * socketfd)
{
	int stream_sockfd, stream_portno;
	socklen_t clilen; 
	time_t raw_time;
	struct tm * time_info;
	char client_IP[INET_ADDRSTRLEN];
	pthread_t input_reader_thread;
	struct sockaddr_in serv_addr_web, serv_addr_stream, cli_addr;

	pthread_mutex_lock(&mutex);

	*socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*socketfd < 0){ 
		log_error("ERROR opening streaming socket");
		return EXIT_FAILURE;
	}

	bzero((char *) &serv_addr_stream, sizeof(serv_addr));
	web_portno = PORT_WEB;
	serv_addr_stream.sin_family = AF_INET;
	serv_addr_stream.sin_addr.s_addr = INADDR_ANY;
	serv_addr_stream.sin_port = htons(portno);

	if (bind(*socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		log_error("ERROR on binding");
		return EXIT_FAILURE;
	}

	pthread_mutex_unlock(&mutex);
}

void open_jpeg_socket()
{
	open_socket(&jpeg_socket);
} 

void open_web_socket()
{
	open_socket(&web_socket);
} 

void close_jpeg_socket()
{
	pthread_mutex_lock(&mutex);
	if ( jpeg_socket == SOCKET_INACTIVE )
		return;
	close(jpeg_socket);
	jpeg_socket = SOCKET_INACTIVE;
}

void close_web_socket()
{
	pthread_mutex_lock(&mutex);
	if ( web_socket == SOCKET_INACTIVE )
		return;
	close(web_socket);
	web_socket = SOCKET_INACTIVE;
	pthread_mutex_unlock(&mutex);
}

int main()
{
	// to check if compiles
}