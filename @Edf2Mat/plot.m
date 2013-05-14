function plot(obj, startIdx, endIdx)
%PLOT plots the edf content to verify if they are useful. For all other
%   post processing you should rather define your own plots
%
%   Syntax: obj.plot();
%           or
%           plot(obj);
%clear
%   See also: Edf2Mat.plot(), Edf2Mat.save()
%             Edf2Mat.Events, Edf2Mat.Samples, Edf2Mat.Header
%             Edf2Mat.about(), Edf2Mat.version()
%
%   <a href = "matlab:Edf2Mat.about">About and Copyright</a>

% AVAILABLE Types: see edf_data.h
% just a few examples:
% #define STARTPARSE 1 	/* these only have time and eye data */
% #define ENDPARSE   2
% #define BREAKPARSE 10
% 
% 			/* EYE DATA: contents determined by evt_data */
% #define STARTBLINK 3    /* and by "read" data item */
% #define ENDBLINK   4    /* all use IEVENT format */
% #define STARTSACC  5
% #define ENDSACC    6
% #define STARTFIX   7
% #define ENDFIX     8
% #define FIXUPDATE  9
% 
%   /* buffer = (none, directly affects state), btype = CONTROL_BUFFER */
% 
% 			 /* control events: all put data into */
% 			 /* the EDF_FILE or ILINKDATA status  */
% #define STARTSAMPLES 15  /* start of events in block */
% #define ENDSAMPLES   16  /* end of samples in block */
% #define STARTEVENTS  17  /* start of events in block */
% #define ENDEVENTS    18  /* end of events in block */
% 
% 
% 
% 	/* buffer = IMESSAGE, btype = IMESSAGE_BUFFER */
% 
% #define MESSAGEEVENT 24  /* user-definable text or data */
% 
% 
% 	/* buffer = IOEVENT, btype = IOEVENT_BUFFER */
% 
% #define BUTTONEVENT  25  /* button state change */
% #define INPUTEVENT   28  /* change of input port */

    assert(isa(obj, 'Edf2Mat'), 'Edf2Mat:plot', 'Only objects of type Edf2Mat can be plotted!');
    
    if ~exist('startIdx', 'var')
        startIdx = 1;
    end
    
    if ~exist('endIdx', 'var')
        endIdx = numel(obj.Samples.posX);
    end
    if endIdx == startIdx
        warning('Edf2Mat:plot:range','Start Index == End Index, nothing do be plotted');
        return;
    end

    screenSize = get(0,'ScreenSize');
    figure( 'Position', [screenSize(3)/4 screenSize(4)/4 2*screenSize(3)/3 2*screenSize(4)/3], ...
        'Name', ['Plotting ' obj.filename], ...
        'NumberTitle', 'off', ...
        'Menubar','none');
    
    
    posX           = obj.Samples.posX(startIdx:endIdx);
    % Y must be inverted, because eyetracker origin
    % is upper left corner in a graph its the lower left
    posY           = obj.Samples.posY(startIdx:endIdx) * -1;
    
    time           = obj.Samples.time(startIdx:endIdx);
    evt_time       = double(obj.RawEdf.RECORDINGS(1).time:obj.RawEdf.RECORDINGS(end).time);
    evt_start      = evt_time(1);
    evt_time       = evt_time - evt_start;
    messageTime    = obj.Events.Messages.time;
    pupilsize      = obj.Samples.pupilSize(startIdx:endIdx);
    blinkStart     = obj.Events.Eblink.start';
    blinkEnd       = obj.Events.Eblink.end';
    
    time = unique(time - time(1));
    time(time < 0) = 0;
    
    subplot(2,2,[1 3]);

    plot(posX, posY, 'o', 'Color','blue'); 
    title('Plot of the eye movement!');
    axis([min(posX) max(posX) min(posY) max(posY)]);
    axis('square');

    xlabel('x-Position');
    ylabel('y-Position');

    % Ploting pupil size
    
    %
    subplot(2,2,2);
    plot(time, pupilsize); 
    title('Pupil Size');
    axis('auto');

    xlabel('time [ms]');

    % Ploting some Event Info
    if ~isempty(blinkStart)
        blinkEnd    = blinkEnd - evt_start;
        blinkStart  = blinkStart - evt_start;            
        
        blinkStart  = ismember(evt_time, blinkStart.').';
        blinkEnd    = ismember(evt_time, blinkEnd.').';
        idx         = 1:length(evt_time);
        idx1        = idx(blinkStart);
        idx2        = idx(blinkEnd);
        idx         = arrayfun(@(x,y) (x:y).', idx1, idx2,  'UniformOutput', false);
        idx         = vertcat(idx{:});
        
        blink_evt       = zeros(numel(evt_time), 1);
        blink_evt(idx)  = 100;        
    end
    
    if ~isempty(messageTime)
        msgEvtTime      = unique(messageTime - evt_start).';
        msg_evt_time        = unique([evt_time(:).', msgEvtTime(:).']);
        messageMarker   = ismember(msg_evt_time, msgEvtTime);
        idx             = 1:length(msg_evt_time);
        idx             = idx(messageMarker);
        msg_evt         = nan(numel(msg_evt_time), 1);
        msg_evt(idx)    = 100;
    end

    subplot(2,2,4);
    legendString = {};
    if exist('blink_evt', 'var')
        plot(evt_time(startIdx:endIdx), blink_evt(startIdx:endIdx), 'r-', 'DisplayName','Blinks');
        legendString{end+1, 1} = 'Blinks';
        hold on;
    end
    if exist('msg_evt', 'var')
        plot(msg_evt_time(startIdx:endIdx), msg_evt(startIdx:endIdx), 'bo', 'DisplayName','Message Occurrence');
        legendString{end+1, 1} = 'Message Occurrence';        
    end
    legend(legendString{:});
    hold off;

end

