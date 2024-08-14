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
/***********************************************************************
 *
 * EYELINK PORTABLE EXPT SUPPORT
 *
 * EyeLink library data extensions
 *        VERSION 2.5
 * UPDATED for EyeLink DLL V2.1: added LOST_DATA_EVENT
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
***********************************************************************/

/*!
    \file edf.h
 */

/*!
\mainpage EyeLink EDF Access API

\section intro Introduction

The EyeLink EDF Access API is a set of C functions that provide access to
EyeLink EDF files. The access method is similar to that of the online data
access API, where the program performs a set of eyelink_get_next_data() and
eyelink_get_float_data() calls to step through the data.

The EDF Access API also provides functions for setting bookmarks within an
EDF file, and for automatically parsing an EDF file into a set of trials,
with functions for stepping through the trial set.

As an example use for the API, the edf2asc translator program has been re-written
to use the API for EDF data access. The source code for this edf2asc program is
included with the API distribution.

This is the first release of the EDF Access API and should be considered a beta release.

Please report any functionality comments or bugs to support@sr-research.com.

*/



#ifndef __SR_RESEARCH_EDF_H__
#define __SR_RESEARCH_EDF_H__
#include "edf_data.h"


/***********************************************************************
 * additional defines - there are more defines defined in edf_data.h
 ***********************************************************************/
#define NO_PENDING_ITEMS 0
#define RECORDING_INFO 30 /* recording struct is returned */

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 * compile conditions for porting.
 ***********************************************************************/
#ifdef WIN32
	#define EXPORT __cdecl
#endif

#ifndef EXPORT
#define	EXPORT
#endif
#ifndef PUBLIC_FN
#define	PUBLIC_FN
#endif


typedef enum
{
	GAZE,
	HREF,
	RAW
}position_type;



#define PUPIL_ONLY_250 0
#define PUPIL_ONLY_500 1
#define PUPIL_CR	   2


/* @struct TRIAL
     The TRIAL structure is used to access a block of data within an EDF file
	 that is considered to be a trial within the experimental session. The start time
	and end time of a TRIAL are defined using the edf_set_trial_identifier() function,
	where a start and end message text token is specified.

   	@param rec  recording information about the current trial
	@param duration  duration of the current trial
	@param starttime start time of the trial
	@param endtime  end time of the trial
*/

/*!The TRIAL structure is used to access a block of data within an EDF file
  that is considered to be a trial within the experimental session. The start time
  and end time of a TRIAL are defined using the edf_set_trial_identifier() function,
  where a start and end message text token is specified.
*/
typedef struct {
	RECORDINGS *    rec;  /*!<recording information about the current trial*/
	unsigned int	duration;  /*!<duration of the current trial */
	unsigned int	starttime; /*!<start time of the trial */
	unsigned int	endtime;   /*!<end time of the trial */
} TRIAL;

/**@struct EDFFILE
	EDFFILE is a dummy structure that holds an EDF file handle.
	*/

typedef struct _EDFFILE EDFFILE;  /*!<EDFFILE is a dummy structure that holds an EDF file handle.*/


/* @struct BOOKMARK
    BOOKMARK is a dummy structure that holds a bookmark handle.
   	@param id

*/

/*!BOOKMARK is a dummy structure that holds a bookmark handle.
 */
typedef struct
{
	unsigned int id;
}BOOKMARK;


#define FLOAT_TIME(x) (((double)((x)->time)) + (((x)->flags & SAMPLE_ADD_OFFSET)?0.5:0.0))

/*=====================================================================*
 *																	   *
 *				GENERAL EDF DATA ACCESS FUNCTIONS					   *
 *																	   *
 *=====================================================================*/
/*!\defgroup GENERALEDFDATAAccess General EDF Data Access Functions
 *@{*/

/***********************************************************************
 *Function Name: edf_open_file
 *Input:		 file name, consistency, errval
 *Output: EDFFILE structure, pointer to int.
 *Purpose: opens the given edffile. If the return value is null then
 *		   the file could not be opened or the file is corrupt.
 *		   the errval is 0 if all the operation succeed. non zero
 *		   in other cases.
 ***********************************************************************/

