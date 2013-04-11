classdef Edf2Mat < handle
    % Edf2Mat is a converter to convert Eyetracker data files to
    % Matlab file and perform some tasks on the data
    %
    % The new procedure uses code from SR-Research that returns all info of
    % the edf and not just part of it. The new routine is based on the work
    % of C. Kovach 2007 and is only for non-commercial use!
    %
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
    %   everything in the @folder & private folder is requiered,
    %   private/edf2asc.exe and private/processEvents.m, the mex files and
    %   the dll's/frameworks. On Mac the edfapi.framework must be copied to
    %   /Library/Framworks/ !!!Not the personal Library but to the root
    %   Library
    %
    % Other Classes required:
    %   no
    %
    %
    % See also: Edf2Mat.plot(), Edf2Mat.save()
    %           Edf2Mat.Events, Edf2Mat.Samples, Edf2Mat.Header          
    %           Edf2Mat.about(), Edf2Mat.version()
    %
    % <a href="matlab:Edf2Mat.about()">Copyright & Info</a>
    
    % CONSTANT AND HIDDEN VARIABLES
    properties (Constant, Hidden)
        HEADERTRASHLINES = 8;
    end
    
    % CONSTANT VARIABLES
    properties (Constant)            
        % Here we add the Version, Author, Copyright Info and the Change
        % Log
        AUTHOR = 'Adrian Etter'; % Author of the Class
        AUTHOREMAIL = 'adrian.etter@econ.uzh.ch'; % Email of the Author
        COPYRIGHT = {'© SNS-Lab'; 'University of Zurich'}; % Copyright Info
        VERSION = 1.5;  % Number of the latest Version
        VERSIONDATE = '2013/April/11' % Date of the latest Version
        CHANGELOG = {eval([mfilename '.VERSIONDATE']), 'Changed to mex instead of using edf.asc.exe'; ...
            '2012/Aug/10', 'Finished'; ...
            '2012/Aug/9', 'created'};
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
         Edf = struct();
                    
    end
    
    properties (Dependent)        
        samplesFilename;    % The file name of ASCII Files which stores all samples
        eventsFilename;     % The file name of ASCII Files which stores all events
        matFilename;        % The file name of MAT File which stores all Header, Samples and Events
    end
    
    % PUBLIC METHODS
    methods % Here come all the public functions
        function obj = Edf2Mat(filename, useOLDProcedure, verbose)
            assert(ispc || ismac, 'Edf2Mat:computer', 'This class is only available on windows or mac!');
            if ~exist('filename', 'var') || ~ischar(filename) || size(filename, 2) < 4 || ~strcmp(filename(end - 3:end), '.edf')
                throw(MException('EdfConverter:filename','Bad Filename, Filename must be given and be of type .edf!'));
            end
            
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
                        warning('Edf2Mat:computer', 'The old procedure is only available on Windows!');
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
            
            if obj.oldProcedure
                obj.convertSamples();
                obj.createHeader();
                obj.convertEvents();
                if obj.verbose
                    disp(['Edf succesfully converted, processed and values are saved in ' obj.matFilename '!']);
                end
            else
                obj.Edf     = edfimporter(obj.filename);
                obj.Header  = obj.Edf.HEADER;
                obj.Samples = obj.Edf.FSAMPLE;
                obj.Events  = obj.Edf.FEVENT;
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
        
        function convertSamples(obj)
            obj.convertFile(obj.cases.samples);
            obj.processSamples();
        end
        
        function convertEvents(obj)
            obj.convertFile(obj.cases.events);
            obj.processEvents();
        end
        
        function save(obj)
            % some how we need to make new copies to store them in a
            % file ...
            header  = obj.Header;
            samples = obj.Samples;
            events  = obj.Events;
            edf     = obj.Edf;
            thisobj = obj;
            vname   = @(x) inputname(1);
            builtin('save', obj.matFilename, vname(header), vname(samples), vname(events), vname(edf), vname(thisobj));
        end    
       
    end
    
    % PRIVATE METHODS
    methods (Access = private)       
        
        function convertFile(obj, kind)
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
        
        function createHeader(obj)
            
            lineNrs = strfind(obj.output, 'Processed');
            header  = obj.output(1 : lineNrs(1) - 1);
            header  = textscan(header, '%s', 'delimiter', sprintf('|'));            
            
            names = fieldnames(obj.Header);
            
            for i = 1 : size(names, 1)
                obj.Header.(names{i}) = strtrim(strrep(header{1}{obj.HEADERTRASHLINES + (i - 1) * 2}, [upper(names{i}) ': '], ''));
            end
            
            obj.Header.serial_number = strrep(obj.Header.serial_number, 'SERIAL NUMBER: ', '');
            
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
            
            fID = fopen(obj.samplesFilename, 'r');
            
            % Read ASCII-Samples File
            samples = textscan(fID, '%f %f %f %f %*s', 'delimiter', '\t', ...
                'EmptyValue', nan);            
            
            obj.Samples =  cell2struct(samples', fieldnames(obj.Samples));
            
            % Close ASCII-Samples File
            fclose(fID);            
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

