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



#include "mex.h"
#include "edf.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#ifndef __MWERKS__
	#include <sys/types.h>
	#include <sys/timeb.h>
#endif
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include "opt.h"

#ifndef WIN32
#define stricmp strcasecmp
#else //if windows - at the moment no scene camera stuff is supported in non windows
/* scene camera related stuff. */
void closeLookupTable();
int openLookupTable(char *edffile, int trial, int  retry);
unsigned int getNextFrameTime();
int getLastFrameNumber();
#endif

#ifndef progress_update
#define progress_update print_percentage
#endif

#define EDF_CANCEL -2
#define EDF_ERROR  -1
#define EDF_OK     0

/************************************************************
 *
 * Utility functions that prints various elements of EDFFILE.
 *
 ************************************************************/
extern int print_event(FEVENT *fevent, int sample_int);
extern int print_recording(EDFFILE * combfile, RECORDINGS * rec);
extern int write_sample(EDFFILE * combfile, RECORDINGS * rec,
						FSAMPLE *s);
/************************************************************
 *
 * function to print usage
 *
 ************************************************************/
void print_usage(char * program);

/************************************************************
 *
 * parses the incoming options and sets the options structure
 *
 ************************************************************/
int parse_options(int argc, char ** argv,
				  int *nooptc, char ***nooptv);

/************************************************************
 *
 * parses the incoming options and sets the options structure
 *
 ************************************************************/
int process_edf_file(char * edffile,char *outfile);

/************************************************************
 *
 *Global variable declarations.
 *
 ************************************************************/
Opt options;
char missing_string[20] = "   .";
extern double ppd_x;
extern double ppd_y;
extern double ppd_count;


/***********************************************************************
 *Function Name: replace
 *Input: instring, find char, replace char
 *Output: none
 *Purpose: in the given string replaces the find char with replace char
 ***********************************************************************/
void replace ( char * ins, char findc, char replacec)
{
	int len = strlen(ins);
	int i =0;
	for(i =0; i< len;i ++)
	{
		if(ins[i] == findc)
			ins[i] = replacec;
	}
}

/***********************************************************************
 *Function Name: replace
 *Input: instring, find char, replace char
 *Output: int
 *Purpose: in the given string count the find char
 ***********************************************************************/
int counttoken ( char * ins, char findc)
{
	int len = strlen(ins);
	int i =0;
	int j =0;
	for(i =0; i< len;i ++)
	{
		if(ins[i] == findc)
			j++;
	}
	return j;
}
/***********************************************************************
 *Function Name: init_options
 *Input: None
 *Output: None
 *Purpose: initializes the struct variable options.
 ***********************************************************************/
void init_options()
{
	/* for events */
	options.output_resolution = 0;      /* 1 to append resolution to samples, events*/
	options.events_enabled = 1;         /* 0 to output numerical samples only */
	options.msg_events_enabled = 1;     /* 0 to output no messages*/
	options.output_event_type = OUTPUT_GAZE;	  /* actual event data type*/
	options.force_resolution = 0;
	options.default_resolution_x = NaN;
	options.default_resolution_y = NaN;
	options.use_tabs = 0;

	options.output_sample_type = OUTPUT_GAZE;
	options.out_sample_left = 1;
	options.output_sample_velocity = 0;
	options.out_sample_right =1;
	options.out_events = 1;


	options.preferred_sample_type = OUTPUT_GAZE;
	options.out_sample_flags = 1;
	options.fast_velocity = 0;
	options.output_left_eye =1;
	options.output_right_eye = 1;
	options.outfile = NULL;



	options.samples_enabled = 1;
	options.start_events_enabled=1;

	options.logfile_name = NULL;
	options.new_path = NULL;
	options.out_event_left = 1;
	options.out_event_right =1;
	options.eye_events_enabled = 1;
	options.enable_consistency_check = 2;
	options.verbose = 0;
	options.hide_viewer_commands=0;
	options.output_button_values=0;

#ifdef WIN32
    options.gazeMap = 0;
    options.insertFrameNumber =0;
#endif
    options.out_marker_fields = 0;
    options.out_averages=0;

	options.output_elcl = 0;
	options.overwrite_asc_ifexists =0;


	options.out_float_time =0;
	options.output_input_values =0;

	options.enable_failsafe = 0;
	options.enable_htarget=1;

	options.Utf8BOM = 0;
	options.disable_large_time_stamp_check = 0;
}

