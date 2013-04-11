/***********************************************************************
 *
 * EYELINK PORTABLE EXPT SUPPORT
 * Copyright (c) 1996-2002 SR Research Ltd.
 *
 * For non-commercial use only
 *
 * EyeLink library data extensions
 *        VERSION 2.5
 * UPDATED for EyeLink DLL V2.1: added LOST_DATA_EVENT
 * This module is for user applications
 * Use is granted for non-commercial
 * applications by Eyelink licensees only
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
#else
#define	EXPORT
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

typedef struct {
	RECORDINGS *    rec;
	unsigned int	duration;
	unsigned int	starttime;
	unsigned int	endtime;
} TRIAL;

typedef struct _EDFFILE EDFFILE;
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

/***********************************************************************
 *Function Name: edf_open_file
 *Input:		 file name, consistency, errval
 *Output: EDFFILE structure, pointer to int.
 *Purpose: opens the given edffile. If the return value is null then
 *		   the file could not be opened or the file is corrupt.
 *		   the errval is 0 if all the operation succeed. non zero
 *		   in other cases.
 ***********************************************************************/
EDFFILE * EXPORT edf_open_file(const char *fname, int consistency,
							   int loadevents, int loadsamples,
							   int *errval);


 /***********************************************************************
 *Function Name: edf_close_file
 *Input:		 file name
 *Output: int
 *Purpose: closes file, frees all memory
 *		   returns an error code. if the error code is 0 then the
 *		   operation succeeded otherwise a non zero is returned.
 ***********************************************************************/
int EXPORT edf_close_file(EDFFILE * ef);










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
int EXPORT edf_get_next_data(EDFFILE *ef);


/***********************************************************************
 *Function Name: edf_get_float_data
 *Input:		 edffile
 *Output: int
 *Purpose: returns the float data with the type returned by
 *			edf_get_next_data
 ***********************************************************************/
ALLF_DATA * EXPORT edf_get_float_data(EDFFILE *ef);



/***********************************************************************
 *Function Name: edf_get_sample_close_to_time
 *Input:		 edffile
 *Output: int
 *Purpose: returns the float sample close to the time
 ***********************************************************************/
ALLF_DATA* EXPORT  edf_get_sample_close_to_time(EDFFILE *ef, unsigned int time);

/***********************************************************************
 *Function Name: edf_get_element_count
 *Input:		 edffile
 *Output: int
 *Purpose: returns the number of elements in the edf file.
 ***********************************************************************/
unsigned int EXPORT  edf_get_element_count(EDFFILE *ef);

/***********************************************************************
 *Function Name: edf_get_preamble_text
 *Input:		 edffile, pointer to char buffer, length of the buffer
 *Output: int
 *Purpose: copies the preamble text into the buffer.
 *		   if the preamble text is longer than the  length the
 *			text will be truncated.
 *			The returned content will always be null terminated
 ***********************************************************************/
int EXPORT  edf_get_preamble_text(EDFFILE *ef,
							char * buffer, int length);


/***********************************************************************
 *Function Name: edf_get_preamble_text_length
 *Input:		 edffile
 *Output: int
 *Purpose: Returns the preamble text length
 ***********************************************************************/
int EXPORT  edf_get_preamble_text_length(EDFFILE * edf);






/***********************************************************************
 *Function Name: edf_get_revision
 *Input:		 edffile
 *Output: int
 *Purpose: returns the edffile revision
 ***********************************************************************/
int EXPORT edf_get_revision(EDFFILE *ef);


/***********************************************************************
 *Function Name: edf_get_eyelink_revision
 *Input:		 edffile
 *Output: int
 *Purpose: returns the revision of the tracker used to record the edf
 *		   file. ie. 1 for eyelinkI, 2 for eyelinkII, 3 for eyelinkCL
 ***********************************************************************/
int edf_get_eyelink_revision(EDFFILE *ef);



/*=====================================================================*
 *																	   *
 *						TRIAL RELATED FUNCTIONS						   *
 *																	   *
 *=====================================================================*/


/***********************************************************************
 *Function Name: edf_set_trial_identifier
 *Input:		 edffile, marker_string
 *Output: int
 *Purpose: sets the string that marks the beginning of the trial
 ***********************************************************************/
int EXPORT  edf_set_trial_identifier(EDFFILE * edf,
									char *start_marker_string,
									char *end_marker_string);


/***********************************************************************
 *Function Name: edf_get_start_trial_identifier
 *Input:		 edffile
 *Output: int
 *Purpose: gets the string that marks the beginning of the trial
 ***********************************************************************/
char* EXPORT edf_get_start_trial_identifier(EDFFILE * ef);

/***********************************************************************
 *Function Name: edf_get_end_trial_identifier
 *Input:		 edffile
 *Output: int
 *Purpose: gets the string that marks the beginning of the trial
 ***********************************************************************/
char* EXPORT  edf_get_end_trial_identifier(EDFFILE * ef);

/***********************************************************************
 *Function Name: edf_get_trial_count
 *Input:		 edffile
 *Output: int
 *Purpose: returns the number of trials
 ***********************************************************************/
int EXPORT  edf_get_trial_count(EDFFILE *edf);


/***********************************************************************
 *Function Name: edf_jump_to_trial
 *Input:		 edffile
 *Output: int
 *Purpose: jumps to the beginning of a given trial.
 ***********************************************************************/
int EXPORT  edf_jump_to_trial(EDFFILE * edf, int trial);




/***********************************************************************
 *Function Name: edf_get_trial_headers
 *Input:		 edffile
 *Output: int
 *Purpose: Returns the current trial information
 ***********************************************************************/
int EXPORT edf_get_trial_header(EDFFILE * edf,TRIAL *trial);