/*!
   \brief Opens the EDF file passed in by edf_file_name and preprocesses the EDF file.
   @param fname name of the EDF file to be opened.
   @param consistency onsistency check control (for the time stamps of the start and end events, etc).
        \c 	0, no consistency check.
    	\c	1,  check consistency and report.
	    \c 	2,  check consistency and fix.
   @param loadevents load/skip loading events
		\c 0,  do not load events.
		\c 1,  load events.
   @param loadsamples load/skip loading of samples
		\c 0,  do not load samples.
		\c 1,  load samples.
   @param errval  This parameter is used for returning error value.
		The pointer should be a valid pointer to an integer. If the
		returned value is not \c 0 then an error occurred.

   @return if successful a pointer to EDFFILE structure is returned.
			Otherwise NULL is returned.
	*/

PUBLIC_FN EDFFILE * EXPORT edf_open_file(const char *fname, int consistency,
							   int loadevents, int loadsamples,
							   int *errval);



/***********************************************************************
*Function Name: edf_open_file
*Input:		 file name, consistency, errval
*Output: EDFFILE structure, pointer to int.
*Purpose: opens the given edffile. If the return value is null then
*		   the file could not be opened or the file is corrupt.
*		   the errval is 0 if all the operation succeed. non zero
*		   in other cases.
***********************************************************************/

/*!
\brief Opens the EDF file passed in by edf_file_name and preprocesses the EDF file.
@param fname name of the EDF file to be opened.
@param consistency onsistency check control (for the time stamps of the start and end events, etc).
\c 	0, no consistency check.
\c	1,  check consistency and report.
\c 	2,  check consistency and fix.
@param loadevents load/skip loading events
\c 0,  do not load events.
\c 1,  load events.
@param loadsamples load/skip loading of samples
\c 0,  do not load samples.
\c 1,  load samples.

@param config  Currently only used to set the
default resolution.  In the future, this will be used to pass more configuration parameters.
@param errval  This parameter is used for returning error value.
The pointer should be a valid pointer to an integer. If the
returned value is not \c 0 then an error occurred.

@return if successful a pointer to EDFFILE structure is returned.
Otherwise NULL is returned.
*/

PUBLIC_FN EDFFILE * EXPORT edf_open_file_ex(const char *fname, int consistency,
	int loadevents, int loadsamples, CONFIG *config,
	int *errval);

 /***********************************************************************
 *Function Name: edf_close_file
 *Input:		 file name
 *Output: int
 *Purpose: closes file, frees all memory
 *		   returns an error code. if the error code is 0 then the
 *		   operation succeeded otherwise a non zero is returned.
 ***********************************************************************/
/*!  \brief Closes an EDF file pointed to by the given EDFFILE pointer and releases all
  of the resources (memory and physical file) related to this EDF file.
  @param ef a valid pointer to EDFFILE structure. This should be created by calling
			edf_open_file ().
  @return  if successful it returns 0, otherwise a non zero is returned.*/



PUBLIC_FN int EXPORT edf_close_file(EDFFILE * ef);




PUBLIC_FN int EXPORT edf_open_logfile(EDFFILE * ef, const char* fname);
PUBLIC_FN int EXPORT edf_set_extra_message_file(const char* fname);

PUBLIC_FN const char* EXPORT edf_get_logmsg(EDFFILE * ef);
PUBLIC_FN //void EXPORT setEyeLogFileName(EDFFILE * ef, const char* fname);





/***********************************************************************
 *Function Name: edf_get_next_data
 *Input:		 edffile
 *Output:		 int
 *Purpose:		 returns the data type of the next data. Possible
 *				 values are
 *				 STARTBLINK
 *				 STARTSACC
 *				 STARTFIX
 *				 STARTSAMPLES
 *				 STARTEVENTS
 *				 STARTPARSE
 *
 *				 ENDBLINK
 *				 ENDSACC
 *				 ENDFIX
 *				 ENDSAMPLES
 *				 ENDEVENTS
 *				 ENDPARSE
 *
 *				 FIXUPDATE
 *				 BREAKPARSE
 *				 BUTTONEVENT
 *				 INPUTEVENT
 *				 MESSAGEEVENT
 *
 *				 SAMPLE_TYPE
 *				 RECORDING_INFO
 *
 *				 NO_PENDING_ITEMS
 ***********************************************************************/