/***********************************************************************
 *Function Name: lowerstring
 *Input: input String
 *Output: none
 *Purpose: Makes the upper case char in the string lower
 ***********************************************************************/
void lowerstring(char * str)
{
	int i =0;
	int len = strlen(str);
	for(i =0; i < len; i++)
	{
		str[i] = tolower(str[i]);
	}
}

/***********************************************************************
 *Function Name: is_edf_file
 *Input: filename
 *Output: true/false : 1/0
 *Purpose: Checks the file name and returns true if the file name ends
 *			with .edf
 ***********************************************************************/
int is_edf_file(char *fname)
{
	int ln =0;
	if(!fname)
		return 0;
	ln =strlen(fname);
	if( ln> 4)
	{
		char ext[5];
		strcpy(ext,&fname[ln -4]);
		lowerstring(ext);
		if(strcmp(ext,".edf")==0)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}
/***********************************************************************
 *Function Name: process
 *Input: argc, argv - same arguments as passed by the system
 *Output: int
 *Purpose: main function of the application.
 ***********************************************************************/
void process(int argc, char ** argv)
{
	int i;
	int filec;
	char **files;
	init_options();  /* initialize the options structure */
	if(!parse_options(argc, argv,&filec, &files)) /* parse the passed in options */
	{
		return;
	}
	for(i =0; i< filec; i++)
	{
		printf("processing file %s \n", files[i]);
		if(i+1 <filec && !is_edf_file(files[i+1]) && files[i+1][0] != '-')
		{
			if(process_edf_file(files[i],files[i+1]) == EDF_CANCEL)
			{
				printf("processing file %s cancelled\n", files[i]);
				return;
			}
			i++;
		}
		else
		{
			if(process_edf_file(files[i],NULL) == EDF_CANCEL)
			{
				printf("processing file %s cancelled\n", files[i]);
				return;
			}
		}
	}
}

/***********************************************************************
 *Function Name: main
 *Input: argc, argv - called by the system
 *Output: int
 *Purpose: main function of the application.
 ***********************************************************************/
//int main (int argc, char ** argv)
void mexFunction(int nlhs, mxArray * plhs[], int nrhs, const mxArray *prhs[])
{
	printf("\nEDF2ASC: EyeLink EDF file -> ASCII (text) file translator\n");
	printf("EDF2ASC version %s \n", edf_get_version());
	printf("(c)1995-2009 by SR Research, last modified %s\n\n", __DATE__);

//     int argc = nrhs;
//     
// 	if(argc < 2)
// 	{
// 
// 		print_usage(argv[0]); /* agrv[0] is always the called program name*/
// #ifdef 	__MWERKS__
// 		{
// 		    char buff[1024];
// 			if(fgets(buff,1024,stdin) !=NULL)
// 			{
// 				int i =0;
// 				char *pname  = argv[0];
// 	       		argc = counttoken(buff,' ') +2;
// 	       		argv = (char **)malloc(sizeof(char*)*argc);
// 	       		argv[0] = "edf2asc";
// 	       		i =0;
// 	       		if(argc >=2)
// 	       		{
// 	       			argv[++i]=strtok(buff," \n");
// 	       			while(i<argc && (argv[++i] = strtok(NULL," \n"))){}
// 	       			argc=i;
// 	       			process(argc,argv);
// 
// 	       		}
// 	       		free(argv);
// 			}
// 		}
// #endif
// 		return -1;
// 	}
// 	process(argc,argv);
// 	return -1;
}


/***********************************************************************
 *Function Name: rfind
 *Input: char *(string ), char (search character)
 *Output: pointer to beginning of string
 *Purpose: finds the first occurrence of a character in the given string
 *			from backwards
 ***********************************************************************/

char * rfind(char *instr, char c)
{
	int i =0;
	for(i = strlen(instr)-1; i >=0; i--)
	{
		if(instr[i] == c) /*find  the beginning of the extension*/
			return &instr[i];
	}
	return NULL;
}
/***********************************************************************
 *Function Name: open_output_file
 *Input: edf file name
 *Output: 0 means everything is just fine.
 *		  others means something went wrong
 *Purpose: opens an asc file
 ***********************************************************************/
int open_output_file(char *edffile)
{
	char *i =NULL;
	char * fn = NULL;

	if(!options.new_path)
	{
		fn = (char *)malloc(sizeof(char) * (strlen(edffile)+1));
		strcpy(fn, edffile);
	}
	else
	{
		char *efname = NULL;
		efname = rfind(edffile,'/');
		if(!efname)
			efname = rfind(edffile,'\\');
		if(efname)
		{
			fn = (char *)malloc(sizeof(char) * (strlen(options.new_path)+strlen(efname)+1));
			strcpy(fn,options.new_path);
			strcat(fn,efname);
		}
		else
		{
			fn = (char *)malloc(sizeof(char) * (strlen(options.new_path)+strlen(edffile)+1));
			strcpy(fn,options.new_path);
			strcat(fn,edffile);
		}
	}

	i = rfind(fn,'.'); /*find  the beginning of the extension*/

	strcpy(i+1, "asc"); /* append the extension */
#ifndef NO_FILEEXIST_CHECK
	if(options.overwrite_asc_ifexists ==0 && fopen(fn,"r")) /* check for file existence */
	{
		static char reply ='N';
		if(reply != 'A')
		{
			char r[10];
			printf("%s already exists. Do you want to overwrite ? [Y/N/A]",fn);
			fgets(r,10,stdin);
			reply = r[0];
			reply = toupper(reply);
			if(!(reply =='A' || reply =='Y'))
				return 1;
		}
	}
#endif
	options.outfile = fopen(fn,"w");
	if(options.outfile)
	{
		//add utf8 bom (EF BB BF) at the beginning of an asc file
		if(options.Utf8BOM)
		{
			unsigned char UTF8_BOM[3]={(unsigned char)0xEF,(unsigned char)0xBB, (unsigned char)0xBF };
			int num = fwrite(UTF8_BOM, 1, sizeof(UTF8_BOM), options.outfile);
		}
		free(fn);
		return 0;
	}
	else
	{
		printf("Could not open output file %s \n", fn);
		free(fn);
		return 1;
	}


}

/***********************************************************************
 *Function Name: close_output_file
 *Input: none
 *Output: none
 *Purpose: closes an asc file
 ***********************************************************************/
void close_output_file()
{
	if(options.outfile)
	{
		fclose(options.outfile);
		options.outfile =  NULL;
	}

}

/***********************************************************************
 *Function Name: print_box
 *Input: any text
 *Output: none
 *Purpose: prints the given text sandwiched by =========
 ***********************************************************************/
#define BOXWIDTH 79
#define BOXWIDTHS "77"
void print_box(char * text)
{
	int i =0;
	int len =0;
	int left = 0;


	len = strlen(text);
	if(len >BOXWIDTH)
		len = BOXWIDTH;

	left = (BOXWIDTH-len)/2;

	for(i =0; i < left; i++)
		printf("=");

	printf("%s", text);
	for(i =(left+len); i< BOXWIDTH; i++)
		printf("=");

	printf ("\n");

}

/***********************************************************************
 *Function Name: print_percentage
 *Input: maxElements, currentElements
 *Output: none
 *Purpose: prints the percentage completion
 ***********************************************************************/
int print_percentage(int maxElements, int currentElements)
{
	static int percent = -1;
	if(maxElements && percent != (currentElements*100)/maxElements)
	{
		int i =0;
		char prog[20]={0};
		percent = (currentElements*100)/maxElements;
		for(i =0; i <percent/10; i++)
			prog[i] ='*';
		prog[i] =0;
		printf("Processed [%-10s]%d%%\r",prog,percent);
	}
	return 0;
}
/***********************************************************************
 *Function Name: print_preamble_text
 *Input: edf file name, preamble text
 *Output: none
 *Purpose: prints the preamble_text.
 ***********************************************************************/
void print_preamble_text(char * edffile, char * text)
{
	char header[1024] = {0};
	int length = 0;
	int i =0;
	char * sub = text;

	sprintf(header,"Preamble of file %s",edffile);
	print_box(header);
	length = strlen(text);
	replace(text,'\n','\0');
	for(i =0; i < length;)
	{
		sub = text +i;
		printf("|%-" BOXWIDTHS "s|\n", (sub+2));
		i += strlen(sub)+1;
	}
	print_box("");
	printf("\n");
}



void add_extension(char *dest,char *src)
{
	if(strlen(src) <=3)
	{
		strcpy(dest, src);
		strcat(dest,".edf");
	}
	else if( src[strlen(src) -4] != '.')
	{
		strcpy(dest, src);
		strcat(dest,".edf");
	}
	else
		strcpy(dest, src);
}



/***********************************************************************
 *Function Name: process_edf_file
 *Input: edf file name, optional output filename
 *Output: 0 means everything is just fine.
 *		  others means something went wrong
 *		  if outfile is null then the input file name is used for the
 *		  asc file.
 *Purpose: processes the edf file
 ***********************************************************************/
int process_edf_file(char * file, char *outfile)
{
#ifdef WIN32
    char avifilename[30] = "";
#endif
	char edffile[512];
	char ascfile[512];
	EDFFILE * ed = NULL;
	int rv;
	INT64 last_sample_time = -1;
	int numberOfElements = 0;
	int currentElement = 0;
	int event_count=0;
	int sample_count =0;
	int trial_count = 0;
    unsigned int frametime = -1;
    int frameprinted = 0;
	int reenable_out_marker_fields = 0;

	ppd_x = 0;
	ppd_y = 0;
	ppd_count = 0; /*reset these */


	options.allow_raw = -1;
	add_extension(edffile,file);
	strcpy(ascfile,edffile);

	ed = edf_open_file(edffile, options.enable_consistency_check|(options.enable_failsafe?1<<2:0)|(options.disable_large_time_stamp_check?1<<3:0),
		(options.events_enabled
#ifdef WIN32
		|| options.gazeMap
#endif
		),
		(options.samples_enabled
#ifdef WIN32
		|| options.insertFrameNumber
#endif
		),
		&rv); /* open the edf file */

	/*
		if ed is not null then the file is opened properly
		otherwise the file is not openable.
	*/
	if(ed != NULL)
	{
		ALLF_DATA *fd = NULL;
		RECORDINGS *rec = NULL;
		RECORDINGS recrd;
		int preambleTextLength;
		time_t start, end;
		start = time(NULL);



		if(options.outfile != NULL) /* close if an output file is already open. */
		{
			fclose(options.outfile);
			options.outfile = NULL;
		}
		if(outfile)
		{
			add_extension(ascfile,outfile);
		}
		if(open_output_file(ascfile)) /* open the output file */
		{
			return EDF_ERROR;
		}
		/*
			Get the preamble text and print it.
		*/
		preambleTextLength = edf_get_preamble_text_length(ed);
		if(preambleTextLength > 0)
		{
			char *preambleText;
			preambleText = (char *)malloc((preambleTextLength+1)* sizeof(char));
			edf_get_preamble_text(ed,preambleText,preambleTextLength+1);
			if(options.events_enabled)
			{
				time_t t;
				time(&t);
				print("** CONVERTED FROM %s using edfapi %s on %s", edffile,
					edf_get_version(), ctime(&t));
				print("%s**\n\n", preambleText);
			}
			print_preamble_text(edffile,preambleText);
			free(preambleText);
		}



		numberOfElements = edf_get_element_count(ed);

		if(edf_get_eyelink_revision(ed)==3 && options.out_marker_fields)
		{
			reenable_out_marker_fields =1;
			options.out_marker_fields = 0;
			printf("Warning: EyelinkCL data file. No marker fields available. \n");
		}
		/*
			Now we print all the data.
		*/
		while(1)
		{
			int type = edf_get_next_data(ed); /*findout what type of data it is*/
			currentElement++;
			if(progress_update(numberOfElements, currentElement))
				return EDF_CANCEL;
			switch(type)
			{
	            case STARTBLINK:
				case STARTSACC:
				case STARTFIX:
				case STARTPARSE:

				case ENDBLINK:
				case ENDSACC:
				case ENDFIX:
				case ENDPARSE:

				case FIXUPDATE:
				case BREAKPARSE:
				case BUTTONEVENT:
				case INPUTEVENT:
				case MESSAGEEVENT:
				{
					fd = edf_get_float_data(ed); /* get the event data */
                    if(type == MESSAGEEVENT)
                    {
#ifdef WIN32
                        if(strstr(&(fd->fe.message->c),"DISPLAY_COORDS"))
                        {
                            parseDisplayCoords(&(fd->fe.message->c));
                        }
                        else if(strstr(&(fd->fe.message->c),"DISPLAY_AREA_COORDS"))
                        {
                            parseDisplayAreaCoords(&(fd->fe.message->c));
                        }
#endif
                    }
					if(print_event((FEVENT *)fd, rec? (int)(1000.0/rec->sample_rate):0) == 0)
						event_count ++;
				}
				break;

				case STARTSAMPLES:
				case STARTEVENTS:
				case ENDSAMPLES:
				case ENDEVENTS:

				{
					fd = edf_get_float_data(ed); /* get the event data */
					print_event((FEVENT *)fd, 0);
				}
				break;
			case SAMPLE_TYPE:
				{

					fd = edf_get_float_data(ed); /* get the sample data */
#ifdef WIN32
                    if(options.insertFrameNumber)
                    {
                        if(frametime != -1 && last_sample_time != -1 && frametime>=last_sample_time/2 && fd->fs.time >= frametime && !frameprinted)
                        {
                            int frameNumber = -1;
                            FEVENT event;
                            char msg[500];
                            event.type = MESSAGEEVENT;
                            event.sttime = event.entime = event.time = frametime;


                            frameNumber = getLastFrameNumber();
                            sprintf(&(((LSTRING*)msg)->c), "VFRAME %d %s%d.avi",frameNumber, avifilename,trial_count/2);
                            ((LSTRING*)msg)->len = strlen(&(((LSTRING*)msg)->c))+1;

                            event.message = ((LSTRING*)msg);
                            print_event(&event, 0);
                            frameprinted = 1;
                        }

                        if(frametime == -1 || fd->fs.time > frametime)
                        {
                            frametime = getNextFrameTime();
                            frameprinted =0;
                        }
                    }
#endif

					if(last_sample_time < 0)
					{
					  last_sample_time =(INT64) (FLOAT_TIME(&(fd->fs))*2);
					}
					else if(rec)
					{
					  int smprt = (int)(2*1000.0/rec->sample_rate);

					  if(FLOAT_TIME(&(fd->fs))*2 > (last_sample_time +smprt))
						  printf("missing sample at %f \n", (last_sample_time +smprt)/2.0);
					  last_sample_time =(INT64) (FLOAT_TIME(&(fd->fs))*2);
					}
					if(write_sample(ed, rec, (FSAMPLE *)fd) == 0)
						sample_count ++;
				}
				break;
			case RECORDING_INFO:
				{
					options.allow_raw = -1;
                    frametime = -1;
					trial_count ++;
                    frameprinted = 0;
					last_sample_time = -1;
					fd = edf_get_float_data(ed); /* get the recording information data */
					if(fd->rec.state == 0)
						rec = NULL;
					else
					{
						memcpy(&recrd,(RECORDINGS*)fd, sizeof(recrd));
						rec = &recrd;

						if(edf_get_eyelink_revision(ed)==3)
						{
							if(rec->sflags&SAMPLE_HEADPOS)
								options.out_marker_fields = 2;
							else
								options.out_marker_fields = 0;
						}

						#ifdef WIN32
                        if(options.insertFrameNumber)
                        {
                            // load the lookup table
                            // if failed to load the lookup table try to create avi2ett
                            //
                            if(openLookupTable(edffile,trial_count/2,1)<0)
                                printf("could not open or create lookup table file. The avi files may not be found or renamed or DirectX 9 is not installed!\n");
                            else
                            {
                                int x =0;
                                for(x =strlen(file)-1; x>= 0 && !(file[x] == '/' || file[x] == '\\'); x--);

                                x = (x<0)? 0:x;
                                strcpy(avifilename,&file[x]);
                                avifilename[strlen(avifilename)-4] = 0;
                            }

                        }
                        #endif
					}
					print_recording(ed,(RECORDINGS*)fd);
				}
				break;
			case NO_PENDING_ITEMS:
				edf_close_file(ed);
				close_output_file();
				printf("Converted successfully: %d events, %d samples, %d blocks.\n", event_count, sample_count, trial_count/2);
				end = time(NULL);
				if(reenable_out_marker_fields)
					options.out_marker_fields=1;
				#ifdef WIN32
                	closeLookupTable();
                #endif
				/*printf("####Duration to write %d seconds \n", end-start); */
				return 0; /* no more data */
				break;
			default:
				assert(0); /* we should not get here */
			}
		}
		return EDF_OK;
	}
	else
	{
		printf("Could not process %s \n", edffile);
		return EDF_ERROR;
	}
}


/***********************************************************************
 *Function Name: print_usage
 *Input: program name
 *Output: None
 *Purpose: prints the usage of edf2asc
 ***********************************************************************/
void print_usage(char * program)
{
      printf("USAGE: %s  [options] <input .edf file>\n", program);
      printf("OPTIONS: -p <path> writes output with same name to <path> directory\n");
      printf("         -p *.<ext> writes output of same name with new extension\n");
      printf("         -d <filename> creates log data file\n");
      printf("         -t use only tabs as delimiters\n");
	  printf("         -c check consistency\n");
	  printf("         -z disable check consistency and fix the errors\n");
	  printf("		   -v verbose - reports warning messages. \n");
	  printf("		   -y overwrite asc file if exists.\n");
      printf("If no output file name, will match wildcards on input file name,\n");
      printf("and will write output files to new path or will overwrite old files.\n");
      printf("DATA OPTIONS: -sp  outputs sample raw pupil position if present\n");
      printf("              -sh  outputs sample HREF angle data if present\n");
      printf("              -sg  outputs sample GAZE data if present (default)\n");
      printf("      -l or -nr   outputs left-eye data only if binocular data file\n");
      printf("      -r or -nl   outputs right-eye data only if binocular data file\n");
      printf("      -res         outputs resolution data if present\n");
      printf("      -vel (-fvel) outputs sample velocity (-fvel matches EDFVIEW numbers)\n");
      printf("      -s or -ne   outputs sample data only\n");
      printf("      -e or -ns   outputs event data only\n");
      printf("      -miss <value>     replaces missing (x,y) in samples with <value>\n");
      printf("      -setres <xr> <yr> uses a fixed <xr>,<yr> resolution always\n");
      printf("      -defres <xr> <yr> uses a default <xr>,<yr> resolution if none in file\n");
	  printf("      -nv         hide viewer commands\n");
      printf("      -nst        blocks output of start events\n");
      printf("      -nmsg       blocks message event output\n");
      printf("      -neye       outputs only non-eye events (for sample-only files)\n");
      printf(" Use  -neye     to get samples labeled with non-eye events only\n");
      printf(" Use  -neye -ns to get non-eye events only\n");
  	  printf(" Use  -utf8/-UTF8 to force the output file to be opened using utf-8 encoding form.\n");
	  printf("-nflags to disable flags data for EyeLink II or EyeLink1000 data files.\n");
      printf("-hpos  output head marker positions\n");
      printf("-avg  output average data \n");
	  printf("-ftime output float time \n");
	  printf("-input output input values in samples.\n");
	  printf("-buttons output buttons values in samples.\n");
	  printf("-failsafe runs in failsafe mode and recover partial edf file \n");
	  printf("-ntarget to disable target data for EyeLink1000 Remote data files.\n");
	  printf("-ntime_check to disable large time stamp check.\n");


#ifdef WIN32
      printf("\n\nScene camera parameters \n");
      printf("-gazemap  outputs gaze data in avi coordinates. -g  can also be used instead\n");
      printf("-insertframe  inserts frame number. -i can also be used instead\n");
      printf("-scenecam  same as using -gazemap -insertframe together\n");

#endif
}




/***********************************************************************
 *Function Name: parse_options
 *Input: argc, argv - argument count and arguments return values
 *		 non parsed parameters and the count.
 *Output: int
 *Purpose: parse the selected options. returns 1 if everything goes well
 *			0 otherwise.
 ***********************************************************************/
int parse_options(int argc, char ** argv, int *nooptc, char ***nooptv)
{
	int i =0;
	char **retargv = NULL;
	int retargc = 0;
	retargv = (char **) malloc(sizeof(char *) * argc);
	for(i =0; i < argc; i++)
		retargv[i] = NULL;

	for(i=1;i<argc;i++)
	{
		if( argv[i][0]=='-')
		{
			switch(toupper(argv[i][1]))
			{
				case 'U':
					if(!stricmp(argv[i],"-UTF8"))  // force to add utf8 bom
					{
						options.Utf8BOM = 1;
					}
					break;
                case 'A':
                    options.out_averages=1;
                    break;
				case 'C':
					switch(toupper(argv[i][2]))
					{
					case 'L':
						options.output_elcl = 1;
						break;

					default:
						options.enable_consistency_check = 1;
						options.verbose = 1; /* report the errors.*/
					}
					break;
				case 'Z':
					options.enable_consistency_check = 0;
					break;

				case 'M':		/* missing data value */
				{
					char msw[50];
					/*float mv = strtod(argv[i+1],e);
					  if(!*e) missing_value = mv;*/
					strcpy(msw, argv[i+1]);
					msw[7] = 0;
					memset(missing_string, 32, 8);
					strcpy(missing_string+7-strlen(msw), msw);
					printf("Command line option: -miss, using missing value=%s\n", missing_string);
					i++;
				}
				break;

				case 'V':		/* sample velocity output*/
					{
						if(strlen(argv[i]) == 2)
							options.verbose = 1;
						else
							options.output_sample_velocity = 1;
					}
				break;

				case 'F':		/* sample velocity output */
					if(stricmp(argv[i],"-ftime")==0)
					{
						options.out_float_time = 1;
					}
					else if(stricmp(argv[i],"-failsafe")==0)
					{
						options.enable_failsafe= 1;
					}
					else
					{
						options.output_sample_velocity = 1;
						options.fast_velocity = 1;
					}
					break;

				case 'D':		/* set log file */
                    if(!stricmp(argv[i],"-DEFRES"))
						{
							float xr =(float) atof(argv[i+1]);
							float yr =(float) atof(argv[i+2]);
							i += 2;
							if(i>argc) break;
							if(xr<0.01 || xr>100000.0) break;
							if(yr<0.01 || yr>100000.0) break;
							options.default_resolution_x = xr;
							options.default_resolution_y = yr;
							if(!stricmp(argv[i],"SETRES")) options.force_resolution = 1;
								break;
						}

					if(options.logfile_name)
						free(options.logfile_name);
					options.logfile_name = (char *) malloc(sizeof(char) * (strlen(argv[i+1])+1));
					strcpy(options.logfile_name,argv[i+1]);
					printf("Command line option: -d, using log file %s\n", options.logfile_name);
					i++;
					break;
				case 'P':		/* set path */
					if(options.new_path)
						free(options.new_path);
					if(i+1 < argc)
					{
						options.new_path = (char *)malloc(sizeof(char ) * (strlen(argv[i+1])+1));
						strcpy(options.new_path,argv[i+1]);
						replace(options.new_path,'\\','/');
						if('/' != options.new_path[strlen(options.new_path)]) strcat(options.new_path,"/");
						i++;
					}
					else
					{
						print_usage(argv[0]);
						return 0;
					}
					break;
				case 'T':		/* delimit with tabs */
					options.use_tabs = 1;
					break;

			    case 'L':		/* set left-only */
					if(options.output_left_eye) options.output_right_eye = 0;
					else options.output_left_eye = options.output_right_eye = 1;
					break;

				case 'R':		/* set right-only */
					if(toupper(argv[i][2])=='E' && toupper(argv[i][3])=='S')
					{
						options.output_resolution = 1;
						break;
					}

					if(options.output_right_eye) options.output_left_eye = 0;
					else options.output_left_eye = options.output_right_eye = 1;
					break;
				case 'X':		/* enable resolution data */
					options.output_resolution = 1;
					break;
				case 'B':
					{
						options.output_button_values = 1;
					}
					break;
				case 'I':
					{
						if(!stricmp(argv[i],"-INPUT"))
						{
							options.output_input_values = 1;
						}
						else
						{
							#ifdef WIN32
							options.insertFrameNumber = 1;
							#endif
						}
						break;
					}
				#ifdef WIN32
                case 'G':
                    options.gazeMap =1;
                    break;
                #endif

				case 'S':
				#ifdef WIN32
                    if(!stricmp(argv[i],"-SCENECAM"))
                    {
                        options.gazeMap = 1;
                        options.insertFrameNumber = 1;
                        break;
                    }
					else
				#endif
					if(!stricmp(argv[i],"-SETRES"))/* samples only */
						{
							float xr =(float) atof(argv[i+1]);
							float yr =(float) atof(argv[i+2]);
							i += 2;
							if(i>argc) break;
							if(xr<0.01 || xr>100000.0) break;
							if(yr<0.01 || yr>100000.0) break;
							options.default_resolution_x = xr;
							options.default_resolution_y = yr;
							if(!stricmp(argv[i],"SETRES")) options.force_resolution = 1;
								break;
						}
					if(argv[i][2])
					{
						switch(toupper(argv[i][2]))
						{
							case 'G':
								options.preferred_sample_type = OUTPUT_GAZE;
								break;
							case 'H':
								options.preferred_sample_type = OUTPUT_HREF;
								break;
							case 'P':
								options.preferred_sample_type = OUTPUT_PUPIL;
								break;
						}
						break;
					}

					if(options.samples_enabled) options.events_enabled = 0;
					else options.samples_enabled = options.events_enabled = 1;
					break;
                case 'H':		/* head marker position */
					options.out_marker_fields = 1;
					break;
				case 'Y':
					options.overwrite_asc_ifexists = 1;
					break;
				case 'E':		/* events only */
					if(options.events_enabled) options.samples_enabled = 0;
					else options.samples_enabled = options.events_enabled = 1;
					break;
				case 'N':
					switch(toupper(argv[i][2]))
					{
						case 'M':
							options.msg_events_enabled = 0;
							break;
						case 'L':
							options.output_left_eye = 0;
							break;
						case 'R':
							options.output_right_eye = 0;
							break;
						case 'F':
							options.out_sample_flags = 0;
							break;
						case 'V':
							options.hide_viewer_commands =1;
							break;
						case 'E':
							if(toupper(argv[i][3])=='Y' && toupper(argv[i][4])=='E')
							{
								options.eye_events_enabled = 0;
								options.samples_enabled = 0;
							}
							else
								options.events_enabled = 0;
							break;
						case 'S':
							if(toupper(argv[i][3])=='T')
								options.start_events_enabled = 0;
							else
								options.samples_enabled = 0;
							break;
						case 'T':
							{
								if(toupper(argv[i][3])=='I')
								{
									options.disable_large_time_stamp_check =1;
									break;
								}
								else
								{
									options.enable_htarget = 0;
									break;
								}
							}
							break;

					}
			}
		}
		else
		{
			retargv[retargc++] = argv[i];
		}
    }
	*nooptc = retargc;
 	*nooptv = retargv;
	return 1;
}



/***********************************************************************
 *Function Name: print
 *Input: format and arguments(format is the same as in printf )
 *Output: Nothing
 *Purpose: prints the given text to output file if it is present.
 *		   if it does not exist the data is printed to the standard out.
 ***********************************************************************/
void print(const char * fmt, ...)
{
   char lpbuffer[1024];
   va_list argptr;
   va_start(argptr, fmt);
   vsprintf(lpbuffer, fmt, argptr);
   va_end(argptr);

   if(options.outfile)
       fprintf(options.outfile, "%s", lpbuffer);
   else
       fprintf(stdout, "%s", lpbuffer);
}
