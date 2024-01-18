#ifndef MODE_HANDLER_H
#define MODE_HANDLER_H

#include <errno.h>
#include <string.h>
#include "logger.h"

// =============================================================================
// Public types
// =============================================================================

typedef enum {
    MODE_IDLE,
    MODE_MOVIE
} mode_handler_mode_t;

// Callback function pointer type
typedef void(*mode_handler_callback_t)(mode_handler_mode_t, int);

// =============================================================================
// Public functions
// =============================================================================

/**
 * @brief Initializes the mode_handler.
 */
void init_mode_handler(void);

/**
 * @brief Closes the mode handler and returns any allocated resources.
 */
void close_mode_handler(void);

/**
 * @brief Sets the current mode of operation.
 * @param new_mode - the new mode of operation.
 * @param motion_detected - flag for motion detected.
 *
 * @details This function will call all the registered callback functions
 *          with the new mode.
 * This function is reentrant.
 */
void set_mode(mode_handler_mode_t new_mode, int motion_detected);

/**
 * @brief Gets the current mode of operation.
 * @details This function is reentrant.
 */
mode_handler_mode_t get_mode(void);

/**
 * @brief Registers a callback function to be called when the mode changes.
 * @param callback - Callback function to register.
 */
void register_on_change_callback(mode_handler_callback_t callback);

#endif // MODE_HANDLER_H
