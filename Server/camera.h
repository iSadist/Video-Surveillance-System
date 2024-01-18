#ifdef FAKE
#include "fakecapture.h"
#else
#include "capture.h"
#endif

#ifndef CAMERA_H
#define CAMERA_H

#define IMAGE_JPEG "image/jpeg"

/* A wrapper for the Axis capture API
 */

struct camera{
#ifdef FAKE
    int frame_nbr; // current position in fake stream
                   // use this for faking motion detection
                   // and for returning the correct "next" frame
#endif
    media_stream* stream;
};

// possibly
typedef struct camera camera;
typedef struct frame frame;
typedef char byte;

#ifdef FRAME_NOT_NEEDED
typedef frame media_frame;
#else
struct frame{
    media_frame* fr;
    int motion;
};
#endif

struct camera* camera_open();
void camera_close(struct camera*);
int analyse_frames(media_frame *, media_frame *);

struct frame* camera_get_frame(struct camera*);

byte* get_frame_bytes(struct frame*);

size_t get_frame_size(struct frame*);

capture_time get_frame_timestamp(struct frame*);

int get_frame_motion(struct frame*);

size_t get_frame_height(struct frame*);

size_t get_frame_width(struct frame*);

void frame_free(struct frame*);

#endif
