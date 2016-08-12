classdef Edf2Mat < handle
    % Edf2Mat is a converter to convert Eyetracker data files to
    % Matlab file and perform some tasks on the data
    %
    % The new procedure uses code from SR-Research that returns all info of
    % the edf and not just part of it. The new routine is based on the work
    % of C. Kovach 2007 and is only for non-commercial use!
    %
    %
    % Syntax: Edf2Mat(filename);
    %         Edf2Mat(filename, verbose);
    %
    % Inputs:
    %   filename:           must be of type *.edf     
    %   useOldProcedure:    If you want to use the old procedure with
    %                       edf2asc.exe, you can set this argument to
    %                       true, default is false
    %   verbose:            logical, can be true or false, default is true.
    %                       If you want to supress output to console, 
    %                       verbose has to be false    
    %
    % Outputs:
    %	The Edf2Mat Object
    %
    % Example:
    %   Give here some examples on how to use the class, eg:
    %   edf = Edf2Mat('example.edf');
    %   edf = Edf2Mat('example.edf', true);       % to activate the old
    %                                               procedure set this
    %                                               argument to true.
    %                                               Instead of using the
    %                                               SR-Research mex file,
    %                                               a edf to ascii
    %                                               converter is used and
    %                                               then processed. Only
    %                                               avaliable for windows.
    %   edf = Edf2Mat('example.edf', true, true); % verbose mode = false:
    %                                               no ouput is generated
    %                                               to the matlab command
    %                                               window. This option is
    %                                               only available if the
    %                                               old procedure is active
    %
    % Other m-files required:
    %   everything in the @folder & private folder is required,
    %   private/edf2asc.exe and private/processEvents.m, the mex files and
    %   the dll's/frameworks. On Mac the edfapi.framework must be copied to
    %   /Library/Framworks/ !!!Not the personal Library but to the root
    %   Library
    %
    % Other Classes required:
    %   no
    %
    %
    % See also: Edf2Mat.plot(), Edf2Mat.save(), Edf2Mat.heatmap()
    %           Edf2Mat.Events, Edf2Mat.Samples, Edf2Mat.Header          
    %           Edf2Mat.about(), Edf2Mat.version()
    %
    % <a href="matlab:Edf2Mat.about()">Copyright & Info</a>
    
    % License:
    %     Copyright (c) 2013, Adrian Etter
    %     All rights reserved.
    % 
    %     Redistribution and use in source and binary forms, with or
    %     without modification, are permitted provided that the following
    %     conditions are met:
    %
    %     Redistributions of source code must retain the above copyright
    %     notice, this list of conditions and the following disclaimer.
    %     Redistributions in binary form must reproduce the above copyright
    %     notice, this list of conditions and the following disclaimer in
    %     the documentation and/or other materials provided with the
    %     distribution. Neither the name of the UNIVERSITY OF ZURICH nor the
    %     names of its contributors may be used to endorse or promote
    %     products derived from this software without specific prior
    %     written permission.
    % 
    %     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
    %     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    %     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    %     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    %     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    %     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    %     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    %     LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
    %     USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
    %     AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    %     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    %     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    %     POSSIBILITY OF SUCH DAMAGE.
    
    % Event structure from SR-Research:
    %     float  px[2], py[2];    /* pupil xy */
    % 	  float  hx[2], hy[2];    /* headref xy */
    % 	  float  pa[2]; 		 /* pupil size or area */
    % 
    % 	  float gx[2], gy[2];    /* screen gaze xy */
    %     float rx, ry;          /* screen pixels per degree */
    %     UINT32 time; /* effective time of event */
    %     INT16 type; /* event type */
    %     UINT16 read; /* flags which items were included */
    %     INT16 eye; /* eye: 0=left, 1=right */
    %     UINT32 sttime; /* start time of the event */
    %     UINT32 entime; /* end time of the event */
    %     float hstx, hsty; /* headref starting points */
    %     float gstx, gsty; /* gaze starting points */
    %     float sta;        // Undocumented by sr-research, adrian etters
    %     assumption: start area of pupil
    %     float henx, heny; /* headref ending points */
    %     float genx, geny; /* gaze ending points */
    %     float ena;        // Undocumented by sr-research, adrian etters
    %     assumption: end area of pupil
    %     float havx, havy; /* headref averages */
    %     float gavx, gavy; /* gaze averages */
    %     float ava;        // Undocumented by sr-research, adrian etters
    %     assumption: average area of pupil
    %     float avel; /* accumulated average velocity */
    %     float pvel; /* accumulated peak velocity */
    %     float svel, evel; /* start, end velocity */
    %     float supd_x, eupd_x; /* start, end units-per-degree */
    %     float supd_y, eupd_y; /* start, end units-per-degree */
    %     UINT16 status; /* error, warning flags */
    %     UINT16 flags; /* error, warning flags */
    %     UINT16 input;
    %     UINT16 buttons;
    %     UINT16 parsedby; /* 7 bits of flags: PARSEDBY codes*/
    %     LSTRING *message; /* any message string
    
    % CONSTANT AND HIDDEN VARIABLES
    properties (Constant, Hidden)
        HEADERSTART = 316;
    end
    
    % CONSTANT VARIABLES
    properties (Constant)            
        % Here we add the Version, Author, Copyright Info and the Change
        % Log
        AUTHOR = 'Adrian Etter, Marc Biedermann'; % Author of the Class
        AUTHOREMAIL = 'engineering@econ.uzh.ch'; % Email of the Author
        COPYRIGHT = {'Department of Economics'; 'University of Zurich'}; % Copyright Info
        VERSION = 1.10;  % Number of the latest Version
        VERSIONDATE = '2016/August/12'; % Date of the latest Version
        CHANGELOG = {eval([mfilename '.VERSIONDATE']), 'Add new Feature heatmap to the class'; ...
            '2016/June/15', 'Add backwards compatibility with older macs'; ...
            '2016/June/14', 'fix version mess'; ...
            '2016/Jan/11', 'added new function for timeline and normalized timeline and blinktimeline'; ...
            '2013/April/11', 'Complete backward compatibility added'; ...
            '2013/May/07', 'Bug fixes and renaming of variable. Old names persist! All event types defined'; ...
            '2013/April/11', 'Changed to mex instead of using edf.asc.exe'; ...
            '2012/Aug/10', 'Finished'; ...
            '2012/Aug/9', 'created'};
        
        EVENT_TYPES = struct('STARTPARSE', 1, ... 	% /* these only have time and eye data */
        'ENDPARSE', 2, ...
        'BREAKPARSE', 10, ...
        ...
        ... % 			/* EYE DATA: contents determined by evt_data */
        'STARTBLINK', 3, ...    % /* and by "read" data item */
        'ENDBLINK', 4, ...    % /* all use IEVENT format */
        'STARTSACC', 5, ...
        'ENDSACC', 6, ...
        'STARTFIX', 7, ...
        'ENDFIX', 8, ...
        'FIXUPDATE', 9, ...
        ... %
        ... %   /* buffer = (none, directly affects state), btype = CONTROL_BUFFER */
        ... %
        ... % 			 /* control events: all put data into */
        ... % 			 /* the EDF_FILE or ILINKDATA status  */
        'STARTSAMPLES', 15, ...  % /* start of events in block */
        'ENDSAMPLES', 16, ...  % /* end of samples in block */
        'STARTEVENTS', 17, ...  % /* start of events in block */
        'ENDEVENTS', 18, ...  % /* end of events in block */
        ... %
        ... %
        ... %
        ... %  	/* buffer = IMESSAGE, btype = IMESSAGE_BUFFER */
        ... %
        'MESSAGEEVENT', 24, ...  % /* user-definable text or data */
        ... %
        ... %
        ... % 	/* buffer = IOEVENT, btype = IOEVENT_BUFFER */
        ... %
        'BUTTONEVENT', 25, ...  % /* button state change */
        'INPUTEVENT', 28, ...  % /* change of input port */
        ... 
        'LOST_DATA_EVENT', hex2dec('3F'));   %/* NEW: Event flags gap in data stream */
    
        RECORDING_STATES    = struct('START', 1, 'END', 0);
        EYES                = struct('LEFT', 1, 'RIGHT', 2, 'BINOCULAR', 3);
        PUPIL               = struct('AREA', 0, 'DIAMETER', 1); 
        MISSING_DATA_VALUE  = -32768;
        EMPTY_VALUE         = 1e08; % only for backward compatibility with old Edf2Mat ...
        
    end  
    
    % PRIVATE VARIABLES
    properties(SetAccess = private, GetAccess = private)
        % Here come the properties, which only can be read and written from
        % the class itself AND aren't visible from the outside
        
        % Description of the variable can also be above the variable.
        oldProcedure    = false;
        verbose         = false;
        output;
        cases           = struct('samples', 'Samples', 'events', 'Events');
        imhandle;
    end
    
    % PRIVATE WRITABLE VARIABLES
    properties(SetAccess = private, GetAccess = public)
        
        filename; % The name of the EDF File converted
        
        % The Header of the Eyetrackerdata, Information about the Eyetrackerdata
        Header = struct('date', [], ...
                'type', [], ...
                'version', [], ...
                'source', [], ...
                'system', [], ...
                'camera', [], ...
                'serial_number', [], ...
                'camera_config', [] ...
                );
            
        % The Samples of the Eyetrackerdata
        Samples = struct('time', [], 'posX', [], 'posY', [], 'pupilSize', []);
        
        % The Events of the Eyetrackerdata
        Events = struct('Messages', [], ...
                        'Start', [], ...
                        'Input', [], ...
                        'Buttons', [], ...
                        'prescaler', [], ...
                        'vprescaler', [], ...
                        'pupilInfo', [], ...
                        'Sfix', [], ...
                        'Efix', [], ...
                        'Ssacc', [], ...
                        'Esacc', [], ...
                        'Sblink', [], ...
                        'Eblink', [], ...
                        'End', [] ...
                        );
         % The converted Edf structure generated by the edfmex mex routine
         RawEdf = struct();
                    
    end
    
    properties (Dependent)        
        samplesFilename;    % The file name of ASCII Files which stores all samples
        eventsFilename;     % The file name of ASCII Files which stores all events
        matFilename;        % The file name of MAT File which stores all Header, Samples and Events
        timeline;
        normalizedTimeline;
    end
    
    % PUBLIC METHODS
    methods % Here come all the public functions
        function obj = Edf2Mat(filename, useOLDProcedure, verbose)
            assert(ispc || ismac, 'Edf2Mat:computer', 'This class is only available on windows or mac!');
            assert(exist('filename', 'var') ...
                  && ischar(filename) ...
                  && size(filename, 2) >= 4 ...
                  && strcmp(filename(end - 3:end), '.edf'), ...
                  'EdfConverter:filename', ...
                  'Bad Filename, Filename must be given and be of type .edf!');
              
              assert(logical(exist(filename, 'file')),...
              'EdfConverter:filenotfound', ...
              ['File ' filename ' not found!']);
            
            obj.filename = filename;
            
            if exist('useOLDProcedure', 'var')
                try
                    obj.oldProcedure = logical(useOLDProcedure);
                catch e
                    e = e.addCause(MException('Edf2Mat:oldProcedure', 'Bad Argument: 2nd argument has to be of type logical!'));
                    rethrow(e);
                end
                
                if obj.oldProcedure
                    if ~(ispc)
                        error('Edf2Mat:computer', 'The old procedure is only available on Windows!');
                    end
                end
                
                if exist('verbose', 'var')
                    try
                        obj.verbose = logical(verbose);
                    catch e
                        e = e.addCause(MException('Edf2Mat:verbose', 'Bad Argument: 3rd argument has to be of type logical!'));
                        rethrow(e);
                    end
                end
            end
            
            obj.processFile();
        end   
        
        function processFile(obj)
            importer = @(varargin)edfimporter(varargin{:});
            if ~obj.oldProcedure  
                if ismac
                    [~, version] = unix('sw_vers -productVersion');
                    version = strsplit(version, '.');
                    version = str2double(version{2});
                    if (version < 11) 
                       importer = @(varargin)edfimporter_pre11(varargin{:});
                    end                
                end
                obj.RawEdf      = importer(obj.filename);
                obj.Header.raw  = obj.RawEdf.HEADER;
                obj.Samples     = obj.RawEdf.FSAMPLE;
            end
            
            obj.convertSamples();
            obj.createHeader();
            obj.convertEvents();
            if obj.verbose
                disp('Edf succesfully converted, processed.!');
            end

        end
        
        function samplesFilename = get.samplesFilename(obj)
            if obj.oldProcedure
                samplesFilename = strrep(obj.filename, '.edf', ['_' lower(obj.cases.samples) '.asc']);
            else
                samplesFilename = 'not available for new procedure!';
            end
        end
        
        function eventsFilename = get.eventsFilename(obj)
            if obj.oldProcedure
                eventsFilename = strrep(obj.filename, '.edf', ['_' lower(obj.cases.events) '.asc']);
            else
                eventsFilename = 'not available for new procedure!';
            end
        end
        
        function matFilename = get.matFilename(obj)
            matFilename = strrep(obj.filename, '.edf', '.mat');
        end    
        
        function timeline = get.timeline(obj)
            timeline = obj.getTimeline();
        end
        
        function timeline = get.normalizedTimeline(obj)
            timeline = obj.getNormalizedTimeline();
        end
        
        function convertSamples(obj)
            if obj.oldProcedure
                obj.convertFile(obj.cases.samples);
            end
            obj.processSamples();
        end
        
        function convertEvents(obj)
            if obj.oldProcedure
                obj.convertFile(obj.cases.events);
            end
            obj.processEvents();
        end
        
        function save(obj)
            % some how we need to make new copies to store them in a
            % file ...
            header  = obj.Header;
            samples = obj.Samples;
            events  = obj.Events;
            edf     = obj.RawEdf;
            thisobj = obj;
            vname   = @(x) inputname(1);
            builtin('save', obj.matFilename, vname(header), vname(samples), vname(events), vname(edf), vname(thisobj));
        end    
        
        function [timeline, offset] = getTimeline(obj)
            timeline       = (obj.Events.Start.time:obj.Events.End.time).';
            offset = timeline(1);
        end
        
        function [timeline, offset] = getNormalizedTimeline(obj)
            [timeline, offset] = obj.getTimeline();
            timeline       = timeline - offset;
        end
        
        function blinkTimeline = getBlinkTimeline(obj)
            startIndecies = arrayfun(@(x)find(obj.Samples.time == x),  ...
                    obj.Events.Eblink.start).';
            endIndecies = arrayfun(@(x)find(obj.Samples.time == x),  ...
                    obj.Events.Eblink.end).';
            
            blinks  = mat2cell([startIndecies, endIndecies], ones(numel(startIndecies), 1));
            blinkTimeline = zeros(numel(obj.timeline), 1);
            blinkTimeline(cell2mat(cellfun(@(x)colon(x(1), x(2)).', ...
                blinks, 'UniformOutput', false))) = 1;
        end
        
        function messageTimeline = getMessageTimeline(obj)
            messageTimes        = unique(obj.Events.Messages.time);
            extendedTimeline    = unique([obj.timeline(:); messageTimes(:)]).';
            messageTimeline     = nan(numel(extendedTimeline), 1);
            messageTimeline(ismember(extendedTimeline, messageTimes))    = 1;
        end
       
    end
    
    % PRIVATE METHODS
    methods (Access = private)       

        function convertFile(obj, kind)
            if obj.oldProcedure
                if obj.verbose
                    disp('Trying to convert!')
                    disp(['Processing ' kind '. Please wait ...']);
                end
                [path, ~, ~] = fileparts(which(mfilename));
                command = ['"' path '\private\edf2asc.exe" -miss nan -y '];
                switch kind
                    case obj.cases.samples
                        command = [command '-s '];
                    case obj.cases.events
                        command = [command '-e -t '];
                    otherwise
                        return;
                end
                
                [~, obj.output] = dos([command obj.filename]);
                
                if isempty(strfind(obj.output, 'Converted successfully:'))
                    throw(MException('EdfConverter:Edf2Asc',['Something went wrong, check log:\n' obj.output]));
                end
                
                if obj.verbose
                    disp([kind ' successfully converted!']);
                end
                
                obj.movefile(kind);
            end
        end
        
        function createHeader(obj)
            names = fieldnames(obj.Header);
            names = names(1:end - 1);  % we skip the raw entry
            if obj.oldProcedure
                lineNrs = strfind(obj.output, 'Processed');
                header  = obj.output(1 : lineNrs(1) - 1);
                header = header(obj.HEADERSTART:end);
            else % create old header elements for backward compatibility
                header = obj.Header.raw;
            end
            header  = textscan(header, '%s', 'delimiter', sprintf('\n'));
            header{1} = strrep(header{1}, '**', '');
            header{1} = strrep(header{1}, '|', '');
            header{1} = strrep(header{1}, '=', '');
            for i = 1 : size(names, 1)
                line = i;
                if line > size(header{1}, 1), break; end
                obj.Header.(names{i}) = strtrim(strrep(header{1}{line}, [upper(names{i}) ': '], ''));
            end
            obj.Header.raw = header{:};

            % sometimes we don't have all fields, especially when tracker
            % records in dummy mode
            if ~isempty(obj.Header.serial_number)
                obj.Header.serial_number = strrep(obj.Header.serial_number, 'SERIAL NUMBER: ', '');
            end
            
        end
        
        function movefile(obj, kind)
            asciiname = strrep(obj.filename, '.edf', '.asc');
            switch kind
                case obj.cases.samples
                    newfilename = obj.samplesFilename;                   
                case obj.cases.events
                    newfilename = obj.eventsFilename;
                otherwise
                    return;
            end
            movefile(asciiname, newfilename, 'f');
        end
        
        function processSamples(obj)
            if obj.oldProcedure
                fID = fopen(obj.samplesFilename, 'r');
                % Read ASCII-Samples File
                samples = textscan(fID, '%f %f %f %f %*s', 'delimiter', '\t', ...
                    'EmptyValue', nan);
                obj.Samples =  cell2struct(samples', fieldnames(obj.Samples));
                % Close ASCII-Samples File
                fclose(fID);
            else % create old header elements for backward compatibility
                names = fieldnames(obj.Samples);
                % make values double for easier computation
                for i = 1 : size(names, 1)
                    samples = double(obj.Samples.(names{i})).';
                    samples(samples == obj.EMPTY_VALUE) = nan;
                    obj.Samples.(names{i}) = samples;
                end
                
                recNr = nan(size(obj.Samples.time, 1), 1);
                endRecordings = obj.RawEdf.RECORDINGS([obj.RawEdf.RECORDINGS.state].' == obj.RECORDING_STATES.END);
                if isempty(endRecordings)
                    warning('Edf2Mat:processSamples:noend', 'Recording was not ended properly! Assuming recorded eye stayed the same for this trial!');
                    startRec = obj.RawEdf.RECORDINGS([obj.RawEdf.RECORDINGS.state].' == obj.RECORDING_STATES.START);
                    eye_used = zeros(size(obj.Samples.time, 1), 1) + double(startRec(1).eye);
                else
                    for i = 1 : numel(endRecordings)
                        recNr(obj.Samples.time < endRecordings(i).time) = i;
                    end
                    eye_used = double([obj.RawEdf.RECORDINGS(recNr).eye]).';
                end
                
                
                if any(eye_used == obj.EYES.BINOCULAR)
                    obj.Samples.posX        = obj.Samples.gx;
                    obj.Samples.posY        = obj.Samples.gy;
                    obj.Samples.pupilSize   = obj.Samples.pa;
                else
                    % add old fields
                    obj.Samples.posX        = obj.Samples.gx(sub2ind(size(obj.Samples.gx), 1:numel(eye_used), eye_used(:)')).'; % select column depending on the eye used!
                    obj.Samples.posY        = obj.Samples.gy(sub2ind(size(obj.Samples.gy), 1:numel(eye_used), eye_used(:)')).';
                    obj.Samples.pupilSize   = obj.Samples.pa(sub2ind(size(obj.Samples.pa), 1:numel(eye_used), eye_used(:)')).';
                end
                
            end
        end
        
        % function proccessEvents is in private/processEvents
    end
    
    % STATIC METHODS    
    methods (Static)
        function ver = version()
            % Edf2Mat.version returns the version of the class
            %
            % Syntax: Edf2Mat.version()
            %
            % Inputs:
            %   No Inputs
            %
            % Outputs:
            %	version
            %
            % Example: Edf2Mat.version();
            %
            % See also: Edf2Mat.plot(), Edf2Mat.save()
            %           Edf2Mat.Events, Edf2Mat.Samples, Edf2Mat.Header
            %           Edf2Mat.about(), Edf2Mat.version()
            
            ver = eval([class(eval(mfilename)) '.VERSION']);
            
        end
        function about()
            % Edf2Mat.about prints everything about the class
            %
            % Syntax: Edf2Mat.about()
            %
            % Inputs:
            %   No Inputs
            %
            % Outputs:
            %	No Outputs
            %
            % Example: Edf2Mat.about();
            %
            % See also: Edf2Mat.plot(), Edf2Mat.save()
            %           Edf2Mat.Events, Edf2Mat.Samples, Edf2Mat.Header
            %           Edf2Mat.about(), Edf2Mat.version()
            
            className = mfilename;
            fprintf('\n\n\t About the <a href="matlab:help %s">%s</a>:\n\n', className, className);
            fprintf('\t\tAuthor: \t%s\n', eval([className '.AUTHOR']));
            fprintf('\t\tE-Mail: \t<a href="mailto:%s">%s</a>\n', eval([className '.AUTHOREMAIL']), eval([className '.AUTHOREMAIL']));
            
            fprintf('\t\tCopyright:');
            fprintf('\t%s\n', eval([className '.COPYRIGHT{1}']));
            for i = 2 : size(eval([className '.COPYRIGHT']), 1)
                fprintf('\t\t\t\t\t%s\n', eval([className '.COPYRIGHT{i}']));
            end
            fprintf('\n');
            fprintf('\t\tVersion: \t%1.1f - %s\n', eval([className '.VERSION']), eval([className '.VERSIONDATE']));
            fprintf('\n');
            fprintf('\t\tLast revision: \t%s\n', eval([className '.CHANGELOG{1, 1}']));
            fprintf('\t\t\t\t\t\t-%s\n', eval([className '.CHANGELOG{1, 2}']));
            
            for i = 2 : size(eval([className '.CHANGELOG']), 1)
                fprintf('\t\t\t\t\t\t%s\n', eval([className '.CHANGELOG{i, 1}']));
                fprintf('\t\t\t\t\t\t-%s\n', eval([className '.CHANGELOG{i, 2}']));
            end
            fprintf('\n\n');
            
        end
    end
    
end

