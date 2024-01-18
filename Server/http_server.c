#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>  // for memcpy, memset, strlen
#include <poll.h>
#include <pthread.h>
#include <signal.h>

#include "webb_listener.h"
#include "socket.h"
#include "socket_writer.h"
#include "mode_handler.h"
#include "motion_listener.h"

#define USE_CAMERA
#define INFO
#undef DEBUG

// the strncasecmp function is not in the ISO C standard
#define USE_POSIX_FUNCTION

#ifdef USE_CAMERA
#include "camera.h"
#endif

static volatile int running = 1;

void sigint_handl(int sig)
{ 
	if ( sig == SIGINT ){
		printf("\nStopping program\n");
		running = 0;
	}
}

int main(int argc, char *argv[])
{
	pthread_t web_loop, stream_loop, motion_detection_loop;
	printf("%s v.1 compiled %s %s\n", __FILE__, __DATE__, __TIME__);
	printf("size of size_t: %d, long long: %d\n", sizeof(size_t), sizeof(long long));

	init_mode_handler();
	register_on_change_callback(socket_set_mode);

	open_web_socket();
	open_jpeg_socket();

	pthread_create(&web_loop, NULL, web_callback, NULL);
	pthread_create(&stream_loop, NULL, stream_callback, NULL);
	pthread_create(&motion_detection_loop, NULL, motion_callback, NULL);

	running = 1;
	signal(SIGINT, sigint_handl);

	while ( running )
		;

	close_web_socket();
	close_jpeg_socket();

	close_mode_handler();

	return 0;
}
