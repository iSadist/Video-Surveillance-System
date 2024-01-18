#ifndef SOCKET_WRITER_H
#define SOCKET_WRITER_H
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

#include "socket.h"
#include "socket_listener.h"
#include "mode_handler.h"

#define SLEEPING_TIME 	5

// =============================================================================
// Public functions
// =============================================================================

/**
 * @brief Runs the socket_listener.
 * @details Run this function as a new thread.
 * @param arg - not used.
 */
void * stream_callback(void *);

/**
 * @brief Captures jpeg and outputs it on socket 
 * @details Run this function as a new thread.
 */
void capture_and_output_socket(int);

/**
 * @brief Outputs the message header to the socket.
 */
void output_header(int, size_t);

/**
 * @brief Sends a JPEG image from the camera over the socket.
 */
void output_jpeg(int, size_t, void *);

/**
 * @brief Sets the mode of operation.
 * @param new_mode - the new mode of operation.
 */
void socket_set_mode(mode_handler_mode_t, int);

void socket_writer_init(void);
void socket_writer_close(void);

#endif // SOCKET_WRITER_H
