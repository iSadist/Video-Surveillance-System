// =============================================================================
// Includes
// =============================================================================
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
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

#include "socket_listener.h"

// =============================================================================
// Private variables
// =============================================================================
static mode_handler_mode_t next_mode = MODE_IDLE;
static bool new_read_mode = false;

// =============================================================================
// Public functions
// =============================================================================

void * stream_listener_callback(void * socket_ptr)
{
	extern int jpeg_socket;
	int stream_socket;

	int n, nothing_count = 0;
	char read_buffer[10];
	
	stream_socket = *((int*) socket_ptr);


	while ( jpeg_socket != SOCKET_INACTIVE && stream_socket != SOCKET_INACTIVE ) {
		memset(read_buffer, '\0', sizeof(read_buffer));
		errno = 0;		
		n = read(stream_socket, read_buffer, 1);
		
		if ( read_buffer[0] != '0' && read_buffer[0] != '1' && read_buffer[0] != '2'){
			nothing_count++;
		}

		if ( nothing_count > JIBBERISH_TOLERANCE ) {
			// The socket is non-blocking and the connection is lost..
			close(stream_socket);
			stream_socket = SOCKET_INACTIVE;
			*((int*)socket_ptr) = SOCKET_INACTIVE;
		}

		printf("%s %s.%s read: %s\n", __TIME__, __FILE__, __func__, read_buffer);
		
		switch(read_buffer[0]){
		case 0x30:
			set_mode( MODE_IDLE, 0 );
			break;
		case 0x31:
			set_mode( MODE_MOVIE, 0 );
			break;
		default:
			break;
		}

	}

    return NULL;
}

// =============================================================================
// Private functions definitions
// =============================================================================


 
