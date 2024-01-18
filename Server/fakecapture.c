#include <math.h>
#include <time.h>
#include "fakecapture.h"

#define DEBUG
#define START_FRAME 1
#define NUM_FRAMES 247
#define FILEPATH_LENGTH 100
static int frame_nr = START_FRAME;

#ifdef __MACH__
#define UNUSED_VALUE 1
#define CLOCK_MONOTONIC UNUSED_VALUE
int clock_gettime(int clk_id, struct timespec* t) {
    struct timeval now;
    int res;
    if((res=gettimeofday(&now, NULL))){
        return res;
    }
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif

media_stream *
capture_open_stream(const char *media_type, const char *media_props)
{
    media_stream *res = malloc(sizeof(media_stream));
    return res;
}

media_frame *
capture_get_frame(media_stream *stream)
{
   int fd;
   char fname[FILEPATH_LENGTH];
   fname[FILEPATH_LENGTH - 1]='\0';
   snprintf(fname, FILEPATH_LENGTH - 1, "media/film%03d.jpg", frame_nr++);
   if(frame_nr > NUM_FRAMES) {
      frame_nr = START_FRAME;
   };
   printf("trying to open %s\n", fname);
   fd = open(fname, O_RDONLY);
   if(fd < 0) {
       perror("fakecapture:capture_get_frame:open");
       return NULL;
   }
   media_frame* res = malloc(sizeof(media_frame));

   struct timespec mono_current_time;
   clock_gettime(CLOCK_MONOTONIC, &mono_current_time);
   res->time = (capture_time) mono_current_time.tv_sec * pow(10, 9) + mono_current_time.tv_nsec;

   res->sz = read(fd, res->data, FRAME_BUF_SZ);
   if(res->sz < 0) {
       perror("fakecapture:capture_get_frame:read");
       free(res);
       res=NULL;
   }
   if(close(fd)){
       perror("Warning! fakecapture:capture_get_frame:close");
   }

   return res;
}

/**
 * @brief Obtain the data from the media_frame.
 *
 * @param frame The media_frame to obtain data from.
 *
 * @return A pointer to the data, NULL if frame is NULL.
 */
void *
capture_frame_data(const media_frame *frame)
{
    return (void*) frame->data;
}

/**
 * @brief Obtain the data size from the media_frame.
 *
 * @param frame The media_frame to obtain data size from.
 *
 * @return The size of the data, 0 if frame is NULL.
 */
size_t
capture_frame_size(const media_frame *frame)
{
    return frame->sz;
}

 /*
 * @param frame The media_frame to obtain timestamp from.
 *
 * @return The timestamp of the data, 0 if frame is NULL.
 */
capture_time
capture_frame_timestamp(const media_frame *frame)
{
    //fprintf(stderr, "WARNING! capture_frame_timestamp not implemented\n");
    if (!frame)
        return 0;
    return frame->time;
}

/**
 * @brief Obtain the height of the media_frame.
 *
 * @param frame The media_frame to obtain height from.
 *
 * @return The height of the frame, 0 if frame is NULL.
 */
size_t
capture_frame_height(const media_frame *frame)
{
    return 480;
}

/**
 * @brief Obtain the width of the media_frame.
 *
 * @param frame The media_frame to obtain width from.
 *
 * @return The width of the frame, 0 if frame is NULL.
 */
size_t
capture_frame_width(const media_frame *frame)
{
    return 640;
}

void
capture_frame_free(media_frame *frame)
{
    free(frame);
}

void
capture_close_stream(media_stream *stream)
{
    free(stream);
}
