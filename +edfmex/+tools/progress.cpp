    #include "mex.h"


    void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        int nrJobs = 1000;
        int progress = 0;
        int progressInPercentage = 0;
        mxArray *timeout = mxCreateDoubleScalar(0.00001);

        mexPrintf("Converting:%3i%%\n",0);
        mexCallMATLAB(0, NULL, 0, NULL, "drawnow");

        while (progress < nrJobs) {
            progressInPercentage = ++progress * 100 / nrJobs;
            mexPrintf("\b\b\b\b\b%3i%%\n", progressInPercentage);               
            mexCallMATLAB(0, NULL, 0, NULL, "drawnow");        
            mexCallMATLAB(0, NULL, 1, &timeout, "pause");
        }

    }
