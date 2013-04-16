%% Type help for help
help Edf2Mat

%% Example 1 for how to use the Edf Converter

%% Converting the EDF File and saving it as a Matlab File
edf0 = Edf2Mat('eyedata.edf');

%% The edf Variable now holds all information
% lets display it:

disp(edf0);

%% And how about just plot it?
plot(edf0);

%% Of course you can also plot in your own style:
figure();
plot(edf0.Samples.posX(end - 2000:end), edf0.Samples.posY(end - 2000:end), 'o');




%% Example 2 for how to use the Edf Converter

%% Converting the EDF File and saving it as a Matlab File
edf1 = Edf2Mat('fMRI_Results_sub_025_270712EYE25r1.edf');

%% The edf Variable now holds all information
% lets display it:

disp(edf1);

%% And how about just plot it?
plot(edf1);

%% Of course you can also plot in your own style:
figure();
plot(edf1.Samples.posX(end - 2000:end), edf1.Samples.posY(end - 2000:end), 'o');

%% Plot the progress of the pupil size

figure();
plot(edf1.Samples.pa(2, end - 500:end));

%% Example 2 for how to use the Edf Converter

%% Converting the EDF File and saving it as a Matlab File
edf2 = Edf2Mat('Pill004.edf');

%% Plot the progress of the pupil size

figure();
plot(edf2.Samples.pa(2, 1:1500));