/***********************************************************************
 *Function Name: edf_goto_previous_trial
 *Input:		 edffile
 *Output: int
 *Purpose: moves to the previous trial
 ***********************************************************************/
int EXPORT  edf_goto_previous_trial(EDFFILE * edf);


/***********************************************************************
 *Function Name: edf_goto_next_trial
 *Input:		 edffile
 *Output: int
 *Purpose: moves to the next trial
 ***********************************************************************/
int EXPORT edf_goto_next_trial(EDFFILE * edf);


/***********************************************************************
 *Function Name: edf_goto_trial_with_start_time
 *Input:		 edffile, start_time
 *Output: int
 *Purpose: moves to the trial with the given start time
 ***********************************************************************/
int EXPORT edf_goto_trial_with_start_time(EDFFILE * edf,
										  unsigned int start_time);

/***********************************************************************
 *Function Name: edf_goto_trial_with_end_time
 *Input:		 edffile, end_time
 *Output: int
 *Purpose: moves to the trial with the given end time
 ***********************************************************************/
int EXPORT edf_goto_trial_with_end_time(EDFFILE * edf,
										  unsigned int end_time);








/*=====================================================================*
 *																	   *
 *					BOOKMARK RELATED FUNCTIONS						   *
 *																	   *
 *=====================================================================*/




/***********************************************************************
 *Function Name: edf_set_bookmark
 *Input:		 edffile, pointer to bookmark object
 *Output: int
 *Purpose: mark the current position of edffile
 ***********************************************************************/
int EXPORT  edf_set_bookmark(EDFFILE *ef, BOOKMARK *bm);

/***********************************************************************
 *Function Name: edf_free_bookmark
 *Input:		 edffile, pointer to bookmark object
 *Output: int
 *Purpose: remove the bookmark
 ***********************************************************************/
int EXPORT edf_free_bookmark(EDFFILE *ef, BOOKMARK *bm);


/***********************************************************************
 *Function Name: edf_goto_bookmark
 *Input:		 edffile, pointer to bookmark object
 *Output: int
 *Purpose: jump to the bookmark
 ***********************************************************************/
int EXPORT edf_goto_bookmark(EDFFILE *ef, BOOKMARK *bm);


/***********************************************************************
 *Function Name: edf_goto_next_bookmark
 *Input:		 edffile
 *Output: int
 *Purpose: jump to the next bookmark
 ***********************************************************************/
int EXPORT edf_goto_next_bookmark(EDFFILE *ef);

/***********************************************************************
 *Function Name: edf_goto_previous_bookmark
 *Input:		 edffile
 *Output: int
 *Purpose: jump to the previous bookmark
 ***********************************************************************/
int EXPORT edf_goto_previous_bookmark(EDFFILE *ef);



/***********************************************************************
 *Function Name: edf_get_version
 *Input:		 none
 *Output: char *
 *Purpose: returns the version of edfapi
 ***********************************************************************/
char * EXPORT edf_get_version();




/***********************************************************************
 *Function Name: get_event
 *Input:		 ALLF_DATA *
 *Output: FEVENT *
 *Purpose: convenient function to translate an ALLF_DATA union to FEVENT
 ***********************************************************************/
FEVENT     *  EXPORT edf_get_event(ALLF_DATA *allfdata);

/***********************************************************************
 *Function Name: get_event
 *Input:		 ALLF_DATA *
 *Output: FEVENT *
 *Purpose: convenient function to translate an ALLF_DATA union to FSAMPLE
 ***********************************************************************/
FSAMPLE    *  EXPORT edf_get_sample(ALLF_DATA *allfdata);

/***********************************************************************
 *Function Name: get_event
 *Input:		 ALLF_DATA *
 *Output: FEVENT *
 *Purpose: convenient function to translate an ALLF_DATA union to RECORDINGS
 ***********************************************************************/
RECORDINGS *  EXPORT edf_get_recording(ALLF_DATA *allfdata);



/*
ELCL
*/
void EXPORT edf_get_uncorrected_raw_pupil(EDFFILE *edf,FSAMPLE *sam, int eye,float *rv);
void EXPORT edf_get_uncorrected_raw_cr(EDFFILE *edf,FSAMPLE *sam, int eye,float *rv);
UINT32   EXPORT edf_get_uncorrected_pupil_area(EDFFILE *edf,FSAMPLE *sam, int eye);
UINT32   EXPORT edf_get_uncorrected_cr_area(EDFFILE *edf,FSAMPLE *sam, int eye);
void EXPORT edf_get_pupil_dimension(EDFFILE *edf,FSAMPLE *sam, int eye, UINT32 *rv);
void EXPORT edf_get_cr_dimension(EDFFILE *edf,FSAMPLE *sam, UINT32 *rv);
void EXPORT edf_get_window_position(EDFFILE *edf,FSAMPLE *sam, UINT32 *rv);
void EXPORT edf_get_pupil_cr(EDFFILE *edf,FSAMPLE *sam, int eye, float *rv);
UINT32   EXPORT edf_get_uncorrected_cr2_area(EDFFILE *edf,FSAMPLE *sam, int eye);
void EXPORT edf_get_uncorrected_raw_cr2(EDFFILE *edf,FSAMPLE *sam, int eye,float *rv);




/*
equivalent functions to edf_get_float_data
*/
FEVENT * edf_get_event_data(EDFFILE *edf);
FSAMPLE * edf_get_sample_data(EDFFILE *edf);
RECORDINGS * edf_get_recording_data(EDFFILE *edf);


/*
Redirect log support
*/
void edf_set_log_function(void (*lfcn )(char *log));

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
				int loadsamples, int *errval)
{
	edf = edf_open_file(fname,consistency, loadevents, loadsamples,
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

