#include "motion_listener.h"

// =============================================================================
// Public functions
// =============================================================================
void * motion_callback(void * arg)
{	    
	media_stream * stream; 
	media_frame * frame_one, * frame_two;
	int result;

	stream = capture_open_stream(IMAGE_UNCOMPRESSED, "fps=25&resolution=640x480");

	while ( 1 ) {
		frame_one = capture_get_frame(stream);	
		usleep(100000);
		frame_two = capture_get_frame(stream);
		result = analyse_frames(frame_one, frame_two);
		//printf("----RESULTS!!!!---- : %d\n", result);
		if ( result > MOTION_THRESHOLD ) {
	
			set_mode(MODE_MOVIE, 1);
			//printf("MOTION DETECTED!!!!!!!!!!!!!!!!!!!!!!\n");	

		}

		capture_frame_free(	frame_one );
		capture_frame_free( frame_two );
	
	}

	capture_close_stream(stream);

    return NULL;
}
 
void motion(void)
{
    // TODO implement void motion(void)
}
