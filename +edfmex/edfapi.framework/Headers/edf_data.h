/**
 * Copyright (c) 1996-2023, SR Research Ltd., All Rights Reserved
 *
 * For use by SR Research licencees only. Redistribution and use in source
 * and binary forms, with or without modification, are NOT permitted.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the distribution.
 *
 * Neither name of SR Research Ltd nor the name of contributors may be used
 * to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* EYELINK PORTABLE EXPT SUPPORT            */

/*  EyeLink library data extensions         */
/*        VERSION 2.5                       */

/* UPDATED for EyeLink DLL V2.1: added LOST_DATA_EVENT */

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



/*!
    \file edf_data.h
 */

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
#pragma pack(push, 1)
#endif

///@cond TEST
typedef struct {
                INT16 len;
                char c;
               } LSTRING;
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(pop)
#endif
#endif
///@endcond

#ifndef FSAMPLEDEF	/* FLOATING-POINT SAMPLE */
#define FSAMPLEDEF 1    /* gaze, resolution prescaling removed */
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(push, 1)
#endif


/*!\page classIntro
 * The EyeLink EDF access API (edfapi.dll) library defines a number of data types that are 
 * used for data reading, found in eye_data.h and edf.h. The useful parts of these structures 
 * are discussed in the following sections. */



/*! The FSAMPLE structure holds information for a sample in the EDF file. 
 * Depending on the recording options set for the recording session, 
 * some of the fields may be empty.*/

typedef struct {
		 UINT32 time;   /*!< time stamp of sample */
		 /*INT16  type; */	/* always SAMPLE_TYPE */ 
		 

		 float  px[2];   /*!< pupil x */
		 float  py[2];   /*!< pupil y */
		 float  hx[2];   /*!< headref x */
		 float  hy[2];   /*!< headref y */
		 float  pa[2]; 	 /*!< pupil size or area */

		 float gx[2];    /*!< screen gaze x */
		 float gy[2];    /*!< screen gaze y */
		 float rx;       /*!< screen pixels per degree */
		 float ry;       /*!< screen pixels per degree */

		 
		 float gxvel[2];  /*!< gaze x velocity */
		 float gyvel[2];  /*!< gaze y velocity */
		 float hxvel[2];  /*!< headref x velocity */
		 float hyvel[2];  /*!< headref y velocity */
		 float rxvel[2];  /*!< raw x velocity */
		 float ryvel[2];  /*!< raw y velocity */

		 float fgxvel[2]; /*!< fast gaze x velocity */
		 float fgyvel[2]; /*!< fast gaze y velocity */
		 float fhxvel[2]; /*!< fast headref x velocity */
		 float fhyvel[2]; /*!< fast headref y velocity */
		 float frxvel[2]; /*!< fast raw x velocity */
		 float fryvel[2]; /*!< fast raw y velocity */

		 INT16  hdata[8];  /*!< head-tracker data (not pre-scaled) */
		 UINT16 flags;     /*!<  flags to indicate contents */
		 
		 //UINT16 status;       /* tracker status flags    */ 
		 UINT16 input;     /*!< extra (input word) */
		 UINT16 buttons;   /*!< button state & changes */

		 INT16  htype;     /*!< head-tracker data type (0=none) */
		 
		 UINT16 errors;    /*!< process error flags */

} FSAMPLE;
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(pop)
#endif

#endif




			/* FLOATING-POINT EYE EVENT */
#ifndef FEVENTDEF       /* gaze, resolution, velocity prescaling removed */
#define FEVENTDEF 1
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(push, 1)
#endif

/*!The FEVENT structure holds information for an event in the EDF file. 
 * Depending on the recording options set for the recording session and the event type, 
 * some of the fields may be empty.*/

typedef struct  {
		UINT32 time;       /*!< effective time of event */
		INT16  type;       /*!< event type */
		UINT16 read;       /*!< flags which items were included */


		UINT32 sttime;   /*!< start time of the event */
		UINT32 entime;   /*!< end time of the event*/
		float  hstx;     /*!< headref starting points */ 
		float  hsty;     /*!< headref starting points */
		float  gstx;     /*!< gaze starting points */
		float  gsty;     /*!< gaze starting points */
		float  sta;      /*!< pupil size at start */
		float  henx;     /*!<  headref ending points */
		float  heny;     /*!<  headref ending points */
		float  genx;     /*!< gaze ending points */
		float  geny;     /*!< gaze ending points */
		float  ena;       /*!< pupil size at end */
		float  havx;     /*!< headref averages */
		float  havy;     /*!< headref averages */
		float  gavx;     /*!< gaze averages */
		float  gavy;     /*!< gaze averages */
		float  ava;       /*!< average pupil size */
		float  avel;     /*!< accumulated average velocity */
		float  pvel;     /*!< accumulated peak velocity */
		float  svel;     /*!< start velocity */
		float evel;      /*!< end velocity */
		float  supd_x;   /*!< start units-per-degree */
		float  eupd_x;   /*!< end units-per-degree */
		float  supd_y;   /*!< start units-per-degree */
		float  eupd_y;   /*!< end units-per-degree */

		INT16  eye;        /*!< eye: 0=left,1=right */
		UINT16 status;           /*!< error, warning flags */
		UINT16 flags;           /*!< error, warning flags*/
		UINT16 input;
		UINT16 buttons;
		UINT16 parsedby;       /*!< 7 bits of flags: PARSEDBY codes*/
		LSTRING *message;		/*!< any message string*/
		} FEVENT;
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(pop)
#endif
#endif

