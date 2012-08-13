/********************************************************
 *
 * EYELINK PORTABLE EXPT SUPPORT
 * Copyright (c) 1996-2002 SR Research Ltd.
 *
 * 27 September 2002 by Suganthan Subramaniam
 * For non-commercial use only
 *
 * EyeLink library data extensions
 *        VERSION 2.5
 * UPDATED for EyeLink DLL V2.1: added LOST_DATA_EVENT
 * This module is for user applications
 * Use is granted for non-commercial
 * applications by Eyelink licencees only
 *
 * UNDER NO CIRCUMSTANCES SHOULD PARTS OF THESE FILES
 * BE COPIED OR COMBINED.  This will make your code
 * impossible to upgrade to new releases in the future,
 * and SR Research will not give tech support for
 * reorganized code.
 *
 * This file should not be modified. If you must modify it,
 * copy the entire file with a new name, and change the
 * the new file.
 *
 * EDFTYPES.H has platform-portable definitions COPYRIGHT
 * TERMS:  This file is part of the EyeLink application
 * support package.  You may compile this code, and may
 * read it to understand the EyeLink data structures.
 *
 *
 * DO NOT MODIFY THIS CODE IN ANY WAY.
 * SR Research will not support any problems caused by
 * modifying this code, and using modified code will make
 * your programs incompatible with future releases.
 *
 * This code has been tested on many 16 and
 * 32-bit compilers, including Borland and
 * Microsoft, GCC, in Windows, MAC OS X, Linux and DOS.
 * It should  compile without warnings on most platforms
 * If you must port to a new platform, check
 * the data types defined in EDFTYPES.H for
 * correct sizes.
 *
 *
 *
 ***********************************************************/


#include "edf.h"
#include "opt.h"
#include <stdio.h>




/***********************************************************************
 *Global variable externs
 ***********************************************************************/
extern Opt options;
extern double ppd_x;
extern double ppd_y;
extern double ppd_count;


/***********************************************************************
 *Global function externs
 ***********************************************************************/
extern void print_value(double val, int dp);



/***********************************************************************
 *Function Name: parse_type
 *Input: type
 *Output: returns the name of the type as c-string.
 *Purpose: To get the name of the type.
 ***********************************************************************/
char *parse_type(int p)
{
  switch(p)
    {
      case PARSEDBY_PUPIL: return "PUPIL";
      case PARSEDBY_HREF:  return "HREF";
      default:
      case PARSEDBY_GAZE:  return "GAZE";
    }
  return "";
}


int select_sample_type(int out_sample_data)
{
  if(options.preferred_sample_type == OUTPUT_PUPIL)
    {
      if(out_sample_data & SAMPLE_PUPILXY) return OUTPUT_PUPIL;
      else if(out_sample_data & SAMPLE_GAZEXY) return OUTPUT_GAZE;
      else return OUTPUT_HREF;
    }
  else if(options.preferred_sample_type == OUTPUT_HREF)
    {
      if(out_sample_data & SAMPLE_HREFXY) return OUTPUT_HREF;
      else if(out_sample_data & SAMPLE_GAZEXY) return OUTPUT_GAZE;
      else return OUTPUT_PUPIL;
    }
  else // default: GAZE
    {
      if(out_sample_data & SAMPLE_GAZEXY) return OUTPUT_GAZE;
      else if(out_sample_data & SAMPLE_HREFXY) return OUTPUT_HREF;
      else return OUTPUT_PUPIL;
    }
}



/***********************************************************************
 *Function Name: print_start
 *Input: recording structure and edffile structure
 *Output: int - returns 1 all the time.
 *Purpose: prints the start recording information.
 ***********************************************************************/
