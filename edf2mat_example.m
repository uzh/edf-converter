%% Example 1 on how to use the Edf Converter

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




%% Example 2 on how to use the Edf Converter

%% Converting the EDF File and saving it as a Matlab File
edf = Edf2Mat('fMRI_Results_sub_025_270712EYE25r1.edf');

%% The edf Variable now holds all information
% lets display it:

disp(edf);

%% And how about just plot it?
plot(edf);

%% Of course you can also plot in your own style:
figure();
plot(edf.Samples.posX(end - 2000:end), edf.Samples.posY(end - 2000:end), 'o');