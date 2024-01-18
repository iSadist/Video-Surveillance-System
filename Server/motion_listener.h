#ifndef MOTION_LISTENER_H
#define MOTION_LISTENER_H

#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include "camera.h"
#include "mode_handler.h"

#define MOTION_THRESHOLD	10


/**
 * @brief Runs the motion thread.
 * @details Run this function as a new thread.
 * @param arg - not used.
 */
void * motion_callback(void * arg); 
 
/**
 * @brief Runs the motion detection logic.
 * @details This function is called from the motion_run function.
 */
void motion(void);

#endif // MOTION_LISTENER_H                 
