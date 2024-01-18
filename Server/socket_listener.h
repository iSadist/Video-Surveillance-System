#ifndef SOCKET_LISTENER_H
#define SOCKET_LISTENER_H

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
#include <capture.h>

#include "mode_handler.h"
#include "socket.h"

/**
 * @brief Runs the socket_listener.
 * @details Run this function as a new thread.
 * @param arg - not used.
 */

#define JIBBERISH_TOLERANCE		10

void * stream_listener_callback(void * arg); 

#endif
