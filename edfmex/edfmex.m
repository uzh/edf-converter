% 
% EDFMEX uses the Eyelink EDF access API to read an eyelink data
% file into a matlab structure.
%
% Usage:
% 
%     edfdata = edfmex(filename,[start_at_record_number],[end_at_record_number],
%                               [load_sample_flag], [load_event_flag],[consistency_check_flag]);
% 
% Input:
% 
%     filename                -   name of the file to be loaded.
%     
%     start_at_record_number  -   load data starting at this record number 
%                                    (default is 0 indicating the first record ).
%     
%     end_at_record_number    -   load up to this record number. Default is
%                                  0 indicating read to end of file.
% 
%     load_sample_flag       -    0 to skip samples. Default = 1.
% 
%     load_events_flag       -    0 to skip events. Default = 1.
% 
%     consistency_check_flag -    Performs file consistency check as
%                                 described in the API documentation:
%                                 see edf_open_file(). Default = 1.
%                                 
% 
% edfdata:  A matlab structure containing the following fields:
%     
%   edfdata.FSAMPLE     - a matlab stucture with fields corresponding to the 
%                         FSAMPLE structure as described in the Eyelink EDF 
%                         access API documentation. Each field is an M x Nsamp 
%                         array, where Nsamp is the number of samples.
%                         
% 	edfdata.FEVENT      - a 1 x Nevent structure array, where Nevent is 
%                         the number of events. Fields correspond to the 
%                         fields of the FEVENT structure as described in 
%                         the API documentation.
%                         
% 	edfdata.IOEVENT     -  as above for the IOEVENT structure.
%     
% 	edfdata.RECORDINGS  - as above for the RECORDINGS structure.
%                         
% 	edfdata.HEADER      - Header information.
%                         
% 	edfdata.FILENAME    - Name of the file from which data are loaded.
% 
%   To recompile from the source code under 32 bit Windows, make sure the
%   source directory contains edfapi.dll and edfapi.lib, as well as 
%   the headers edf.h edf_data.h and edftypes.h which can be obtained 
%   from the Eyelink support website.
%
%   I compiled successfully in matlab using microsoft visual C++ compiler 
%   with the command:
%
%        mex edfmex.cpp edfapi.lib
% 

%Last modified Nov. 28th 2007

% Written by C. Kovach 2007 and released for non-commercial use 
% without any gaurantee whatsoever. I am not responsible for any 
% loss or corruption of data, damage to hardware, personal injury, 
% marital discord or any other disaster arising from the use of 
% this software. 
%
% Questions can be sent to christopher-kovach@uiowa.edu.

 
