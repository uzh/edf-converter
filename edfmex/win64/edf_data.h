/* EYELINK PORTABLE EXPT SUPPORT            */
/* Copyright (c) 1996-2002 SR Research Ltd. */
/*     8 June '97 by Dave Stampe            */
/*     For non-commercial use only          */
/*				            */
/*  EyeLink library data extensions         */
/*        VERSION 2.5                       */

/* UPDATED for EyeLink DLL V2.1: added LOST_DATA_EVENT */

/* This module is for user applications   */
/* Use is granted for non-commercial      */
/* applications by Eyelink licencees only */

/************** WARNING **************/
/*                                   */
/* UNDER NO CIRCUMSTANCES SHOULD     */
/* PARTS OF THESE FILES BE COPIED OR */
/* COMBINED.  This will make your    */
/* code impossible to upgrade to new */
/* releases in the future, and       */
/* SR Research will not give tech    */
/* support for reorganized code.     */
/*                                   */
/* This file should not be modified. */
/* If you must modify it, copy the   */
/* entire file with a new name, and  */
/* change the new file.          */
/*                                   */
/************** WARNING **************/

/* EDFTYPES.H has platform-portable definitions  */

/* COPYRIGHT TERMS:  This file is part of the */
/* EyeLink application support package.  You  */
/* may compile this code, and may read it to  */
/* understand the EyeLink data structures.    */
/*   DO NOT MODIFY THIS CODE IN ANY WAY.      */
/* SR Research will not support any problems  */
/* caused by modifying this code, and using   */
/* modified code will make your programs      */
/* incompatible with future releases.         */
/*   This code has been tested on many 16 and */
/* 32-bit compilers, including Borland and    */
/* Microsoft, in Windows and DOS.  It should  */
/* compile without warnings on most platforms */
/* If you must port to a new platform, check  */
/* the data types defined in EDFTYPES.H for   */
/* correct sizes.			      */


#ifndef BYTEDEF
  #include "edftypes.h"
#endif

#ifndef SIMTYPESINCL
#define SIMTYPESINCL

