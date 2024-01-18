#include "camera.h"
#include <stdio.h>

camera* camera_open(){
	camera *cam = malloc(sizeof(camera));
	if (!cam){
		perror("Out of memory, cannot malloc camera");
		return NULL;
	}
#ifdef FAKE
	cam->frame_nbr = 0;
#endif
	cam->stream = capture_open_stream(IMAGE_JPEG, "fps=25&resolution=640x480");
	return cam;
}

void camera_close(camera* cam){
	capture_close_stream(cam->stream);
	free(cam);
}

frame* camera_get_frame(camera* cam){
	frame *f = malloc(sizeof(frame));
	f->fr = capture_get_frame(cam->stream);
#ifdef FAKE
	f->motion = (cam->frame_nbr >= 86 && cam->frame_nbr <= 240);
	cam->frame_nbr = (cam->frame_nbr % 247) + 1;
#else
	//TODO
	//f->motion = analyseframes(capture_get_frame(cam->stream, */prev_frame*/);
#endif
	return f;
}

//From axis example code
int analyse_frames(media_frame *cur_frame, media_frame *prev_frame){
	int   i, j;
	char *prev_data  = NULL;
	int   prev_width;
	int   prev_height;
	//int   prev_stride;
	char *cur_data  = NULL;
	int   cur_width;
	int   cur_height;
	//int   cur_stride;
	int   result = 0;

	prev_data   = (char *)capture_frame_data(prev_frame);
	prev_width  = capture_frame_width(prev_frame);
	prev_height = capture_frame_height(prev_frame);
	//prev_stride = capture_frame_stride(prev_frame);

	cur_data   = (char *)capture_frame_data(cur_frame);
	cur_width  = capture_frame_width(cur_frame);
	cur_height = capture_frame_height(cur_frame);
	//cur_stride = capture_frame_stride(cur_frame);;

	/* simple pixel diff */
	for (i = 0; i < cur_height; i++) {
		for (j = 0; j < cur_width; j++) {
			int c_pos = i /* * cur_stride */ + j;
			int p_pos = i /* * prev_stride */ + j;
			result += abs(cur_data[c_pos] - prev_data[p_pos]);
		}
	}
	result /= (cur_width * cur_height);

	return result; //Arbitrary value used in the axis example 
}

byte* get_frame_bytes(frame* f){
	return (byte *) capture_frame_data(f->fr);
}

size_t get_frame_size(frame* f){
	return capture_frame_size(f->fr);
}

capture_time get_frame_timestamp(frame* f){
	return capture_frame_timestamp(f->fr);
}

int get_frame_motion(frame* f){
	return f->motion;
}

size_t get_frame_height(frame* f){
	return capture_frame_height(f->fr);
}

size_t get_frame_width(frame* f){
	return capture_frame_width(f->fr);
}

void frame_free(frame* f){
	capture_frame_free(f->fr);
	free(f);
}
