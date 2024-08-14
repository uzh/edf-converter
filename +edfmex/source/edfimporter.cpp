// Some small changes and renaming
// - 2013 by Adrian Etter
// - 2024 by Marc Biedermann
/*
 * Written by C. Kovach 2007 and released for non-commercial use
 * without any gaurantee whatsoever: I am not responsible for any
 * loss or corruption of data, damage to hardware, personal injury,
 * marital discord or any other disaster arising from the use of
 * this software. Feel free to modify as needed.
 * Questions can be sent to christopher-kovach@uiowa.edu.
 */

// #pragma once

// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
#include <stdio.h>
#include "edfmex.h"
#include "edf2mex.h"

BuildMexArrays::~BuildMexArrays() // The rest of this class is defined in edf2mex.h
{
    edf_close_file(edfptr);
};

int BuildMexArrays::PopulateArrays(int Offset, int MaxRec)
{

    int modtest = 0;
    int done = 0;
    int stepsize = 100;                               // size of displayed steps is 100./stepsize
    mxArray *timeout = mxCreateDoubleScalar(0.00001); // MATLAB 2015b hack, otherwise progress is not showing

    mexPrintf("Converting:%3i%%\n", 0);
    mexCallMATLAB(0, NULL, 0, NULL, "drawnow");
    mexCallMATLAB(0, NULL, 1, &timeout, "pause");

    for (int i = 0; (MaxRec == 0 && i < Nrec) || i < MaxRec; i++)
    {
        IncrementRecord();
        if (Offset == 0 || i >= Offset)
        {
            AppendRecord();
            done = (i - Offset + 1) * stepsize / (Nrec - Offset);
            if (done > modtest)
            {
                mexPrintf("\b\b\b\b%3i%%", done * 100 / stepsize);
                mexCallMATLAB(0, NULL, 0, NULL, "drawnow");
                mexCallMATLAB(0, NULL, 1, &timeout, "pause");
                modtest++;
            };
        };
    };

    char headertext[1000];

    edf_get_preamble_text(edfptr, headertext, 1000);
    mxAddField(OutputMexObject, "HEADER");
    mxSetField(OutputMexObject, 0, "HEADER", mxCreateString(headertext));
    mxAddField(OutputMexObject, "FILENAME");
    mxSetField(OutputMexObject, 0, "FILENAME", mxCreateString(FileName));

    mexPrintf("\n");
    return 0;
};

int load_samples = 1;
int load_events = 1;
int consistency_check = 1;
mwSize countN;

int err = 0;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int MaxRec = 0;
    int Offset = 0;

    char FileName[100];

    int chk;
    chk = mxGetString(prhs[0], FileName, mxGetNumberOfElements(prhs[0]) + 1);
    if (chk == 1)
        mexErrMsgTxt("Not a valid string");

    if (nrhs >= 2)
        Offset = *mxGetPr(prhs[1]);
    if (nrhs >= 3)
        MaxRec = *mxGetPr(prhs[2]);
    if (nrhs >= 4)
        load_samples = *mxGetPr(prhs[3]);
    if (nrhs >= 5)
        load_events = *mxGetPr(prhs[4]);
    if (nrhs >= 6)
        consistency_check = *mxGetPr(prhs[5]);

    BuildMexArrays BuildMex(FileName, (int)consistency_check, (int)load_events, (int)load_samples);

    BuildMex.PopulateArrays(Offset, MaxRec);
    BuildMex.CreateMexStruc();

    plhs[0] = BuildMex.OutputMexObject;

    nlhs = 1;
};
