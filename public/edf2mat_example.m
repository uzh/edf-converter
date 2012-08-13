%% Example on how to use the Edf Converter

%% Converting the EDF File and saving it as a Matlab File
edf = Edf2Mat('eyedata.edf');

%% The edf Variable now holds all information
% lets display it:

disp(edf);

%% And how about just plot it?
plot(edf);

%% Of course you can also plot in your own style:
figure();
plot(edf.Samples.posX(end - 2000:end), edf.Samples.posY(end - 2000:end), 'o');




