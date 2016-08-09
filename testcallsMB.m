%% Programm for generate an exaple of Eyedata.
%define filename and path
fprintf('\nEyedata generating program:\n\n');
defaultfile = 'testEye.edf';

[file,path] = uiputfile(defaultfile,'Save file name');

assert(numel(file)<=12, 'testcallsMB:filename:length', 'Filename too long');

%% initialization sequence
Eyelink('initializedummy');				
Eyelink('shutdown');

% standard initialization sequence
status = Eyelink('initialize');
if status,
	error('fatal error in intitialization'); 
    fprintf('standard initialization failed:  %d\n',status);
end

%% define Eyelink default settings and constants
fprintf('Initializing defaults and constants\n');
el=EyelinkInitDefaults;

assert(boolean(Eyelink('isconnected')), ...
    'testcallsMB:Initianlize:Connection', 'Is not connected');

%% open File
Eyelink('openfile', file);

%% set Eyelink options (alternatively one can use defaults in Operater PC)
Eyelink('command', 'clear_screen 0');		% clear operater screen 
Eyelink('command', 'active_eye = RIGHT');		% set eye to record
Eyelink('command', 'binocular_enabled = NO');	
Eyelink('command', 'head_subsample_rate = 0');	% normal (no anti-reflection)
Eyelink('command', 'heuristic_filter = ON');	% ON for filter (normal)	
Eyelink('command', 'pupil_size_diameter = NO');	% no for pupil area
Eyelink('command', 'simulate_head_camera NO');	% NO to use head camera

Eyelink('command', 'calibration_type = HV9');
Eyelink('command', 'enable_automatic_calibration = YES');	% YES default
Eyelink('command', 'automatic_calibration_pacing = 1000');	% 1000 ms default

Eyelink('command', 'saccade_velocity_threshold = 35');
Eyelink('command', 'saccade_acceleration_threshold = 9500');
Eyelink('command', ...
	'file_event_filter = LEFT,RIGHT,FIXATION,SACCADE,BLINK,MESSAGE,BUTTON');
	
Eyelink('command', 'link_event_filter = LEFT,RIGHT,FIXATION,BUTTON');
Eyelink('command', 'link_sample_data  = LEFT,RIGHT,GAZE,AREA');

%% Put trial number at the bottom of operater display
Eyelink('command', 'record_status_message ''TRIAL %d''', 1);
Eyelink('message', 'TRIAL ID %d', 1);	
Eyelink('startrecording');

WaitSecs(0.1);

assert(~boolean(Eyelink('checkrecording')), ...
    'testcalls:recording','Eyelink is not recording.');

Eyelink('message', 'DISPLAY_ON');		%marker for RT in analysis
Eyelink('message', 'SYNCTIME');			%zero-plot time for EDFVIEW
fprintf('retrive available eye (0-left; 1-right; 2-binoc):  %d\n', ...
		Eyelink('eyeavailable'));

%% test sampling routines
while ~Eyelink('newfloatsampleavailable')==1; end;	% wait till new sample
evt = Eyelink('newestfloatsample'); 	% get the sample
fprintf('\nThe following is the float sample data structure:\n');
disp(evt);								%write out sample

%% Generating Eyedata while:

WaitSecs(20);

%% End of program
Eyelink('stoprecording');

assert(boolean(Eyelink('receivefile',file)), ...
    'testcallsMB:file:recieved', 'File could not be saved.');

Eyelink('shutdown');
fprintf('Program Complete\n');