/*!
    \brief Returns the type of the next data element in the EDF file pointed to by *edf. Each call to edf_get_next_data()
	will retrieve the next data element within the data file. The contents of the data element are not accessed using
	this method, only the type of the element is provided. Use edf_get_float_data() instead to access the contents of
	the data element.
	@param ef a valid pointer to EDFFILE structure. This handle should be created by calling edf_open_file().
	@return One of the following values:\n
			\c STARTBLINK   	 the upcoming data is a start blink event.\n
			\c STARTSACC		 the upcoming data is a start saccade event.\n
			\c STARTFIX 		 the upcoming data is a start fixation event.\n
			\c STARTSAMPLES 	 the upcoming data is a start samples event.\n
			\c STARTEVENTS 	 the upcoming data is a start events event.\n
			\c STARTPARSE	     the upcoming data is a start parse event.\n
			\c ENDBLINK		 the upcoming data is an end blink event.\n
			\c ENDSACC		     the upcoming data is an end saccade event.\n
			\c ENDFIX		     the upcoming data is an end fixation event.\n
			\c ENDSAMPLES	     the upcoming data is an end samples event.\n
			\c ENDEVENTS		 the upcoming data is an end events event.\n
			\c ENDPARSE		 the upcoming data is an end parse event.\n
			\c FIXUPDATE		 the upcoming data is a fixation update event.\n
			\c BREAKPARSE	     the upcoming data is a break parse event.\n
			\c BUTTONEVENT	     the upcoming data is a button event.\n
			\c INPUTEVENT	     the upcoming data is an input event.\n
			\c MESSAGEEVENT	 the upcoming data is a message event.\n
			\c SAMPLE_TYPE 	 the upcoming data is a sample.\n
			\c RECORDING_INFO 	 the upcoming data is a recording info.\n
			\c NO_PENDING_ITEMS no more data left.*/
PUBLIC_FN int EXPORT edf_get_next_data(EDFFILE *ef);


/***********************************************************************
 *Function Name: edf_get_float_data
 *Input:		 edffile
 *Output: int
 *Purpose: returns the float data with the type returned by
 *			edf_get_next_data
 ***********************************************************************/

/*!
   Returns the float data with the type returned by edf_get_next_data().
   This function does not move the current data access pointer to the next element;
   use edf_get_next_data() instead to step through the data elements.
   @param ef a valid pointer to \c EDFFILE structure. This handle should be created by
			 calling edf_open_file().
   @return Returns a pointer to the \c ALLF_DATA structure with the type returned by
		   edf_get_next_data().*/

PUBLIC_FN ALLF_DATA * EXPORT edf_get_float_data(EDFFILE *ef);
/**@}*/


/***********************************************************************
 *Function Name: edf_get_sample_close_to_time
 *Input:		 edffile
 *Output: int
 *Purpose: returns the float sample close to the time
 ***********************************************************************/
PUBLIC_FN ALLF_DATA* EXPORT  edf_get_sample_close_to_time(EDFFILE *ef, unsigned int time);

/***********************************************************************
 *Function Name: edf_get_element_count
 *Input:		 edffile
 *Output: int
 *Purpose: returns the number of elements in the edf file.
 ***********************************************************************/
/*!\ingroup GENERALEDFDATAAccess
    Returns the number of elements (samples, eye events, messages, buttons, etc) in the EDF file.
	@param ef a valid pointer to \c EDFFILE structure. This should be created by calling edf_open_file.
	@return  the number of elements in the \c EDF file.*/
PUBLIC_FN unsigned int EXPORT  edf_get_element_count(EDFFILE *ef);

/***********************************************************************
 *Function Name: edf_get_preamble_text
 *Input:		 edffile, pointer to char buffer, length of the buffer
 *Output: int
 *Purpose: copies the preamble text into the buffer.
 *		   if the preamble text is longer than the  length the
 *			text will be truncated.
 *			The returned content will always be null terminated
 ***********************************************************************/
/*!\ingroup GENERALEDFDATAAccess
    Copies the preamble text into the given buffer. If the preamble text is
	longer than the length the text will be truncated. The returned content will always
	be null terminated.
	@param ef a valid pointer to \c EDFFILE structure. This handle should be created by
			  calling edf_open_file().
    @param buffer a character array to be filled by the preamble text.
	@param length length of the buffer.
	@return returns \c 0 if the operation is successful. */
PUBLIC_FN int EXPORT  edf_get_preamble_text(EDFFILE *ef,
							char * buffer, int length);


/***********************************************************************
 *Function Name: edf_get_preamble_text_length
 *Input:		 edffile
 *Output: int
 *Purpose: Returns the preamble text length
 ***********************************************************************/
