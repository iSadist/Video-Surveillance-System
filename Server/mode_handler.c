// =============================================================================
// Includes
// =============================================================================
#include "mode_handler.h"

#include <pthread.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

// =============================================================================
// Private variables
// =============================================================================

// current mode of operation
static mode_handler_mode_t mode = MODE_IDLE;
pthread_mutex_t mode_mutex = PTHREAD_MUTEX_INITIALIZER;

// Callback function list (null terminated)
typedef struct callback_list_node_t
{
    struct callback_list_node_t *  next_node;
    mode_handler_callback_t callback;    
} callback_list_node_t;

static callback_list_node_t * callback_list_first;

// =============================================================================
// Private functions
// =============================================================================

#define MODE_LOCK       do {pthread_mutex_lock(&mode_mutex);} while (0)
#define MODE_UNLOCK     do {pthread_mutex_unlock(&mode_mutex);} while (0)

// =============================================================================
// Public functions
// =============================================================================
void init_mode_handler(void)
{
	printf("%s.%s\n", __FILE__, __func__);    
    
    mode = MODE_IDLE;
    
    callback_list_first = NULL;
}

void close_mode_handler(void)
{
    // Make sure that the mutex is not locked
   // MODE_LOCK;
   // MODE_UNLOCK;
    
    // Free callback list
    callback_list_node_t * current_node = callback_list_first;
    
    while (current_node != NULL) {
        callback_list_node_t * node_to_free = current_node;
        current_node = current_node->next_node;
        free(node_to_free);
    }
    
    // Destroy mutex
    pthread_mutex_destroy(&mode_mutex);
}

void set_mode(mode_handler_mode_t new_mode, int motion_detected)
{   
	//printf("Entered %s.%s\n", __FILE__, __func__);    
	
	MODE_LOCK;

    if (mode != new_mode) {
        callback_list_node_t * current_node;
        
        mode = new_mode;
        current_node = callback_list_first;
        
        while (NULL != current_node) {
            current_node->callback(new_mode,motion_detected);
            current_node = current_node->next_node;
        }
    }
    
    MODE_UNLOCK;
}

mode_handler_mode_t get_mode(void)
{
	MODE_LOCK;
    mode_handler_mode_t ret_val;
    
    ret_val = mode;

	MODE_UNLOCK;
    return ret_val;
}

void register_on_change_callback(mode_handler_callback_t callback)
{
	printf("%s.%s begin\n",__FILE__,__func__);
	MODE_LOCK;
    // Find the end of the list
    callback_list_node_t * current_node = callback_list_first;
    callback_list_node_t * previous_node;
    
    while (NULL != current_node) {
        previous_node = current_node;
        current_node = current_node->next_node;
    }
    
    // Append the callback
    current_node = (callback_list_node_t *)malloc(sizeof(callback_list_node_t));
    
    if (NULL != current_node) {
        current_node->callback = callback;
        current_node->next_node = callback_list_first;
        callback_list_first = current_node;
    } else {
        printf("Out of memory when registering callback\n");
    }
	printf("%s.%s end\n",__FILE__,__func__);
	MODE_UNLOCK;
}

// =============================================================================
// Private function definitions
// =============================================================================
