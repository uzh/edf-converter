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


#include <stdio.h>
#include <string.h>
#include "edf.h"
#include <math.h>
#include <ctype.h>
#include "opt.h"





/***********************************************************************
 *Global variable externs
 ***********************************************************************/

extern Opt options;
extern char missing_string[];
extern double ppd_x;
extern double ppd_y;
extern double ppd_count;


#ifdef WIN32
float getMappedX(float x);
float getMappedY(float y);
#else
#define getMappedX(x) x
#define getMappedY(y) y
#endif








/***********************************************************************
 *Function Name: trim_lstring
 *Input: input string and maximum length
 *Output: Nothing
 *Purpose: Trims the given string to a maximum string and removes CR
 ***********************************************************************/
void trim_lstring(LSTRING *str, int maxlen)
	{
	  int l;
	  char *c;

	  c = &(str->c);
	  l = str->len;
	  if(l<0)
		  return;
	  else if(l ==0)
	  {
		c[l] =0;
		return;
	  }
	  if(l>maxlen) l = maxlen;
	  l--;
	  c[l] = 0;
	  l--;
	  if(l>=0 && c[l]==0) l--;
	  if(l>=0 && c[l]==0) l--;
	  if(l>=0 && c[l]==0) l--;
	  if(l>=0 && c[l]==0) l--;
	  if(l>=0 && c[l]=='\n') c[l] = 0;
	}


/***********************************************************************
 *Function Name: print_value
 *Input: value and decimal points
 *Output: Nothing
 *Purpose: prints the double value in given decimal points.
 ***********************************************************************/
void print_value(double val, int dp)
{
	if(options.use_tabs)
	{
		if(val==NaN)
		{
		  print("\t%s", missing_string);
		  return;
		}

	  if(dp==0)
		print("\t%.0f", val);
	  else
		{
		  float am =(float) fabs(val);
		  if(dp==1 && am>=100000.0) print("\t%.1e", val);
		  else if(dp==2 && am>=10000.0) print("\t%.1e", val);
		  else if(dp>2  && am>=1000.0) print("\t%.1e", val);
		  else print("\t%.*f", dp, val);
		}
	}
	else
	{
	  if(val==NaN)
		{
		  print("\t%s", missing_string);
		  return;
		}

	  if(dp==0)
		print("\t%7.0f", val);
	  else
		{
		  float am =(float) fabs(val);
		  if(dp==1 && am>=100000.0) print("\t %7.1e", val);
		  else if(dp==2 && am>=10000.0) print("\t %7.1e", val);
		  else if(dp>2  && am>=1000.0) print("\t %7.1e", val);
		  else print("\t%7.*f", dp, val);
		}
	}
}


/***********************************************************************
 *Function Name: is_start_event
 *Input: event
 *Output: int
 *Purpose: returns 1 if the event is a start event and 0 otherwise
 ***********************************************************************/
int is_start_event(FEVENT *fevent)
{
	switch(fevent->type)
	{
		case STARTBLINK:
		case STARTSACC:
		case STARTFIX:
		case STARTSAMPLES:
		case STARTEVENTS:
		case STARTPARSE:
			return 1;
		default:
			return 0;
	}
}


/***********************************************************************
 *Function Name: print_event
 *Input: event and sample interval
 *Output: int
 *Purpose: prints an event in readable asc format
 ***********************************************************************/
