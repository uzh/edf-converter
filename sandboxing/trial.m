%% Set Filenam
FILENAME = 'sampleonly.edf';

%% Set needed Variables

% Header = struct('Date', [], ...
%                 'Type', [], ...
%                 'Version', [], ...
%                 'Source', [], ...
%                 'Type', [], ...
%                 'System', [], ...
%                 'Camera', [], ...
%                 'SerialNumber', [], ...
%                 'CameraConfig', [] ...
%                  );

%
% ** CONVERTED FROM bbbbbb2.edf using edfapi 3.1 Win32 Apr 29 2011 on Wed Aug 08 13:54:32 2012
% ** DATE: Tue Aug  7 09:53:09 2012
% ** TYPE: EDF_FILE BINARY EVENT SAMPLE TAGGED
% ** VERSION: EYELINK II 1
% ** SOURCE: EYELINK CL
% ** EYELINK II CL v4.51 Mar 13 2010
% ** CAMERA: EyeLink CL Version 1.4 Sensor=BJA
% ** SERIAL NUMBER: CL1-AAB35
% ** CAMERA_CONFIG: AAB35140.SCD

%% Make Sample File
[status, output] = dos(['edf2asc -s -y ' FILENAME]);

if isempty(strfind(output, 'Converted successfully:'))
    throw(MException('EdfConverter:Edf2Asc',['Something went wrong, check log:\n' output]));
end

%% Open ASCII file
ascname = strrep(FILENAME, '.edf', '.asc');
fID = fopen(ascname, 'r');
%% READ

C = textscan(fID, '%f %f %f %f %*s', 'delimiter', '\t', ...
    'EmptyValue', nan);
%%
fclose(fID);
%
%%
            %%
y = 0;
tline = fgetl(fID);
while ischar(tline)
   matches = strfind(tline, literal);
   num = length(matches);
   if num > 0
      y = y + num;
      fprintf(1,'%d:%s\n',num,tline);
   end
   tline = fgetl(fID);
end
fclose(fID);

%% CLOSE FILE
fclose(fID);

%% PROCESS CONTENT 