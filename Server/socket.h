#ifdef __cplusplus
extern "C" {
#endif 

#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <stddef.h>
#include <errno.h>
#include <pthread.h>

#define SOCKET_INACTIVE			-1
#define PORT_WEB				9091
#define PORT_STREAM				9090

#define DEBUG 

extern int jpeg_socket;
extern int web_socket;
extern bool web_running;

void open_jpeg_socket(void);
void open_web_socket(void);
void close_jpeg_socket(void);
void close_web_socket(void);

#endif

#ifdef __cplusplus
}
#endif