#ifdef __cplusplus 	/* For C++ compilation */
extern "C" {
#endif

/*********** EYE DATA FORMATS **********/

/* ALL fields use MISSING_DATA when value was not read, */
/* EXCEPT for <buttons>, <time>, <sttime> and <entime>, which use 0. */
/* This is true for both floating point or integer variables. */

	/* Both samples and events may have             */
	/* several fields that have not been updated.   */
	/* These fields may be detected from the        */
	/* content flags, or by testing the field value */
	/* against these constants: */

#define MISSING_DATA -32768     /* data is missing (integer) */
#define MISSING -32768
#define INaN -32768


	/* binocular data needs to ID the eye for events */
	/* samples need to index the data */
	/* These constants are used as eye identifiers */

#define LEFT_EYE  0   /* index and ID of eyes */
#define RIGHT_EYE 1
#define LEFTEYEI  0
#define RIGHTEYEI 1
#define LEFT      0
#define RIGHT     1

#define BINOCULAR 2   /* data for both eyes available */


/********* EYE SAMPLE DATA FORMATS *******/

	/* The SAMPLE struct contains data from one 4-msec  */
	/* eye-tracker sample. The <flags> field has a bit for each */
	/* type of data in the sample. Fields not read have 0 flag */
	/* bits, and are set to MISSING_DATA */

/* flags to define what data is included in each sample.   */
/* There is one bit for each type.  Total data for samples */
/* in a block is indicated by these bits in the <sam_data> */
/* field of ILINKDATA or EDF_FILE, and is updated by the    */
/* STARTSAMPLES control event. */

#define SAMPLE_LEFT      0x8000  /* data for these eye(s) */
#define SAMPLE_RIGHT     0x4000

#define SAMPLE_TIMESTAMP 0x2000  /* always for link, used to compress files */

#define SAMPLE_PUPILXY   0x1000  /* pupil x,y pair */
#define SAMPLE_HREFXY    0x0800  /* head-referenced x,y pair */
#define SAMPLE_GAZEXY    0x0400  /* gaze x,y pair */
#define SAMPLE_GAZERES   0x0200  /* gaze res (x,y pixels per degree) pair */
#define SAMPLE_PUPILSIZE 0x0100  /* pupil size */
#define SAMPLE_STATUS    0x0080  /* error flags */
#define SAMPLE_INPUTS    0x0040  /* input data port */
#define SAMPLE_BUTTONS   0x0020  /* button state: LSBy state, MSBy changes */

#define SAMPLE_HEADPOS   0x0010  /* * head-position: byte tells # words */
#define SAMPLE_TAGGED    0x0008  /* * reserved variable-length tagged */
#define SAMPLE_UTAGGED   0x0004  /* * user-defineabe variable-length tagged */
#define SAMPLE_ADD_OFFSET 0x0002 /* if this flag is set for the sample add .5ms to the sample time */


#ifndef LSTRINGDEF      /* length-def'd data packet*/
#define LSTRINGDEF 1
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(1)
#endif
typedef struct {
                INT16 len;
                char c;
               } LSTRING;
#endif


#ifndef FSAMPLEDEF	/* FLOATING-POINT SAMPLE */
#define FSAMPLEDEF 1    /* gaze, resolution prescaling removed */
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(1)
#endif

typedef struct {
		 UINT32 time;   /* time of sample */
		 /*INT16  type; */	/* always SAMPLE_TYPE */

		 

		 float  px[2], py[2];    /* pupil xy */
		 float  hx[2], hy[2];    /* headref xy */
		 float  pa[2]; 		 /* pupil size or area */

		 float gx[2], gy[2];    /* screen gaze xy */
		 float rx, ry;          /* screen pixels per degree */

		 
		 float gxvel[2];
		 float gyvel[2];
		 float hxvel[2];
		 float hyvel[2];
		 float rxvel[2];
		 float ryvel[2];

		 float fgxvel[2];
		 float fgyvel[2];
		 float fhxvel[2];
		 float fhyvel[2];
		 float frxvel[2];
		 float fryvel[2];

		 INT16  hdata[8];  /* head-tracker data (not prescaled) */
		 UINT16 flags;  /* flags to indicate contents */
		 
		 //UINT16 status;       /* tracker status flags    */
		 UINT16 input;        /* extra (input word)      */
		 UINT16 buttons;      /* button state & changes  */

		 INT16  htype;     /* head-tracker data type (0=none)   */
		 
		 UINT16 errors;       /* process error flags */

} FSAMPLE;
#endif




			/* FLOATING-POINT EYE EVENT */
#ifndef FEVENTDEF       /* gaze, resolution, velocity prescaling removed */
#define FEVENTDEF 1
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(1)
#endif

typedef struct  {
		UINT32 time;       /* effective time of event */
		INT16  type;       /* event type */
		UINT16 read;       /* flags which items were included */


		UINT32 sttime, entime;   /* start, end times */
		float  hstx, hsty;       /* starting points */
		float  gstx, gsty;       /* starting points */
		float  sta;
		float  henx, heny;       /* ending points */
		float  genx, geny;       /* ending points */
		float  ena;
		float  havx, havy;       /* averages */
		float  gavx, gavy;       /* averages */
		float  ava;
		float  avel;             /* avg velocity accum */
		float  pvel;             /* peak velocity accum */
		float  svel, evel;       /* start, end velocity */
		float  supd_x, eupd_x;   /* start, end units-per-degree */
		float  supd_y, eupd_y;   /* start, end units-per-degree */

		INT16  eye;        /* eye: 0=left,1=right */
		UINT16 status;           /* error, warning flags */
		UINT16 flags;           /* error, warning flags*/
		UINT16 input;
		UINT16 buttons;
		UINT16 parsedby;       /* 7 bits of flags: PARSEDBY codes*/
		LSTRING *message;		/* any message string*/
		} FEVENT;
#endif


			/* message events: usually text */
			/* but may contain binary data  */
#ifndef IMESSAGEDEF
#define IMESSAGEDEF
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(1)
#endif

typedef struct  {
		UINT32 time;       /* time message logged */
		INT16  type;       /* event type: usually MESSAGEEVENT */

		UINT16 length;	   /* length of message */
		byte   text[260];  /* message contents (max length 255) */
		} IMESSAGE;
#endif

			/* button, input, other simple events */
#ifndef IOEVENTDEF
#define IOEVENTDEF
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(1)
#endif

typedef struct  {
		UINT32 time;       /* time logged */
		INT16  type;       /* event type: */

		UINT16 data;	   /* coded event data */
		} IOEVENT;
#endif

#ifndef __RECORDINGS___
#define __RECORDINGS___
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(1)
#endif

typedef struct
{
	UINT32 time;		/* start time or end time*/
	float sample_rate; /* 250 or 500 or 1000*/
	UINT16 eflags;       /* to hold extra information about events */
	UINT16 sflags;       /* to hold extra information about samples */
	byte state;			/* 0 = END, 1=START */
	byte record_type;	/* 1 = SAMPLES, 2= EVENTS, 3= SAMPLES and EVENTS*/
	byte pupil_type;	/* 0 = AREA, 1 = DIAMETER*/
	byte recording_mode;/* 0 = PUPIL, 1 = CR */
	byte filter_type;   /* 1,2,3 */	
	byte  pos_type;		/*PARSEDBY_GAZE  PARSEDBY_HREF PARSEDBY_PUPIL*/
	byte eye;			/* 1=LEFT, 2=RIGHT, 3=LEFT and RIGHT */


}RECORDINGS;
#endif



			/* FLOATING POINT TYPES for eye event, sample */
#ifndef EDFDATADEF
#define EDFDATADEF
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(1)
#endif

typedef union {
		FEVENT    fe;
		IMESSAGE  im;
		IOEVENT   io;
		FSAMPLE   fs;
		RECORDINGS rec;
	      } ALLF_DATA ;
#endif


/********** SAMPLE, EVENT BUFFER TYPE CODES ***********/

		/* the type code for samples */
#define SAMPLE_TYPE 200




/************* EVENT TYPE CODES ************/

	/* buffer = IEVENT, FEVENT, btype = IEVENT_BUFFER */

#define STARTPARSE 1 	/* these only have time and eye data */
#define ENDPARSE   2
#define BREAKPARSE 10

			/* EYE DATA: contents determined by evt_data */
#define STARTBLINK 3    /* and by "read" data item */
#define ENDBLINK   4    /* all use IEVENT format */
#define STARTSACC  5
#define ENDSACC    6
#define STARTFIX   7
#define ENDFIX     8
#define FIXUPDATE  9

  /* buffer = (none, directly affects state), btype = CONTROL_BUFFER */

			 /* control events: all put data into */
			 /* the EDF_FILE or ILINKDATA status  */
#define STARTSAMPLES 15  /* start of events in block */
#define ENDSAMPLES   16  /* end of samples in block */
#define STARTEVENTS  17  /* start of events in block */
#define ENDEVENTS    18  /* end of events in block */



	/* buffer = IMESSAGE, btype = IMESSAGE_BUFFER */

#define MESSAGEEVENT 24  /* user-definable text or data */


	/* buffer = IOEVENT, btype = IOEVENT_BUFFER */

#define BUTTONEVENT  25  /* button state change */
#define INPUTEVENT   28  /* change of input port */

#define LOST_DATA_EVENT 0x3F   /* NEW: Event flags gap in data stream */

/************* CONSTANTS FOR EVENTS ************/

	/* "read" flag contents in IEVENT */

				/* time data */
#define READ_ENDTIME 0x0040     /* end time (start time always read) */

			    /* non-position eye data: */
#define READ_GRES    0x0200     /* gaze resolution xy */
#define READ_SIZE    0x0080     /* pupil size */
#define READ_VEL     0x0100     /* velocity (avg, peak) */
#define READ_STATUS  0x2000     /* status (error word) */

#define READ_BEG     0x0001     /* event has start data for vel,size,gres */
#define READ_END     0x0002     /* event has end data for vel,size,gres */
#define READ_AVG     0x0004     /* event has avg pupil size, velocity */

			    /* position eye data */
#define READ_PUPILXY 0x0400    /* pupilxy REPLACES gaze, href data if read */
#define READ_HREFXY  0x0800
#define READ_GAZEXY  0x1000

#define READ_BEGPOS  0x0008    /* position data for these parts of event */
#define READ_ENDPOS  0x0010
#define READ_AVGPOS  0x0020


		       /* RAW FILE/LINK CODES: REVERSE IN R/W */
#define FRIGHTEYE_EVENTS  0x8000 /* has right eye events */
#define FLEFTEYE_EVENTS   0x4000 /* has left eye events */

	/* "event_types" flag in ILINKDATA or EDF_FILE */
	/* tells what types of events were written by tracker */

#define LEFTEYE_EVENTS   0x8000 /* has left eye events */
#define RIGHTEYE_EVENTS  0x4000 /* has right eye events */
#define BLINK_EVENTS     0x2000 /* has blink events */
#define FIXATION_EVENTS  0x1000 /* has fixation events */
#define FIXUPDATE_EVENTS 0x0800 /* has fixation updates */
#define SACCADE_EVENTS   0x0400 /* has saccade events */
#define MESSAGE_EVENTS   0x0200 /* has message events */
#define BUTTON_EVENTS    0x0040 /* has button events */
#define INPUT_EVENTS     0x0020 /* has input port events */


	/* "event_data" flags in ILINKDATA or EDF_FILE */
	/* tells what types of data were included in events by tracker */

#define EVENT_VELOCITY  0x8000  /* has velocity data */
#define EVENT_PUPILSIZE 0x4000  /* has pupil size data */
#define EVENT_GAZERES   0x2000  /* has gaze resolution */
#define EVENT_STATUS    0x1000  /* has status flags */

#define EVENT_GAZEXY    0x0400  /* has gaze xy position */
#define EVENT_HREFXY    0x0200  /* has head-ref xy position */
#define EVENT_PUPILXY   0x0100  /* has pupil xy position */

#define FIX_AVG_ONLY    0x0008  /* only avg. data to fixation evts */
#define START_TIME_ONLY 0x0004  /* only start-time in start events */

#define PARSEDBY_GAZE   0x00C0  /* how events were generated */
#define PARSEDBY_HREF   0x0080
#define PARSEDBY_PUPIL  0x0040


/************ STATUS FLAGS (samples and events) ****************/

#define LED_TOP_WARNING       0x0080    /* marker is in border of image*/
#define LED_BOT_WARNING       0x0040
#define LED_LEFT_WARNING      0x0020
#define LED_RIGHT_WARNING     0x0010
#define HEAD_POSITION_WARNING 0x00F0    /* head too far from calibr???*/

#define LED_EXTRA_WARNING     0x0008    /* glitch or extra markers */
#define LED_MISSING_WARNING   0x0004    /* <2 good data points in last 100 msec) */
#define HEAD_VELOCITY_WARNING 0x0001    /* head moving too fast */

#define CALIBRATION_AREA_WARNING 0x0002  /* pupil out of good mapping area */

#define MATH_ERROR_WARNING   0x2000  /* math error in proc. sample */

/* THESE CODES ONLY VSLID FOR EYELINK II */

            /* this sample interpolated to preserve sample rate
               usually because speed dropped due to missing pupil */
#define INTERP_SAMPLE_WARNING 0x1000

            /* pupil interpolated this sample
               usually means pupil loss or
               500 Hz sample with CR but no pupil
            */
#define INTERP_PUPIL_WARNING  0x8000

            /* all CR-related errors */
#define CR_WARNING       0x0F00
#define CR_LEFT_WARNING  0x0500
#define CR_RIGHT_WARNING 0x0A00

            /* CR is actually lost */
#define CR_LOST_WARNING        0x0300
#define CR_LOST_LEFT_WARNING   0x0100
#define CR_LOST_RIGHT_WARNING  0x0200

            /* this sample has interpolated/held CR */
#define CR_RECOV_WARNING       0x0C00
#define CR_RECOV_LEFT_WARNING  0x0400
#define CR_RECOV_RIGHT_WARNING 0x0800





#define TFLAG_MISSING   0x4000    /* missing */
#define TFLAG_ANGLE     0x2000    /* extreme target angle */
#define TFLAG_NEAREYE   0x1000    /* target near eye so windows overlapping */
/*  DISTANCE WARNINGS (limits set by remote_distance_warn_range command)*/
#define TFLAG_CLOSE     0x0800    /* distance vs. limits */
#define TFLAG_FAR       0x0400
/* TARGET TO CAMERA EDGE  (margin set by remote_edge_warn_pixels command)*/
#define TFLAG_T_TSIDE   0x0080    /* target near edge of image (left, right, top, bottom)*/
#define TFLAG_T_BSIDE   0x0040
#define TFLAG_T_LSIDE   0x0020
#define TFLAG_T_RSIDE   0x0010
/* EYE TO CAMERA EDGE  (margin set by remote_edge_warn_pixels command)*/
#define TFLAG_E_TSIDE   0x0008    /* eye near edge of image (left, right, top, bottom) */
#define TFLAG_E_BSIDE   0x0004
#define TFLAG_E_LSIDE   0x0002
#define TFLAG_E_RSIDE   0x0001


#ifdef __cplusplus	/* For C++ compilation */
}
#endif

#endif /* SIMTYPESINCL */