/*!\ingroup GENERALEDFDATAAccess
    Returns the length of the preamble text.
	@param edf  a valid pointer to c EDFFILE structure. This handle should be created by
				calling edf_open_file().
	@return An integer for the length of preamble text.*/
PUBLIC_FN int EXPORT  edf_get_preamble_text_length(EDFFILE * edf);






/***********************************************************************
 *Function Name: edf_get_revision
 *Input:		 edffile
 *Output: int
 *Purpose: returns the edffile revision
 ***********************************************************************/
PUBLIC_FN int EXPORT edf_get_revision(EDFFILE *ef);


/***********************************************************************
 *Function Name: edf_get_eyelink_revision
 *Input:		 edffile
 *Output: int
 *Purpose: returns the revision of the tracker used to record the edf
 *		   file. ie. 1 for eyelinkI, 2 for eyelinkII, 3 for eyelinkCL
 ***********************************************************************/
PUBLIC_FN int EXPORT edf_get_eyelink_revision(EDFFILE *ef);



/*=====================================================================*
 *																	   *
 *						TRIAL RELATED FUNCTIONS						   *
 *																	   *
 *=====================================================================*/
/*!\defgroup TRIALRELATEDFunc Trial Related Functions
   The EDF access API also provides the following trial related functions for
   the ease of counting the total number the trials in the recording file and
   navigating between different trials.  To use this functionality, it is desirable
   that the user first define the trial start/end identifier strings with
   edf_set_trial_identifier(). [The identifier string settings can be checked with
   the edf_get_start_trial_identifier() and edf_get_end_trial_identifier() functions].
   Use edf_jump_to_trial(), edf_goto_previous_trial(), edf_goto_next_trial(),
   edf_goto_trial_with_start_time(), or edf_goto_trial_with_end_time() functions
   to go to a target trial.  The recording and start/end time of the target trial can be
   checked with edf_get_trial_header().
   @{*/

/***********************************************************************
 *Function Name: edf_set_trial_identifier
 *Input:		 edffile, marker_string
 *Output: int
 *Purpose: sets the string that marks the beginning of the trial
 ***********************************************************************/
/*!\brief Sets the message strings that mark the beginning and the end of a trial.
		  The message event that contains the marker string is considered start or
		  end of the trial.
  @param edf a valid pointer to \c EDFFILE structure. This should be created by
			calling edf_open_file().
  @param start_marker_string string that contains the marker for beginning of a trial.
  @param end_marker_string string that contains the marker for end of the trial.
  @return \c 0 if no error occurred.
  @remarks NOTE: The following restrictions apply for collecting the trials.\n
				 1.The \c start_marker_string message should be before the start recording
				 (indicated by message  ``START'').\n
				 2.The \c end_marker_string message should be after the end recording
				 (indicated by message  ``END'').\n
				 3.If the \c start_marker_string is not found before start recording or
				 if the \c start_marker_string is null, start recording will be the starting
				 position of the trial.\n
				 4.If the \c end_marker_string is not found after the end recording,
				 end recording will be the ending position of the trial.\n
				 5.If \c start_marker_string is not specified the string ``TRIALID'',
				 if found, will be used as the \c start_marker_string.\n
				 6.If the \c end_marker_string is not specified, the beginning of the
				 next trial is the end of the current trial.*/
PUBLIC_FN int EXPORT  edf_set_trial_identifier(EDFFILE * edf,
									char *start_marker_string,
									char *end_marker_string);


/***********************************************************************
 *Function Name: edf_get_start_trial_identifier
 *Input:		 edffile
 *Output: int
 *Purpose: gets the string that marks the beginning of the trial
 ***********************************************************************/
/*!\brief Returns the trial identifier that marks the beginning of a trial.
	@param ef a valid pointer to \c EDFFILE structure. This should be created by
	calling edf_open_file().
	@return a string that marks the beginning of a trial.*/
PUBLIC_FN char* EXPORT edf_get_start_trial_identifier(EDFFILE * ef);

/***********************************************************************
 *Function Name: edf_get_end_trial_identifier
 *Input:		 edffile
 *Output: int
 *Purpose: gets the string that marks the beginning of the trial
 ***********************************************************************/
/*!\brief Returns the trial identifier that marks the end of a trial.
 @param ef a valid pointer to \c EDFFILE structure. This should be created by calling
		   edf_open_file().
 @return a string that marks the end of a trial.*/
