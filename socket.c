#include "socket.h"

pthread_mutex_t mutex;

void open_socket(int * socketfd, int portno)
{
	int stream_sockfd;
	socklen_t clilen; 
	time_t raw_time;
	struct tm * time_info;
	char client_IP[INET_ADDRSTRLEN];
	pthread_t input_reader_thread;
	struct sockaddr_in serv_addr, cli_addr;

	pthread_mutex_lock(&mutex);

	*socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*socketfd < 0){ 
		log_error("ERROR opening streaming socket");
		return;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(*socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		log_error("ERROR on binding");
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

int main()
{
	// to check if compiles
}