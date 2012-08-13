function testeye(samples, et)

    % Author: Adrian Etter
    % Based on James Sulzer C-Daq Acquiring Code
    % email: adrian.etter@econ.uzh.ch
    % © SNS-Lab,
    % University of Zurich
    % Version 1.0 2011/September/29
    
    figure(1);
    hold on;
    h_old=plot(0,0); %plot handle
    axis([0 500 -500 0]);
    
    
    if ~exist('samples', 'var') || ~isnumeric(samples) || ~isreal(samples) 
        samples = 100; % samples to aquire
    end
        
%     voltages = nan(samples,1);
%     time = 1:samples;
    %handle = initAnalogChannel;
    %daq = DaqInterface(); % Initializes the daq card
   % daq = daq.setDevice('Dev1/ai0');
    tocs = nan(samples, 1);
    
    
    for i=1:samples;
        %[handle output(i)] = readAnalogChannel(handle);
        tic;
        %voltages(i) = daq.getVoltage();  % gets the voltage data
        [x, y] = et.getEyePosition();
%         disp(['x pos: ' num2str(x(2))]);
%         disp(['y pos: ' num2str(y(2))]);
        tocs(i) = toc;
        pause(0.05);
        h = plot(x(2), -1 * y(2), 'o');
        delete(h_old);
        h_old = h;
        drawnow();
    end
    %delete(daq); % clears the task on the daq card
    %terminateAnalogChannel(handle);

%     disp(['The voltage acquisition was performed with an average of: ' ...
%         num2str(mean(tocs)) ' seconds']);
% 
 end