PUBLIC_FN char* EXPORT  edf_get_end_trial_identifier(EDFFILE * ef);

/***********************************************************************
 *Function Name: edf_get_trial_count
 *Input:		 edffile
 *Output: int
 *Purpose: returns the number of trials
 ***********************************************************************/
/*!\brief Returns the number of trials in the EDF file.
	@param edf a valid pointer to \c EDFFILE structure. This should be created
		   by calling edf_open_file().
   @return an integer for the number of trials in the EDF file.*/
PUBLIC_FN int EXPORT  edf_get_trial_count(EDFFILE *edf);


/***********************************************************************
 *Function Name: edf_jump_to_trial
 *Input:		 edffile
 *Output: int
 *Purpose: jumps to the beginning of a given trial.
 ***********************************************************************/
/*! Jumps to the beginning of a given trial.
	@param edf a valid pointer to \c EDFFILE structure. This should be created
			by calling edf_open_file().
	@param trial trial number.  This should be a value between \c 0 and
			edf_get_trial_count ()- \c 1.
	@return unless there are any errors it returns a 0.*/
PUBLIC_FN int EXPORT  edf_jump_to_trial(EDFFILE * edf, int trial);




/***********************************************************************
 *Function Name: edf_get_trial_headers
 *Input:		 edffile
 *Output: int
 *Purpose: Returns the current trial information
 ***********************************************************************/
/*!\brief Returns the trial specific information. See the TRIAL structure for
	more details.
	@param edf a valid pointer to \c EDFFILE structure. This should be created
				by calling edf_open_file().
	@param trial pointer to a valid \c TRIAL structure (note \c trial must be
				initialized before being used as a parameter for this function).
				This pointer is used to hold information of the current trial.

	@return unless there are any errors it returns a 0.*/

PUBLIC_FN int EXPORT edf_get_trial_header(EDFFILE * edf,TRIAL *trial);




/***********************************************************************
 *Function Name: edf_goto_previous_trial
 *Input:		 edffile
 *Output: int
 *Purpose: moves to the previous trial
 ***********************************************************************/
/*!\brief Jumps to the beginning of the previous trial.
	@param edf a valid pointer to \c EDFFILE structure. This should be created
				by calling edf_open_file().
	@return unless there are any errors it returns \c 0.*/
PUBLIC_FN int EXPORT  edf_goto_previous_trial(EDFFILE * edf);


/***********************************************************************
 *Function Name: edf_goto_next_trial
 *Input:		 edffile
 *Output: int
 *Purpose: moves to the next trial
 ***********************************************************************/
/*!\brief Jumps to the beginning of the next trial.
	@param edf a valid pointer to \c EDFFILE structure.
				This should be created by calling edf_open_file().
	@return unless there are any errors it returns \c 0.*/
PUBLIC_FN int EXPORT edf_goto_next_trial(EDFFILE * edf);


/***********************************************************************
 *Function Name: edf_goto_trial_with_start_time
 *Input:		 edffile, start_time
 *Output: int
 *Purpose: moves to the trial with the given start time
 ***********************************************************************/
/*!\brief Jumps to the trial that has the same start time as the given start time.
	@param edf a valid pointer to \c EDFFILE structure. This should be created by
				calling edf_open_file().
	@param start_time start time of the EDF trial
	@return unless there are any errors it returns 0.*/
PUBLIC_FN int EXPORT edf_goto_trial_with_start_time(EDFFILE * edf,
										  unsigned int start_time);

/***********************************************************************
 *Function Name: edf_goto_trial_with_end_time
 *Input:		 edffile, end_time
 *Output: int
 *Purpose: moves to the trial with the given end time
 ***********************************************************************/
/*! \brief Jumps to the trial that has the same start time as the given end time.
	@param edf a valid pointer to \c EDFFILE structure. This should be created by
				calling edf_open_file().
	@param end_time end time of the EDF trial
	@return unless there are any errors it returns \c 0. */
PUBLIC_FN int EXPORT edf_goto_trial_with_end_time(EDFFILE * edf,
										  unsigned int end_time);
/**@}*/







/*=====================================================================*
 *																	   *
 *					BOOKMARK RELATED FUNCTIONS						   *
 *																	   *
 *=====================================================================*/

