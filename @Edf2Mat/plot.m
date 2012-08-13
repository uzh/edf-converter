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

    screenSize = get(0,'ScreenSize');
    figure( 'Position', [screenSize(3)/4 screenSize(4)/4 2*screenSize(3)/3 2*screenSize(4)/3], ...
            'Name', ['Plotting ' obj.filename], ...
            'NumberTitle', 'off', ...
             'Menubar','none');


    % Ploting the Eye Movement
    posX = obj.Samples.posX;
    % Y must be inverted, because eyetracker origin 
    % is upper left corner in a graph its the lower left
    posY = obj.Samples.posY * -1; 
    subplot(2,2,[1 3]);

    plot(posX, posY, 'o', 'Color','blue'); 
    title('Plot of the eye movement!');
    axis([min(posX) max(posX) min(posY) max(posY)]);
    axis('square');

    xlabel('x-Position');
    ylabel('y-Position');

    % Ploting pupil size
    time = obj.Samples.time;
    pupilsize = obj.Samples.pupilSize;
    %
    subplot(2,2,2);
    plot(time, pupilsize); 
    title('Pupil Size');
    axis('auto');

    xlabel('time [ms]');
    ylabel(lower(obj.Events.pupilInfo));

    % Ploting some Event Info
    messageTime     = obj.Events.Messages.time;
    time            = obj.Samples.time;
    time            = [(messageTime(1):1:time(1))'; time];
    messageMarker   = zeros(size(time, 1), 1);
    messageMarker(ismember(time, messageTime)) = 10;



    blinkStart      = obj.Events.Eblink.start';
    blinkEnd        = obj.Events.Eblink.end';
    blink = [];
    for i = 1:size(blinkStart, 1)
        blink = [blink; (blinkStart(i):4:blinkEnd(i))'];
    end
    blinkMarker     = zeros(size(time, 1), 1);
    blinkMarker(ismember(time, blink)) = 10;

    subplot(2,2,4);
    plot(time, messageMarker, time, blinkMarker);
    legend('Message Occurence','Blink');

end