#ifndef CONFIGDEF
#define CONFIGDEF
/*!The CONFIG structure holds optional configuration information for EDF file processing. 
 * User configured parameters or application specif parameters can be passed to edfapi 
 * via this configuration.*/

typedef struct  {
		float  default_rx;     /*!< user defined resolution rx value */
		float  default_ry;     /*!< user defined resolution ry value */
		} CONFIG;
#endif

			/* message events: usually text */
			/* but may contain binary data  */
#ifndef IMESSAGEDEF
#define IMESSAGEDEF
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(push, 1)
#endif

 /// @cond REMOVE
typedef struct  {
		UINT32 time;       /* time message logged */
		INT16  type;       /* event type: usually MESSAGEEVENT */

		UINT16 length;	   /* length of message */
		byte   text[260];  /* message contents (max length 255) */
		} IMESSAGE;
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(pop)
#endif

#endif
 /// @endcond
			/* button, input, other simple events */
#ifndef IOEVENTDEF
#define IOEVENTDEF
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(push, 1)
#endif


/// @cond TEST
typedef struct  {
		UINT32 time;       /* time logged */
		INT16  type;       /* event type: */

		UINT16 data;	   /* coded event data */
		} IOEVENT;
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(pop)
#endif

#endif
/// @endcond

#ifndef __RECORDINGS___
#define __RECORDINGS___
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(push, 1)
#endif

/*! The RECORDINGS structure holds information about a recording block in an EDF file. 
 * A RECORDINGS structure is present at the start of recording and the end of recording. 
 * Conceptually a RECORDINGS structure is similar to the START and END lines inserted in an EyeLink ASC file. 
 * RECORDINGS with a state field = 0 represent the end of a recording block, and contain information regarding 
 * the recording options set before recording was initiated. */


typedef struct
{
	UINT32 time;		/*!< start time or end time*/
	float sample_rate;  /*!< 250 or 500 or 1000*/
	UINT16 eflags;      /*!< to hold extra information about events */
	UINT16 sflags;      /*!< to hold extra information about samples */
	byte state;			/*!< 0 = END, 1=START */
	byte record_type;	/*!< 1 = SAMPLES, 2= EVENTS, 3= SAMPLES and EVENTS*/
	byte pupil_type;	/*!< 0 = AREA, 1 = DIAMETER*/
	byte recording_mode;/*!< 0 = PUPIL, 1 = CR */
	byte filter_type;   /*!< 1,2,3 */	
	byte  pos_type;		/*!<0 = GAZE, 1= HREF, 2 = RAW*/  /*PARSEDBY_GAZE  PARSEDBY_HREF PARSEDBY_PUPIL*/
	byte eye;			/*!< 1=LEFT, 2=RIGHT, 3=LEFT and RIGHT */


}RECORDINGS;
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(pop)
#endif

#endif



			/* FLOATING POINT TYPES for eye event, sample */
#ifndef EDFDATADEF
#define EDFDATADEF
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(push, 1)
#endif

/*!Any one of the above three data types can be read into a buffer of type ALLF_DATA, which is 
 * a union of the event, sample, and recording buffer formats:*/

typedef union {
		FEVENT    fe;
		IMESSAGE  im;
		IOEVENT   io;
		FSAMPLE   fs;
		RECORDINGS rec;
	      } ALLF_DATA ;
#if defined(_M_X64) || defined(__x86_64__) // pack to 1 byte otherwise crashes on matlab
#pragma pack(pop)
#endif

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



#define EDF_NO_CHECK_CONSISTENCY 0x0 
#define EDF_CHECK_CONSISTENCY 0x1 
#define EDF_CHECKFIX_CONSISTENCY 0x3 


#define EDF_NO_EVENTS 0x0 
#define EDF_NO_SAMPLES 0x0 
#define EDF_LOAD_EVENTS 0x1 
#define EDF_EXTRA_MSG   0x2
#define EDF_LOAD_EXTRA_MSG (EDF_EXTRA_MSG|EDF_LOAD_EVENTS) 
#define EDF_LOAD_SAMPLE 0x1 



#ifdef __cplusplus	/* For C++ compilation */
}
#endif

#endif /* SIMTYPESINCL */