/*!\defgroup BOOKMARK Bookmark Related Functions
In addition to navigation between different trials in an EDF recording file
with the functions provided in the previous section, the EDF access API also
allows the user to ``bookmark'' any position of the EDF file using the edf_set_bookmark()
function.  The bookmarks can be revisited with edf_goto_bookmark().  Finally, the bookmarks
should be freed with the edf_free_bookmark() function call.
@{*/

/***********************************************************************
 *Function Name: edf_set_bookmark
 *Input:		 edffile, pointer to bookmark object
 *Output: int
 *Purpose: mark the current position of edffile
 ***********************************************************************/
/*! Bookmark the current position of the edf file.
	@param ef a valid pointer to \c EDFFILE structure. This should be created
				by calling edf_open_file.
	@param bm pointer to a valid \c BOOKMARK structure. This structure will be
				filled by this function.  \c bm should be initialized before
				being used by this function.
	@return unless there are any errors it returns 0.*/
PUBLIC_FN int EXPORT  edf_set_bookmark(EDFFILE *ef, BOOKMARK *bm);

/***********************************************************************
 *Function Name: edf_free_bookmark
 *Input:		 edffile, pointer to bookmark object
 *Output: int
 *Purpose: remove the bookmark
 ***********************************************************************/
/*! Removes an existing bookmark
	@param ef a valid pointer to \c EDFFILE structure. This should be created
				by calling edf_open_file.
	@param bm pointer to a valid \c BOOKMARK structure. This structure will be
				filled by this function.  Before calling this function edf_set_bookmark
				should be called and bm should be initialized there.
	@return unless there are any errors it returns 0.*/
PUBLIC_FN int EXPORT edf_free_bookmark(EDFFILE *ef, BOOKMARK *bm);


/***********************************************************************
 *Function Name: edf_goto_bookmark
 *Input:		 edffile, pointer to bookmark object
 *Output: int
 *Purpose: jump to the bookmark
 ***********************************************************************/
/*! Jumps to the given bookmark.
	@param ef a valid pointer to \c EDFFILE structure. This should be created by calling
				edf_open_file.
	@param bm pointer to a valid \c BOOKMARK structure. This structure will be filled
				by this function.  Before calling this function edf_set_bookmark should
				be called and bm should be initialized there.
	@return unless there are any errors it returns \c 0.*/
PUBLIC_FN int EXPORT edf_goto_bookmark(EDFFILE *ef, BOOKMARK *bm);

/**@}*/
/***********************************************************************
 *Function Name: edf_goto_next_bookmark
 *Input:		 edffile
 *Output: int
 *Purpose: jump to the next bookmark
 ***********************************************************************/
PUBLIC_FN int EXPORT edf_goto_next_bookmark(EDFFILE *ef);

/***********************************************************************
 *Function Name: edf_goto_previous_bookmark
 *Input:		 edffile
 *Output: int
 *Purpose: jump to the previous bookmark
 ***********************************************************************/
PUBLIC_FN int EXPORT edf_goto_previous_bookmark(EDFFILE *ef);



/***********************************************************************
 *Function Name: edf_get_version
 *Input:		 none
 *Output: char *
 *Purpose: returns the version of edfapi
 ***********************************************************************/
/*!\defgroup EDFSpecificFunc EDF Specific Functions
 * @{
 */
/*!Returns a string which indicates the version of EDFAPI.dll library used.
 * @return	a string indicating the version of EDFAPI library used.*/
PUBLIC_FN const char * EXPORT edf_get_version();

/**@}*/


/***********************************************************************
 *Function Name: get_event
 *Input:		 ALLF_DATA *
 *Output: FEVENT *
 *Purpose: convenient function to translate an ALLF_DATA union to FEVENT
 ***********************************************************************/
PUBLIC_FN FEVENT     *  EXPORT edf_get_event(ALLF_DATA *allfdata);

/***********************************************************************
 *Function Name: get_event
 *Input:		 ALLF_DATA *
 *Output: FEVENT *
 *Purpose: convenient function to translate an ALLF_DATA union to FSAMPLE
 ***********************************************************************/
PUBLIC_FN FSAMPLE    *  EXPORT edf_get_sample(ALLF_DATA *allfdata);

/***********************************************************************
 *Function Name: get_event
 *Input:		 ALLF_DATA *
 *Output: FEVENT *
 *Purpose: convenient function to translate an ALLF_DATA union to RECORDINGS
 ***********************************************************************/