int print_event(FEVENT *fevent, int sample_int)
{
  int i;
  /* 3 types of events sac, blink, fix - all has 2 sides and are pairs*/
  static unsigned int eventpairs[3][2][2] = {{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}}};



  unsigned short b, bs;
  unsigned short etype = fevent->type;




	if( options.output_event_type == PARSEDBY_GAZE&&  (fevent->read & READ_GRES) && !options.samples_enabled)
	{

	  if(fevent->supd_x>0.01 && fevent->supd_x<10000 && fevent->supd_y>0.01 && fevent->supd_y<10000 )
	   {
		  ppd_x += fevent->supd_x;
		  ppd_y += fevent->supd_y;
		  ppd_count++;
	   }
	  if(fevent->eupd_x>0.01 && fevent->eupd_x<10000 && fevent->eupd_y>0.01 && fevent->eupd_y<10000 )
	   {
		  ppd_x += fevent->eupd_x;
		  ppd_y += fevent->eupd_y;
		  ppd_count++;
	   }
	}

  if(is_start_event(fevent) && !options.start_events_enabled )
	  return 1;
  if(!options.events_enabled) return 1;

  switch(etype)
  {
	    case STARTBLINK:
		case STARTSACC:
		case STARTFIX:

		case ENDBLINK:
		case ENDSACC:
		case ENDFIX:
			if(!options.eye_events_enabled) return 1;
            if(fevent->eye)
              {/* right eye event */
	            if(!options.output_right_eye) return 1;
              }
              else
              {/* left eye event */
	            if(!options.output_left_eye) return 1;
              }
			break;
  }
  switch(etype)
  {
      case BREAKPARSE:
		print("** BREAK IN DATA STREAM **\n");
		break;

      case STARTBLINK:
		  eventpairs[0][0][fevent->eye] = fevent->sttime;
		if(options.use_tabs)
			print("SBLINK\t%c\t%lu%s\n", fevent->eye?'R':'L', fevent->sttime,(options.out_float_time?".0":""));
		else
			print("SBLINK %c %lu%s\n", fevent->eye?'R':'L', fevent->sttime,(options.out_float_time?".0":""));
		break;

	  case ENDBLINK:
		  if(options.verbose && (fevent->entime < fevent->sttime) )
		  {
			  printf("###ENDBLINK end time is less than start time %lu %lu\n",fevent->sttime, fevent->entime);
		  }
		  eventpairs[0][1][fevent->eye] = fevent->sttime;
		if(options.use_tabs)
			print("EBLINK\t%c\t%lu\t%lu\t%lu\n", fevent->eye?'R':'L',
				fevent->sttime, fevent->entime, fevent->entime - fevent->sttime + sample_int);
		else
			print("EBLINK %c %lu\t%lu\t%lu\n", fevent->eye?'R':'L',
				fevent->sttime, fevent->entime, fevent->entime - fevent->sttime + sample_int);
		break;

	  case STARTSACC:
		  eventpairs[1][0][fevent->eye] = fevent->sttime;
		if(options.use_tabs)
			print("SSACC\t%c\t%lu%s\n", fevent->eye?'R':'L', fevent->sttime,(options.out_float_time?".0":""));
		else
			print("SSACC %c  %lu%s\n", fevent->eye?'R':'L', fevent->sttime,(options.out_float_time?".0":""));
		break;

	  case ENDSACC:
		  if(fevent->entime < fevent->sttime)
		  {
			  printf("###ENDSACC end time is less than start time %lu %lu\n",fevent->sttime, fevent->entime);
		  }
		  eventpairs[1][1][fevent->eye] = fevent->sttime;
		if(options.use_tabs)
			print("ESACC\t%c\t%lu\t%lu\t%lu", fevent->eye?'R':'L',
				fevent->sttime, fevent->entime, fevent->entime - fevent->sttime + sample_int);
		else
			print("ESACC %c  %lu%s\t%lu%s\t%lu", fevent->eye?'R':'L',
				fevent->sttime,(options.out_float_time?".0":""), fevent->entime,(options.out_float_time?".0":""),
				fevent->entime - fevent->sttime + sample_int);
		switch(options.output_event_type)
		{
			case OUTPUT_PUPIL:
			print_value(getMappedX(fevent->gstx), 1);
			print_value(getMappedY(fevent->gsty), 1);
			print_value(getMappedX(fevent->genx), 1);
			print_value(getMappedY(fevent->geny), 1);
			if(fevent->read & READ_GRES)   /* CHANGED FOR REAL SACCADE AMPLITUDE */
			{
				print_value(hypot(
					getMappedX((fevent->gstx-fevent->genx))/((fevent->supd_x+fevent->eupd_x)/2.0),
					getMappedY((fevent->gsty-fevent->geny))/((fevent->supd_y+fevent->eupd_y)/2.0)), 2);
				if(fevent->read & READ_VEL)
					print_value(fevent->pvel, 0);
				else
					print_value(NaN, 0);
			}
			else if(fevent->read & READ_VEL)
			{
				print_value(fevent->avel*((float)(fevent->entime - fevent->sttime + sample_int)), 2);
				print_value(fevent->pvel, 1);
			}
			else
			{
				print("\t%s\t%s", missing_string, missing_string);
			}
			break;
			case OUTPUT_HREF:
			  print_value(fevent->hstx, 0);
			  print_value(fevent->hsty, 0);
			  print_value(fevent->henx, 0);
			  print_value(fevent->heny, 0);
			  if(1)  /* CHANGED FOR REAL SACCADE AMPLITUDE */
				{
					float x1 = fevent->hstx;
					float y1 = fevent->hsty;
					float x2 = fevent->henx;
					float y2 = fevent->heny;
					float diff =(float)( (15000.0*15000.0+x1*x2+y1*y2)/
						sqrt((15000.0*15000.0+x1*x1+y1*y1) *
							 (15000.0*15000.0+x2*x2+y2*y2) ));

					print_value(57.296*acos(diff), 2);
					if(fevent->read & READ_VEL)
						print_value(fevent->pvel, 0);
					else
						print_value(NaN, 0);
				}
			  else if(fevent->read & READ_VEL)
				{
					print_value(fevent->avel*((float)(fevent->entime - fevent->sttime + sample_int)), 2);
					print_value(fevent->pvel, 1);
				}
			  break;
			case OUTPUT_GAZE:
			default:
				print_value(getMappedX(fevent->gstx), 1);
				print_value(getMappedY(fevent->gsty), 1);
				print_value(getMappedX(fevent->genx), 1);
				print_value(getMappedY(fevent->geny), 1);
				if(fevent->read & READ_GRES)  /* CHANGED FOR REAL SACCADE AMPLITUDE */
				{
					print_value(hypot(
						(getMappedX(fevent->gstx-fevent->genx))/((fevent->supd_x+fevent->eupd_x)/2.0),
						getMappedY((fevent->gsty-fevent->geny))/((fevent->supd_y+fevent->eupd_y)/2.0)), 2);
					if(fevent->read & READ_VEL)
						print_value(fevent->pvel, 0);
					else
						print_value(NaN, 0);
				}
				else if(fevent->read & READ_VEL)
				{
					print_value(fevent->avel*((float)(fevent->entime - fevent->sttime + sample_int)/1000.0), 2);
					print_value(fevent->pvel, 1);
				}
				else
				{
					print("\t%s\t%s", missing_string,missing_string);
				}
			  break;
	  }

	if(options.output_resolution)
	 switch(options.output_event_type)
	   {
			case OUTPUT_HREF:
			{
			  float x1 = (float)((fevent->hstx+fevent->henx)/2.0);
			  float y1 = (float)((fevent->hsty+fevent->heny)/2.0);
			  float dx = (float)((15000.0*15000.0+x1*x1+y1*y1)/
					sqrt(15000.0*15000.0+y1*y1));
			  float dy = (float)((15000.0*15000.0+x1*x1+y1*y1)/
					sqrt(15000.0*15000.0+x1*x1));
			  print_value(dx/57.2958, 2);
			  print_value(dy/57.2958, 2);
			}
			  break;
			case OUTPUT_PUPIL:
			case OUTPUT_GAZE:
			default:
			  if((fevent->read&READ_GRES) && !options.force_resolution)
				{
					print_value((fevent->supd_x+fevent->eupd_x)/2.0, 2);
					print_value((fevent->supd_y+fevent->eupd_y)/2.0, 2);
				}
			  else
				{
					print_value(options.default_resolution_x, 2);
					print_value(options.default_resolution_y, 2);
				}
				break;
	   }
	print("\n");
	break;

      case STARTFIX:
		 eventpairs[2][0][fevent->eye] = fevent->sttime;
		if(options.use_tabs)
			print("SFIX\t%c\t%lu%s\n", fevent->eye?'R':'L', fevent->sttime,(options.out_float_time?".0":""));
		else
			print("SFIX %c   %lu%s\n", fevent->eye?'R':'L', fevent->sttime,(options.out_float_time?".0":""));
		break;
	  case ENDFIX:
		  if(fevent->entime < fevent->sttime)
		  {
			  printf("###ENDFIX end time is less than start time %lu %lu\n",fevent->sttime, fevent->entime);
		  }
		  eventpairs[2][1][fevent->eye] = fevent->sttime;
		if(options.use_tabs)
			print("EFIX\t%c\t%lu%s\t%lu%s\t%lu", fevent->eye?'R':'L',
				fevent->sttime,(options.out_float_time?".0":""),
				fevent->entime,(options.out_float_time?".0":""),
				fevent->entime - fevent->sttime + sample_int);
		else
			print("EFIX %c   %lu%s\t%lu%s\t%lu", fevent->eye?'R':'L',
				fevent->sttime, (options.out_float_time?".0":""),
				fevent->entime, (options.out_float_time?".0":""),
				fevent->entime - fevent->sttime + sample_int);
		switch(options.output_event_type)
		{
			case OUTPUT_HREF:
			  print_value(fevent->havx, 1);
			  print_value(fevent->havy, 1);
			  print_value(fevent->ava,  0);
			case OUTPUT_PUPIL:
			case OUTPUT_GAZE:
			default:
			  print_value(getMappedX(fevent->gavx), 1);
			  print_value(getMappedY(fevent->gavy), 1);
			  print_value(fevent->ava,  0);
		}
		if(options.output_resolution)
			switch(options.output_event_type)
			{
				case OUTPUT_HREF:
					if(fevent->read & READ_GRES)
					{
						print_value((fevent->supd_x+fevent->eupd_x)/2.0, 2);
						print_value((fevent->supd_y+fevent->eupd_y)/2.0, 2);
					}
					else
					{
						float x1 = getMappedX(fevent->gavx);
						float y1 = getMappedY(fevent->gavy);
						float dx = (float)((15000.0*15000.0+x1*x1+y1*y1)/
							sqrt(15000.0*15000.0+y1*y1));
						float dy = (float)((15000.0*15000.0+x1*x1+y1*y1)/
							sqrt(15000.0*15000.0+x1*x1));
						print_value(dx/57.2958, 2);
						print_value(dy/57.2958, 2);
					}
				break;
				case OUTPUT_PUPIL:
				case OUTPUT_GAZE:
				default:
					if(fevent->read & READ_GRES)
					{
						print_value((fevent->supd_x+fevent->eupd_x)/2.0, 2);
						print_value((fevent->supd_y+fevent->eupd_y)/2.0, 2);
					}
					else
					{
						print("\t%s\t%s", missing_string,missing_string);
					}
					break;
			}
			print("\n");
			break;

      case BUTTONEVENT:
			b  = fevent->buttons>>8;
			bs = 255 & fevent->buttons;	/* button DOWN flags*/
			if(!b) break;
			for(i=1;i<9;i++)
			{
				if(b&1)
				{
					print("BUTTON\t%lu%s\t%d\t%d\n", fevent->sttime,(options.out_float_time?".0":""), i, bs&1);
				}
				b  >>= 1;
				bs >>= 1;
			}
			break;

      case MESSAGEEVENT:
			if(!options.msg_events_enabled) break;
			if(options.hide_viewer_commands)
			{
				char *msg = &(fevent->message->c);
				int ln = strlen(&(fevent->message->c));
				int i =0;
				for(; i < ln && msg[i] == ' '; i++); // skip the spaces at the beginning 
				// message may start with a number
				if(i<ln && msg[i] == '-') i++; // skip the negative sign
				for(; i < ln && isdigit(msg[i]); i++); // skip the numbers
				for(; i < ln && msg[i] == ' '; i++); // skip any more spaces
	
				if(i+2 < ln &&  strncmp(msg+i,"!V",2) == 0) break;// found !V cmd
				if(i+3 < ln &&  strncmp(msg+i,"!EG",3) == 0) break; // found !EG cmd
			}

			if(!fevent->message || fevent->message->len==0) break;
			trim_lstring(fevent->message, 140);
			if(options.use_tabs)
				print("MSG\t%lu%s\t%s\n", fevent->sttime,(options.out_float_time?".0":""), &(fevent->message->c));
			else
				print("MSG\t%lu%s %s\n", fevent->sttime,(options.out_float_time?".0":""), &(fevent->message->c));
			break;
      case INPUTEVENT:
            print("INPUT\t%lu%s\t%d\n", fevent->sttime,(options.out_float_time?".0":""), fevent->input);
            break;
      default:
			break;
  }
  if(options.verbose)
  {
	int i ;
	int j;
	for(i =0; i <3; i++)
	{
		for(j =0; j <2; j++)
		{
			if(eventpairs[i][0][j] != -1 && eventpairs[i][1][j] != -1)
			{
				if(eventpairs[i][0][j] != eventpairs[i][1][j])
				{
					printf("start time miss match %s %u %u %d\n", (i==0)? "BLINK" : (i ==1)? "SAC" :"FIX",
						eventpairs[i][0][j], eventpairs[i][1][j],eventpairs[i][1][j]-eventpairs[i][0][j]);
				}
				eventpairs[i][0][j] = eventpairs[i][1][j] = -1;
			}
		}
	}
  }
  return 0;

}

