function processEvents(obj)
%PROCESSEVENTS Summary of this function goes here
%   Detailed explanation goes here

    if obj.oldProcedure
        fID = fopen(obj.eventsFilename, 'r');

        %% Read ASCII-Events File - split all up in single lines
        lines = textscan(fID, '%s', 'delimiter', '\t',  'EmptyValue', nan, ...
            'commentStyle', '**', 'HeaderLines', 11);

        %% Close ASCII-Events File
        fclose(fID);

        %% Flatten lines
        lines = lines{1};

        %% We have some structs that we have to define:        
        Msg         = struct('time', [], 'info', []);
        Start       = struct('time', [], 'eye', [], 'info', []);
        End         = struct('time', [], 'info', [], 'info2', [], 'value1', [], 'value2', []);
        prescaler   = nan(sum(cell2mat(strfind(lines, 'PRESCALER')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        vprescaler  = nan(sum(cell2mat(strfind(lines, 'VPRESCALER')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        pupilInfo   = cell(sum(cell2mat(strfind(lines, 'PUPIL')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Sfix        = struct('eye', [], 'time', []);
        Efix        = struct('eye', [], 'start', [], 'end', [], 'duration', [], ...
            'posX', [], 'posY', [], 'pupilSize', []);
        Ssacc       = struct('eye', [], 'time', []);
        Esacc       = struct('eye', [], 'start', [], 'end', [], 'duration', [], ...
            'posX', [], 'posY', [], 'posXend', [], 'posYend', [], ...
            'hypot', [], 'pvel', []);
        Sblink      = struct('eye', [], 'time', []);
        Eblink      = struct('eye', [], 'start', [], 'end', [], 'duration', []);
        Input       = struct('time', [], 'value', []);

        %% Allocate Memory for our structures:
        Msg         = repmat(Msg, sum(cell2mat(strfind(lines, 'MSG')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Start       = repmat(Start, sum(cell2mat(strfind(lines, 'START')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Sfix        = repmat(Sfix, sum(cell2mat(strfind(lines, 'SFIX')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Efix        = repmat(Efix, sum(cell2mat(strfind(lines, 'EFIX')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Ssacc       = repmat(Ssacc, sum(cell2mat(strfind(lines, 'SSACC')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Esacc       = repmat(Esacc, sum(cell2mat(strfind(lines, 'ESACC')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Sblink      = repmat(Sblink, sum(cell2mat(strfind(lines, 'SBLINK')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Eblink      = repmat(Eblink, sum(cell2mat(strfind(lines, 'EBLINK')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        End         = repmat(End, sum(cell2mat(strfind(lines, 'END')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag
        Input       = repmat(Input, sum(cell2mat(strfind(lines, 'INPUT')) == 1), 1); % == 1 because we want only to find lines, where the line start with the specified tag

        %% Process all possible Events
        nMsg        = 1;
        nStart      = 1;
        nEnd        = 1;
        nInput      = 1;
        nPrescaler  = 1;
        nVprescaler = 1;
        nPupil      = 1;
        nSfix       = 1;
        nEfix       = 1;
        nSsacc      = 1;
        nEsacc      = 1;
        nSblink     = 1;
        nEblink     = 1;

        row         = 1;

        while row < size(lines, 1)
            switch lines{row}
                case 'MSG'
                    Msg(nMsg).time              = str2double(lines{ row + 1});
                    Msg(nMsg).info              = lines{ row + 2};
                    nMsg                        = nMsg + 1;
                    row                         = row + 3;
                case 'START'
                    Start(nStart).time          = str2double(lines{ row + 1});
                    Start(nStart).eye           = lines{ row + 2};
                    Start(nStart).info          = lines{ row + 2}; % unnecessary, will be removed in the next version
                    nStart                      = nStart + 1;
                    row                         = row + 4;
                case 'PRESCALER'
                    prescaler(nPrescaler)       = str2double(lines{ row + 1});
                    nPrescaler                  = nPrescaler + 1;
                    row                         = row + 2;
                case 'INPUT'
                    Input(nInput).time          = str2double(lines{ row + 1});
                    Input(nInput).value         = str2double(lines{ row + 2});
                    nInput                      = nInput + 1;
                    row                         = row + 3;
                case 'VPRESCALER'
                    vprescaler(nVprescaler)     = str2double(lines{ row + 1});
                    nVprescaler                 = nVprescaler + 1;
                    row                         = row + 2;
                case 'PUPIL'
                    pupilInfo{nPupil}           = lines{ row + 1};
                    nPupil                      = nPupil + 1;
                    row                         = row + 2;
                case 'EVENTS'
                    % ignore %
                    row                         = row + 9;
                case 'SFIX'
                    Sfix(nSfix).eye             = lines{ row + 1};
                    Sfix(nSfix).time            = str2double(lines{ row + 2});
                    nSfix                       = nSfix + 1;
                    row                         = row + 3;
                case 'EFIX'
                    Efix(nEfix).eye             = lines{ row + 1};
                    Efix(nEfix).start           = str2double(lines{ row + 2});
                    Efix(nEfix).end             = str2double(lines{ row + 3});
                    Efix(nEfix).duration        = str2double(lines{ row + 4});
                    Efix(nEfix).posX            = str2double(lines{ row + 5});
                    Efix(nEfix).posY            = str2double(lines{ row + 6});
                    Efix(nEfix).pupilSize       = str2double(lines{ row + 7});
                    nEfix                       = nEfix + 1;
                    row                         = row + 8;
                case 'SSACC'
                    Ssacc(nSsacc).eye           = lines{ row + 1};
                    Ssacc(nSsacc).time          = str2double(lines{ row + 2});
                    nSsacc                      = nSsacc + 1;
                    row                         = row + 3;
                case 'ESACC'
                    Esacc(nEsacc).eye           = lines{ row + 1};
                    Esacc(nEsacc).start         = str2double(lines{ row + 2});
                    Esacc(nEsacc).end           = str2double(lines{ row + 3});
                    Esacc(nEsacc).duration      = str2double(lines{ row + 4});
                    Esacc(nEsacc).posX          = str2double(lines{ row + 5});
                    Esacc(nEsacc).posY          = str2double(lines{ row + 6});
                    Esacc(nEsacc).posXend       = str2double(lines{ row + 7});
                    Esacc(nEsacc).posYend       = str2double(lines{ row + 8});
                    Esacc(nEsacc).hypot         = str2double(lines{ row + 9});
                    Esacc(nEsacc).pvel          = str2double(lines{ row + 10});
                    nEsacc                      = nEsacc + 1;
                    row                         = row + 11;
                case 'SBLINK'
                    Sblink(nSblink).eye         = lines{ row + 1};
                    Sblink(nSblink).time        = str2double(lines{ row + 2});
                    nSblink                     = nSblink + 1;
                    row                         = row + 3;
                case 'EBLINK'
                    Eblink(nEblink).eye         = lines{ row + 1};
                    Eblink(nEblink).start       = str2double(lines{ row + 2});
                    Eblink(nEblink).end         = str2double(lines{ row + 3});
                    Eblink(nEblink).duration    = str2double(lines{ row + 4});
                    nEblink                     = nEblink + 1;
                    row                         = row + 5;
                case 'END'
                    End(nEnd).time              = str2double(lines{row + 1});
                    End(nEnd).info              = lines{ row + 2};
                    End(nEnd).info2             = lines{ row + 3};
                    End(nEnd).value1            = str2double(lines{ row + 4});
                    End(nEnd).value2            = str2double(lines{ row + 5});
                    nEblink                     = nEblink + 1;
                    row                         = row + 6;
                case ''
                    % Todo get Calibration data%
                    row = row + 1;
                otherwise

                    if strfind(lines{row}, '>>>>>>> CALIBRATION')
                        % ignore
                    else
                        coordinates = [];
                        try
                            coordinates = sscanf(lines{row}, '%d %d %d %d');
                        catch e
                            if isa(e, 'MException')
                            else
                                rethrow(e);
                            end
                        end
                        if isempty(coordinates);
                            disp(row);
                            disp(['Unknown Event, please send edf file to ' obj.AUTHOREMAIL ' of this script!']);
                        end
                    end

                    row = row + 1;
            end
        end

        %% Reshape the structures to more useful variables

        Msg(1).time             = [Msg.time];
        Msg(1).info             = {Msg.info};
        Msg                     = Msg(1);

        Start(1).time           = [Start.time];
        Start(1).eye            = {Start.eye};
        Start(1).info           = {Start.info};
        Start                   = Start(1);

        Sfix(1).eye             = {Sfix.eye};
        Sfix(1).time            = [Sfix.time];
        Sfix                    = Sfix(1);

        Efix(1).eye             = {Efix.eye};
        Efix(1).start           = [Efix.start];
        Efix(1).end             = [Efix.end];
        Efix(1).duration        = [Efix.duration];
        Efix(1).posX            = [Efix.posX];
        Efix(1).posY            = [Efix.posY];
        Efix(1).pupilSize       = [Efix.pupilSize];
        Efix                    = Efix(1);

        Ssacc(1).eye            = {Ssacc.eye};
        Ssacc(1).time           = [Ssacc.time];
        Ssacc                   = Ssacc(1);

        Esacc(1).eye            = {Esacc.eye};
        Esacc(1).start          = [Esacc.start];
        Esacc(1).end            = [Esacc.end];
        Esacc(1).duration       = [Esacc.duration];
        Esacc(1).posX           = [Esacc.posX];
        Esacc(1).posY           = [Esacc.posY];
        Esacc(1).posXend        = [Esacc.posXend];
        Esacc(1).posYend        = [Esacc.posYend];
        Esacc(1).hypot          = [Esacc.hypot];
        Esacc(1).pvel           = [Esacc.pvel];
        
        Esacc                   = Esacc(1);
        % Keep previouse unknown variables
        Esacc.value1            = Esacc.hypot;
        Esacc.value2            = Esacc.pvel;

        Sblink(1).eye           = {Sblink.eye};
        Sblink(1).time          = [Sblink.time];
        Sblink                  = Sblink(1);

        Eblink(1).eye           = {Eblink.eye};
        Eblink(1).start         = [Eblink.start];
        Eblink(1).end           = [Eblink.end];
        Eblink(1).duration      = [Eblink.duration];
        Eblink                  = Eblink(1);

        End(1).time             = [End.time];
        End(1).info             = {End.info};
        End(1).info2            = {End.info2};
        End(1).value1           = [End.value1];
        End(1).value2           = [End.value2];
        End                     = End(1);

    else % create old header elements for backward compatibility
        eyeNames                = fieldnames(obj.EYES);
        
        Messages                = obj.Edf.FEVENT([obj.Edf.FEVENT.type].' == obj.EVENT_TYPES.MESSAGEEVENT);
        Msg.time                = double([Messages.sttime]);
        Msg.info                = {Messages.message};

%       Start of recording
        startRecordings         = obj.Edf.RECORDINGS([obj.Edf.RECORDINGS.state].' == obj.RECORDING_STATES.START);
        Start.time              = double([startRecordings.time]);
        eyes                    = [startRecordings.eye]; % here not plus one as SR_RESEARCH can't follow it's own convention and start in recording with 1 whereas in events with 0!!!!
        
        Start.eye               =  eyeNames(eyes).';
        Start.info              =  eyeNames(eyes).';
        
        % prescaler
        prescaler               = 1; % Undocummented but from C-Code its always 1! print("PRESCALER\t1\n");
        
        % vprescaler
        vprescaler              = 1; % Undocummented but from C-Code its always 1! print("PRESCALER\t1\n");
        
        % Pupil Info        
        pupilTypeNames          = fieldnames(obj.PUPIL);
        pupilInfo               = pupilTypeNames(double(startRecordings.pupil_type + 1));
% 
        startfix                = obj.Edf.FEVENT([obj.Edf.FEVENT.type].' == obj.EVENT_TYPES.STARTFIX);
        Sfix.eye                = eyeNames(double([startfix.eye]) + 1).'; % + 1 because in c indexing start with 0 whereas in matlab with 1
        Sfix.time               = double([startfix.sttime]);
% 
        endfix                  = obj.Edf.FEVENT([obj.Edf.FEVENT.type].' == obj.EVENT_TYPES.ENDFIX);
        Efix.eye                = eyeNames(double([endfix.eye]) + 1).'; % + 1 because in c indexing start with 0 whereas in matlab with 1
        Efix.start              = double([endfix.sttime]);
        Efix.end                = double([endfix.entime]);
        Efix.duration           = Efix.end - Efix.start;
        Efix.posX               = double([endfix.gavx]);
        Efix.posY               = double([endfix.gavy]);
        Efix.pupilSize          = double([endfix.ava]);

% 
        startSaccade            = obj.Edf.FEVENT([obj.Edf.FEVENT.type].' == obj.EVENT_TYPES.STARTSACC);
        Ssacc.eye               = eyeNames(double([startSaccade.eye]) + 1).'; % + 1 because in c indexing start with 0 whereas in matlab with 1
        Ssacc.time              = double([startSaccade.sttime]);
% 
        endSaccade              = obj.Edf.FEVENT([obj.Edf.FEVENT.type].' == obj.EVENT_TYPES.ENDSACC);
        Esacc.eye               = eyeNames(double([endSaccade.eye]) + 1).'; % + 1 because in c indexing start with 0 whereas in matlab with 1
        Esacc.start             = double([endSaccade.sttime]); % == double([startSaccade.sttime])!!!!!!
        Esacc.end               = double([endSaccade.entime]); % != double([startSaccade.entime]) => 0 !!!
        Esacc.duration          = Esacc.end - Esacc.start;
        Esacc.posX              = double([endSaccade.gstx]);
        Esacc.posY              = double([endSaccade.gsty]);
        Esacc.posXend           = double([endSaccade.genx]);
        Esacc.posYend           = double([endSaccade.geny]);
        
        Esacc.hypot             = hypot((double([endSaccade.gstx]) - double([endSaccade.genx])) ...
                                          ./((double([endSaccade.supd_x]) + double([endSaccade.eupd_x]))/2.0), ...
                                        (double([endSaccade.gsty]) - double([endSaccade.geny])) ...
                                          ./((double([endSaccade.supd_y]) + double([endSaccade.eupd_y]))/2.0)); % Hypotenouse of something ... ????
        
        Esacc.pvel              = double([endSaccade.pvel]);
        % Keep old namings after figuring out where they come from
        Esacc.value1            = Esacc.hypot;
        Esacc.value2            = Esacc.pvel;
        

% 
        startBlink              = obj.Edf.FEVENT([obj.Edf.FEVENT.type].' == obj.EVENT_TYPES.STARTBLINK);
        Sblink.eye              = eyeNames(double([startBlink.eye]) + 1).'; % + 1 because in c indexing start with 0 whereas in matlab with 1
        Sblink.time             = double([startBlink.sttime]);

% 
        endBlink                = obj.Edf.FEVENT([obj.Edf.FEVENT.type].' == obj.EVENT_TYPES.ENDBLINK);
        Eblink.eye              = eyeNames(double([endBlink.eye]) + 1).'; % + 1 because in c indexing start with 0 whereas in matlab with 1
        Eblink.start            = double([endBlink.sttime]); % == double([startSaccade.sttime])!!!!!!
        Eblink.end              = double([endBlink.entime]); % != double([startSaccade.entime]) => 0 !!!
        Eblink.duration         = Eblink.end - Eblink.start;
% 

        endRecordings           = obj.Edf.RECORDINGS([obj.Edf.RECORDINGS.state].' == obj.RECORDING_STATES.END);
        End.time                = [endRecordings.time];
        End.info                = {'EVENTS'};
        End.info2               = {'RES'};
        End.ppd_x_ppd_total     = sum(obj.Samples.rx)/numel(obj.Samples.rx); % What does it significate? Sum of all screen pixels per degree divided by number of samples => What does it tell?
        End.ppd_y_ppd_total     = sum(obj.Samples.ry)/numel(obj.Samples.ry); % What does it significate? Sum of all screen pixels per degree divided by number of samples => What does it tell?
        End.value1              = End.ppd_x_ppd_total;
        End.value2              = End.ppd_x_ppd_total;

    end


    %% Create the Event Structure
    obj.Events.Messages         = Msg;
    obj.Events.Start            = Start;
    obj.Events.prescaler        = prescaler;
    obj.Events.vprescaler       = vprescaler;
    obj.Events.pupilInfo        = pupilInfo;
    obj.Events.Sfix             = Sfix;
    obj.Events.Ssacc            = Ssacc;
    obj.Events.Esacc            = Esacc;
    obj.Events.Efix             = Efix;    
    obj.Events.Sblink           = Sblink;
    obj.Events.Eblink           = Eblink;
    obj.Events.End              = End;
end

