#ifndef __FAKECAPTURE_H
#define __FAKECAPTURE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#ifdef __MACH__
#include <sys/time.h>
#endif

#define IMAGE_JPEG "image/jpeg"

#define FRAME_BUF_SZ 100000

typedef unsigned long long capture_time;

struct _media_stream {
};

struct _media_frame {
    int sz;
    capture_time time;
    char data[FRAME_BUF_SZ];
};

typedef struct _media_stream media_stream;
typedef struct _media_frame media_frame;

#ifdef __MACH__
int clock_gettime(int clk_id, struct timespec* t);
#endif

/* open a fake camera stream.
 * the parameters are ignored
 */
media_stream *
capture_open_stream(const char *media_type, const char *media_props);

/**
 * @brief Read a media_frame from an open stream. 
 *
 * @param stream The structure associated with the stream.
 * 
 * @return A pointer to the data frame.
 */
media_frame *
capture_get_frame(media_stream *stream);

/**
 * @brief Obtain the data from the media_frame.
 *
 * @param frame The media_frame to obtain data from.
 *
 * @return A pointer to the data, NULL if frame is NULL.
 */
void *
capture_frame_data(const media_frame *frame);

/**
 * @brief Obtain the data size from the media_frame.
 *
 * @param frame The media_frame to obtain data size from.
 *
 * @return The size of the data, 0 if frame is NULL.
 */
size_t
capture_frame_size(const media_frame *frame);

/*
 * @param frame The media_frame to obtain timestamp from.
 *
 * @return The timestamp of the data, 0 if frame is NULL.
 */
capture_time
capture_frame_timestamp(const media_frame *frame);

/**
 * @brief Obtain the height of the media_frame.
 *
 * @param frame The media_frame to obtain height from.
 *
 * @return The height of the frame, 0 if frame is NULL.
 */
size_t
capture_frame_height(const media_frame *frame);

/**
 * @brief Obtain the width of the media_frame.
 *
 * @param frame The media_frame to obtain width from.
 *
 * @return The width of the frame, 0 if frame is NULL.
 */
size_t
capture_frame_width(const media_frame *frame);

void 
capture_frame_free(media_frame *frame);

void
capture_close_stream(media_stream *stream);
#endif
