#ifndef __SR_RESEARCH_SAMPLE_OPT__
#define __SR_RESEARCH_SAMPLE_OPT__
#include <stdio.h>
typedef struct
{
	int events_enabled;
	int use_tabs;
	int output_event_type;
	int msg_events_enabled;
	int output_resolution;
	float default_resolution_x;
	float default_resolution_y;
	int force_resolution;
	int out_event_left;
	int out_event_right;
	int eye_events_enabled;
	int output_sample_type;
	int out_sample_left;
	int output_sample_velocity;
	int out_sample_right;
	int out_events;


	int preferred_sample_type;
	int out_sample_flags;
#ifdef WIN32
    int gazeMap;
    int insertFrameNumber;
#endif

    int out_marker_fields;

    int out_averages;


	/* only  used in main */
	int fast_velocity;
	int output_left_eye;
	int output_right_eye;
	int samples_enabled;
	int start_events_enabled;
	int enable_consistency_check;
	int verbose;
	int hide_viewer_commands;

	char * logfile_name;
	char * new_path;
	FILE * outfile;
	int output_elcl;
	int overwrite_asc_ifexists;

	int out_float_time;
	int output_input_values;
	int output_button_values;
	int enable_failsafe;
	int enable_htarget;

	int Utf8BOM;
	int disable_large_time_stamp_check;
	int allow_raw;

}Opt;

#define OUTPUT_GAZE  PARSEDBY_GAZE
#define OUTPUT_HREF  PARSEDBY_HREF
#define OUTPUT_PUPIL PARSEDBY_PUPIL
#define NaN 1e8                  /* missing floating-point values*/

void print(const char *fmt, ...);
void parseDisplayAreaCoords(char *coords);
void parseDisplayCoords(char *coords);
#endif

