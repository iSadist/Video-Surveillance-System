#ifdef __cplusplus
extern "C" {
#endif 

#ifndef WEBB_LISTENER_H
#define WEBB_LISTENER_H

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
#include "socket.h"
#include "logger.h"

extern int jpeg_socket;
extern int web_socket;

void read_socket(void);
void * web_callback(void * );
void output_web_jpeg();
void output_web_error();

#endif

#ifdef __cplusplus
}
#endif