int print_start(RECORDINGS * rec, EDFFILE * combfile)
{


  if(!options.events_enabled )
  {
	  // make sure that we have the selection updated properly.
	  if(options.samples_enabled && (rec->record_type == 1 || rec->record_type ==3))
	  {
		  options.output_sample_type = select_sample_type(rec->sflags);
		  if(options.output_sample_type!=options.preferred_sample_type)
		  {
			printf("\rWARNING: preferred sample type %s not available: using %s data\n",
				parse_type(options.preferred_sample_type),
				parse_type(options.output_sample_type));
			//options.output_sample_type = rec->pos_type;
		  }
	  }


	  return 1;
  }
  if(options.events_enabled )
  {
      print("START\t%lu%s \t%s%s%s%s\n", rec->time,(options.out_float_time?".0":""),
		      ((options.eye_events_enabled||options.samples_enabled) && options.output_left_eye && options.out_event_left && (rec->eye == 1 || rec->eye == 3))   ? "LEFT\t"  : "",
		      ((options.eye_events_enabled||options.samples_enabled) && options.output_right_eye && options.out_event_right && (rec->eye == 2 || rec->eye == 3)) ? "RIGHT\t" : "" ,
		      ((rec->record_type == 1 || rec->record_type ==3) ?"SAMPLES\t":""),
		      ((rec->record_type == 2 || rec->record_type ==3) ?"EVENTS":""));
      print("PRESCALER\t1\n");
      print("VPRESCALER\t1\n");
      print("PUPIL\t%s\n", rec->pupil_type ==  0 ? "AREA" : "DIAMETER");
  }
  if(options.eye_events_enabled && (rec->record_type == 2 || rec->record_type ==3))
    {
	  if(options.output_event_type != rec->pos_type)
		  options.output_event_type = rec->pos_type;

      print("EVENTS\t%s", parse_type(rec->pos_type));
      if(options.output_left_eye && options.out_event_left && (rec->eye == 1 || rec->eye == 3))     print("\tLEFT");
      if(options.output_right_eye && options.out_event_right && (rec->eye == 2 || rec->eye == 3))    print("\tRIGHT");
      if(options.output_resolution)  print("\tRES");
	  //if(options.out_marker_fields == 2 && rec->sflags&SAMPLE_HEADPOS) print("\tHTARGET");

      print("\tRATE\t%7.2f", rec->sample_rate);
      if(edf_get_revision(combfile) > 0)
		{                /* EYELINK II EXTENSION */
			print("\tTRACKING\t%s", rec->recording_mode ? "CR" : "P");
			print("\tFILTER\t%d", rec->filter_type);
		}
      print("\n");
    }

  if(options.samples_enabled && (rec->record_type == 1 || rec->record_type ==3))
  {

	  options.output_sample_type = select_sample_type(rec->sflags);
      if(options.output_sample_type!=options.preferred_sample_type)
	  {
		printf("\rWARNING: preferred sample type %s not available: using %s data\n",
			parse_type(options.preferred_sample_type),
			parse_type(options.output_sample_type));
		//options.output_sample_type = rec->pos_type;
	  }

      if(options.events_enabled )
      {
	      print("SAMPLES\t%s", parse_type(options.output_sample_type));
	      if(options.output_left_eye && options.out_sample_left && (rec->eye == 1 || rec->eye == 3))     print("\tLEFT");
	      if(options.output_right_eye && options.out_sample_right && (rec->eye == 2 || rec->eye == 3))    print("\tRIGHT");
	      if(options.output_sample_velocity) print("\tVEL");
	      if(options.output_resolution)   print("\tRES");
          if(options.out_marker_fields==1)  print("\tHMARKER");
		  if(options.out_marker_fields == 2 && rec->sflags&SAMPLE_HEADPOS) print("\tHTARGET");
	      print("\tRATE\t%7.2f", rec->sample_rate);
	      if(edf_get_revision(combfile) > 0)
		    {                /* EYELINK II EXTENSION */
		      print("\tTRACKING\t%s", rec->recording_mode ? "CR" : "P");
		      print("\tFILTER\t%d", rec->filter_type);
		    }
		  if(options.output_input_values)
			  print("\tINPUT");
		  if(options.output_button_values)
			  print("\tBUTTONS");
	      print("\n");
      }
  }



  return 1;
}



/***********************************************************************
 *Function Name: print_end
 *Input: recording structure and edffile structure
 *Output: int - returns 1 all the time.
 *Purpose: prints the end recording information.
 ***********************************************************************/
int print_end(RECORDINGS * rec)
{
  if(options.events_enabled )
  {
      print("END\t%lu \t%s%s\tRES", rec->time,
		      ((rec->record_type == 1 || rec->record_type ==3) ?"SAMPLES\t":""),
		      ((rec->record_type == 2 || rec->record_type ==3) ?"EVENTS":""));
      if(ppd_count)
        {
          print_value(ppd_x/ppd_count, 2);
          print_value(ppd_y/ppd_count, 2);
        }
      else
        {
          print_value(options.default_resolution_x, 2);
          print_value(options.default_resolution_y, 2);
        }
      print("\n");
  }
  return 1;

}



/***********************************************************************
 *Function Name: print_recording
 *Input: recording structure and edffile structure
 *Output: int
 *Purpose: prints the start or end recording information based on state
 ***********************************************************************/
int print_recording(EDFFILE * combfile, RECORDINGS * rec)
{
	if(rec->state == 0)
	{
		return print_end(rec);
	}
	else
	{
		return print_start(rec,combfile);
	}
}

