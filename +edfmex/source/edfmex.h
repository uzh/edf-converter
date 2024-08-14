// small changes and formatting 2024 by Marc Biedermann
/*
Written by C. Kovach 2007 and released for non-commercial use
without any gaurantee whatsoever: I am not responsible for any
loss or corruption of data, damage to hardware, personal injury,
marital discord or any other disaster arising from the use of
this software.
Questions can be sent to christopher-kovach@uiowa.edu.
*/

#pragma once

#ifndef single
#define single float
#endif

#include "mex.h"
#include "matrix.h"
#include <stdlib.h>
#include "string.h"
#include <vector>
#include "edf.h"
#include "edf_data.h"
#include "edftypes.h"

// Base class for the BuildMexArrays class (which is defined in edf2mex.h)
class BuildMexArraysBaseClass
{

public:
	int DataCode;
	int fileError;
	int Nrec;
	char *FileName;
	const char *RecordTypes[100];

	EDFFILE *edfptr;
	ALLF_DATA *CurrRec;
	mxArray *OutputMexObject;
	int Initialize(char *filenamein, int consistency_check, int load_events, int load_samples);

	int InitializeMexArrays();

	int CreateMexArrays();

	int IncrementRecord();

	int AppendRecord(); // defined in edf2mex.h

	int GetDataCode();
};

int BuildMexArraysBaseClass::IncrementRecord()
{
	DataCode = edf_get_next_data(edfptr); // Calls eyelink API and steps to next record
	//	CurrRec = edf_get_float_data( edfptr);
	return 0;
};

int BuildMexArraysBaseClass::Initialize(char *filenamein, int consistency_check, int load_events, int load_samples)
{
	fileError = 0;

	FileName = filenamein;

	try
	{
		edfptr = edf_open_file(FileName, consistency_check, load_events, load_samples, &fileError);
		if (edfptr == NULL)
			throw 0;
	}
	catch (int)
	{
		mexErrMsgTxt("Failed to open file.");
	}

	Nrec = edf_get_element_count(edfptr); // Number of records

	return 0;
};

int BuildMexArraysBaseClass::GetDataCode()
{
	return DataCode;
};