PUBLIC_FN RECORDINGS *  EXPORT edf_get_recording(ALLF_DATA *allfdata);




/*








		*/
/*
ELCL
*/
PUBLIC_FN void EXPORT edf_get_uncorrected_raw_pupil(EDFFILE *edf,FSAMPLE *sam, int eye,float *rv);
PUBLIC_FN void EXPORT edf_get_uncorrected_raw_cr(EDFFILE *edf,FSAMPLE *sam, int eye,float *rv);
PUBLIC_FN UINT32   EXPORT edf_get_uncorrected_pupil_area(EDFFILE *edf,FSAMPLE *sam, int eye);
PUBLIC_FN UINT32   EXPORT edf_get_uncorrected_cr_area(EDFFILE *edf,FSAMPLE *sam, int eye);
PUBLIC_FN void EXPORT edf_get_pupil_dimension(EDFFILE *edf,FSAMPLE *sam, int eye, UINT32 *rv);
PUBLIC_FN void EXPORT edf_get_cr_dimension(EDFFILE *edf,FSAMPLE *sam, UINT32 *rv);
PUBLIC_FN void EXPORT edf_get_window_position(EDFFILE *edf,FSAMPLE *sam, UINT32 *rv);
PUBLIC_FN void EXPORT edf_get_pupil_cr(EDFFILE *edf,FSAMPLE *sam, int eye, float *rv);
PUBLIC_FN UINT32   EXPORT edf_get_uncorrected_cr2_area(EDFFILE *edf,FSAMPLE *sam, int eye);
PUBLIC_FN void EXPORT edf_get_uncorrected_raw_cr2(EDFFILE *edf,FSAMPLE *sam, int eye,float *rv);




PUBLIC_FN void  EXPORT edf_calculate_ppd(float hpixel, float vpixel,FSAMPLE *f, int eye, double *rx, double *ry);
PUBLIC_FN int EXPORT edf_compute_physical_setup(float simulation_screen_distance,float simulation_screen_distance_bot,
float screen_phys_l,
float screen_phys_t,
float screen_phys_r,
float screen_phys_b,

float screen_pixel_l,
float screen_pixel_t,
float screen_pixel_r,
float screen_pixel_b
						   );

/*
equivalent functions to edf_get_float_data
*/
FEVENT * edf_get_event_data(EDFFILE *edf);
FSAMPLE * edf_get_sample_data(EDFFILE *edf);
RECORDINGS * edf_get_recording_data(EDFFILE *edf);


/*
Redirect log support
*/
PUBLIC_FN void EXPORT edf_set_log_function(void (*lfcn )(char *log));

#ifdef __cplusplus
};
#endif

#ifdef __cplusplus
/*=====================================================================*
 *																	   *
 *							C++ EDF WRAPPER 						   *
 *																	   *
 *=====================================================================*/


class EDF
{
protected:
	EDFFILE * edf;
public:
/*=====================================================================*
 *																	   *
 *				GENERAL EDF DATA ACCESS FUNCTIONS					   *
 *																	   *
 *=====================================================================*/
	inline EDF(const char *fname, int consistency, int loadevents,
		int loadsamples, int *errval);
	inline EDF(const char *fname, int consistency, int loadevents,
		int loadsamples, CONFIG *config, int *errval);
	virtual inline ~EDF();
	inline int getEyelinkCRMode();
	inline int getNextData();
	inline ALLF_DATA * getFloatData();
	inline int getPreambleText(char * buffer, int length);
	inline int getPreambleTextLength();
	inline int getSampleInterval();
	inline int getRevision();

	inline int getElementCount() { return edf_get_element_count(edf); }

/*=====================================================================*
 *																	   *
 *						TRIAL RELATED FUNCTIONS						   *
 *																	   *
 *=====================================================================*/
	inline int setTrialIdentifier(char *start_string, char *end_string);
	inline int getTrialCount();
	inline int jumpToTrial(int trial);
	inline int getTrialHeader(TRIAL *trial);
	inline int gotoPreviousTrial();
	inline int gotoNextTrial();
	inline int gotoTrialWithStartTime(unsigned int start_time);
	inline int gotoTrialWithEndTime(unsigned int end_time);

/*=====================================================================*
 *																	   *
 *					BOOKMARK RELATED FUNCTIONS						   *
 *																	   *
 *=====================================================================*/
	inline int setBookmark(BOOKMARK *bm);
	inline int freeBookmark(BOOKMARK *bm);
	inline int gotoBookmark(BOOKMARK *bm);
	inline int gotoNextBookmark();
	inline int gotoPreviousBookmark();
    static inline FEVENT * getEvent(ALLF_DATA *allfdata);
    static inline FSAMPLE   * getSample(ALLF_DATA *allfdata);
    static inline RECORDINGS * getRecording(ALLF_DATA *allfdata);
    inline FEVENT * getEvent();
    inline FSAMPLE   * getSample();
    inline RECORDINGS * getRecording();

};



