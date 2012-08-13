function [Samples, Events, Header] = convert_edf2mat( filename )
%CONVERTER Summary of this function goes here
%   Detailed explanation goes here

    %% Input Validation 
    if isempty(filename) || ~ischar(filename) || size(filename, 2) < 4 || ~strcmp(filename(end - 3:end), '.edf')
        throw(MException('EdfConverter:filename','Bad Filename, Filename must be given and be of type .edf!'));
    end

    %% Convert EDF to ASCII File - only Samples
    disp('Trying to convert!')
    disp('Processing Samples. Please wait ...');
    [~, output] = dos(['edf2asc -s -miss nan -y ' filename]);

    if isempty(strfind(output, 'Converted successfully:'))
        throw(MException('EdfConverter:Edf2Asc',['Something went wrong, check log:\n' output]));
    end
    disp('Samples successfully converted!');
    
    %% Process Header
    proccessedStr = strfind(output, 'Processed');
    header = output(1 : proccessedStr(1) - 1);
    header = textscan(header, '%s', 'delimiter', sprintf('|'));
    Header = struct('date', [], ...
                'type', [], ...
                'version', [], ...
                'source', [], ...
                'system', [], ...
                'camera', [], ...
                'serial_number', [], ...
                'camera_config', [] ...
                 );
             
    names = fieldnames(Header);
    for i = 1 : size(names, 1)
        Header.(names{i}) = strtrim(strrep(header{1}{7 + (i - 1) * 2}, [upper(names{i}) ': '], ''));
    end
    Header.serial_number = strrep(Header.serial_number, 'SERIAL NUMBER: ', '');
             
    %% Move Samples File
    ascname = strrep(filename, '.edf', '.asc');
    samplesFilename = strrep(ascname, '.asc', '_samples.asc');
    movefile(ascname, samplesFilename, 'f');
    
    %% Convert EDF to ASCII File - only Events
    disp('Processing Events. Please wait ...');
    [~, output] = dos(['edf2asc  -e -t -miss nan -y ' filename]);
    
    if isempty(strfind(output, 'Converted successfully:'))
        throw(MException('EdfConverter:Edf2Asc',['Something went wrong, check log:\n' output]));
    end
    disp('Events successfully converted!');
    %% Move Events File
    eventsFilename = strrep(ascname, '.asc', '_events.asc');
    movefile(ascname, eventsFilename, 'f');
    
    
    %% PROCESS SAMPLES
    %% Open ASCII-Samples File
    fID = fopen(samplesFilename, 'r');
    
    %% Read ASCII-Samples File    
    samples = textscan(fID, '%f %f %f %f %*s', 'delimiter', '\t', ...
        'EmptyValue', nan);
    
    fields = {'time', 'posX', 'posY', 'pupilSize'};
    Samples =  cell2struct(samples', fields);
    
    %% Close ASCII-Samples File    
    fclose(fID);    
    
    %% PROCESS EVENTS
    %% Open ASCII-Events File
    fID = fopen(eventsFilename, 'r');
    
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
    prescaler   = nan(sum(cell2mat(strfind(lines, 'PRESCALER'))), 1);
    vprescaler  = nan(sum(cell2mat(strfind(lines, 'VPRESCALER'))), 1);
    pupilInfo   = cell(sum(cell2mat(strfind(lines, 'PUPIL'))), 1);
    Sfix        = struct('eye', [], 'time', []);
    Efix        = struct('eye', [], 'start', [], 'end', [], 'duration', [], ...
                            'posX', [], 'posY', [], 'pupilSize', []);
    Ssacc       = struct('eye', [], 'time', []);
    Esacc       = struct('eye', [], 'start', [], 'end', [], 'duration', [], ...
                        'posX', [], 'posY', [], 'posXend', [], 'posYend', [], ...
                        'value1', [], 'value2', []);
    Sblink      = struct('eye', [], 'time', []);
    Eblink      = struct('eye', [], 'start', [], 'end', [], 'duration', []);
    
    %% Allocate Memory for our structures:
    Msg         = repmat(Msg, sum(cell2mat(strfind(lines, 'MSG'))), 1);
    Start       = repmat(Start, sum(cell2mat(strfind(lines, 'START'))), 1);
    Sfix        = repmat(Sfix, sum(cell2mat(strfind(lines, 'SFIX'))), 1);
    Efix        = repmat(Efix, sum(cell2mat(strfind(lines, 'EFIX'))), 1);
    Ssacc       = repmat(Ssacc, sum(cell2mat(strfind(lines, 'SSACC'))), 1);
    Esacc       = repmat(Esacc, sum(cell2mat(strfind(lines, 'ESACC'))), 1);
    Sblink      = repmat(Sblink, sum(cell2mat(strfind(lines, 'SBLINK'))), 1);
    Eblink      = repmat(Eblink, sum(cell2mat(strfind(lines, 'EBLINK'))), 1);
    End         = repmat(End, sum(cell2mat(strfind(lines, 'END'))), 1);
    
    %% Process all possible Events
    nMsg        = 1;
    nStart      = 1;
    nEnd        = 1;
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
                Start(nStart).info          = lines{ row + 2};
                nStart                      = nStart + 1;
                row                         = row + 4;
            case 'PRESCALER'
                prescaler(nPrescaler)       = str2double(lines{ row + 1});
                nPrescaler                  = nPrescaler + 1;
                row                         = row + 2;
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
                Esacc(nEsacc).value1        = str2double(lines{ row + 9});
                Esacc(nEsacc).value2        = str2double(lines{ row + 10});  
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
            otherwise
                disp(row);
                disp('Unknown Event, please send edf file to the author of this script!');
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
    Esacc(1).value1         = [Esacc.value1];
    Esacc(1).value2         = [Esacc.value2];
    Esacc                   = Esacc(1);
    
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

    
    %% Create the Event Structure 
    Events.Messages         = Msg;       
    Events.Start            = Start;
    Events.prescaler        = prescaler;
    Events.vprescaler       = vprescaler;
    Events.pupilInfo        = pupilInfo;
    Events.Sfix             = Sfix;
    Events.Efix             = Efix;
    Events.Ssacc            = Ssacc;
    Events.Esacc            = Esacc;
    Events.Sblink           = Sblink;
    Events.Eblink           = Eblink;
    Events.End              = End;
    

    
     
    
    %% SAVE TO MAT FILE
    matfilename = strrep(filename, '.edf', '');
    save(matfilename, 'Header', 'Samples', 'Events');
    disp(['File succesfully converted and ' matfilename '.mat created!']);

    


end

