function plot(obj)
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

    screenSize = get(0,'ScreenSize');
    figure( 'Position', [screenSize(3)/4 screenSize(4)/4 2*screenSize(3)/3 2*screenSize(4)/3], ...
        'Name', ['Plotting ' obj.filename], ...
        'NumberTitle', 'off', ...
        'Menubar','none');
    
    
    posX           = obj.Samples.posX;
    % Y must be inverted, because eyetracker origin
    % is upper left corner in a graph its the lower left
    posY           = obj.Samples.posY * -1;
    
    time           = obj.Samples.time;
    messageTime    = obj.Events.Messages.time;
    pupilsize      = obj.Samples.pupilSize;
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
        time = min(blinkStart(1), messageTime(1)): max(blinkEnd(end), messageTime(end));
        blinkMarker = zeros(numel(time), 1);
        for i = 1:numel(blinkStart)
            blinkMarker((blinkStart(i) + 1:blinkEnd(i)) - blinkStart(1)) = 10;
        end
    end
    
    messageMarker = zeros(numel(time), 1);
    messageMarker(messageTime - messageTime(1) + 1) = 10;

    subplot(2,2,4);
    if ~exist('blinkMarker', 'var')
        plot(time, messageMarker);
        legend('Message Occurrence');
    else
    plot(time, messageMarker, time, blinkMarker);
    legend('Message Occurrence','Blink');
    end

end