inline EDF::EDF(const char *fname, int consistency, int loadevents,
				int loadsamples,  int *errval)
{
	edf = edf_open_file(fname,consistency, loadevents, loadsamples, errval);
}
inline EDF::EDF(const char *fname, int consistency, int loadevents,
	int loadsamples, CONFIG *config, int *errval)
{
	edf = edf_open_file_ex(fname, consistency, loadevents, loadsamples, config,
		errval);
}
inline EDF::~EDF()
{
	if(edf)
		edf_close_file(edf);

}


inline int EDF::getNextData()
{
	return edf_get_next_data(edf);
}
inline ALLF_DATA * EDF::getFloatData()
{
	return edf_get_float_data(edf);
}
inline int EDF::getPreambleText(char * buffer, int length)
{
	return edf_get_preamble_text(edf,buffer,length);
}
inline int EDF::getPreambleTextLength()
{
	return edf_get_preamble_text_length(edf);
}

inline int EDF::getRevision()
{
	return edf_get_revision(edf);
}

inline int EDF::setTrialIdentifier(char *start_marker_string, char * end_marker_string)
{
	return edf_set_trial_identifier(edf,start_marker_string, end_marker_string);
}


inline int EDF::getTrialCount()
{
	return edf_get_trial_count(edf);
}
inline int EDF::jumpToTrial(int trial)
{
	return edf_jump_to_trial(edf,trial);
}
inline int EDF::getTrialHeader(TRIAL *trial)
{
	return edf_get_trial_header(edf,trial);
}
inline int EDF::gotoPreviousTrial()
{
	return edf_goto_previous_trial(edf);
}
inline int EDF::gotoNextTrial()
{
	return edf_goto_next_trial(edf);
}
inline int EDF::gotoTrialWithStartTime(unsigned int start_time)
{
	return edf_goto_trial_with_start_time(edf, start_time);
}
inline int EDF::gotoTrialWithEndTime(unsigned int end_time)
{
	return edf_goto_trial_with_end_time(edf, end_time);
}

inline int EDF::setBookmark(BOOKMARK *bm)
{
	return edf_set_bookmark(edf, bm);
}
inline int EDF::freeBookmark(BOOKMARK *bm)
{
	return edf_free_bookmark(edf,bm);
}
inline int EDF::gotoBookmark(BOOKMARK *bm)
{
	return edf_goto_bookmark(edf,bm);
}
inline int EDF::gotoNextBookmark()
{
	return edf_goto_next_bookmark(edf);
}
inline int EDF::gotoPreviousBookmark()
{
	return edf_goto_previous_bookmark(edf);
}




inline FEVENT * EDF::getEvent(ALLF_DATA *allfdata)
{
    if(allfdata->fe.type == SAMPLE_TYPE || allfdata->fe.type == RECORDING_INFO)
        return 0L;
    else
        return (FEVENT *)allfdata;
}

inline FSAMPLE   * EDF::getSample(ALLF_DATA *allfdata)
{
    if(allfdata->fe.type == SAMPLE_TYPE)
        return (FSAMPLE *)allfdata;
    else
        return 0L;

}
inline RECORDINGS * EDF::getRecording(ALLF_DATA *allfdata)
{
    if(allfdata->fe.type == RECORDING_INFO)
        return (RECORDINGS *)allfdata;
    else
        return 0L;

}

inline FEVENT * EDF::getEvent()
{
    ALLF_DATA *allfdata = this->getFloatData();
    return EDF::getEvent(allfdata);
}

inline FSAMPLE   * EDF::getSample()
{
    ALLF_DATA *allfdata = this->getFloatData();
    return EDF::getSample(allfdata);
}
inline RECORDINGS * EDF::getRecording()
{
    ALLF_DATA *allfdata = this->getFloatData();
    return EDF::getRecording(allfdata);
}

#endif



#endif


