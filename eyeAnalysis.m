function output = eyeAnalysis(file_with_path)

 

%set path to Adi's toolbox

addpath('/Volumes/DataHD/Dropbox/work/teaching/spm/2013_Zurich_internal/session_10_08052013/eye/Edf2Mat_Toolbox');

 

% read in data

edf = Edf2Mat(file_with_path);

 


%% set first timestamp to zero

edfTime = (edf.Samples.time - edf.Samples.time(1)).'; %%% first entry is now t=0: start of experiment%%%

 

%% determine temporal resolution

edfTimeStepRes = edfTime(2) - edfTime(1);

 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% determine saccade parameters
SACCADE_START_TYPE = 5;
% saccadeStarts = edf.Events([edf.Events.type]' == 5) 

saccStarts     = double([edf.Events([edf.Events.type].' == SACCADE_START_TYPE).sttime].');
% saccStarts = saccadeStarts.sttime;


% Sacc.Ons  = saccStarts - edf.Events.Start.time;
START_PHASE_TYPE = 1;
start_time = edf.Events([edf.Events.type] == START_PHASE_TYPE);
start_time = double(start_time(1).sttime); % (1) if multiple start
Sacc.Ons  = saccStarts - start_time; 


 
SACCADE_END_TYPE = 6;
% saccadeStops = edf.Events([edf.Events.type]' == 5) 

% saccStops = saccadeStops.sttime;
saccStops   = double([edf.Events([edf.Events.type].' == SACCADE_END_TYPE).sttime].');

% Sacc.Ends = saccStops - edf.Events.Start.time;
Sacc.Ends = saccStops - start_time;

 

Sacc.Dur  = Sacc.Ends - Sacc.Ons;

posX = [edf.Events([edf.Events.type].' == SACCADE_END_TYPE).gstx];
posXend  = [edf.Events([edf.Events.type].' == SACCADE_END_TYPE).genx]
posY = [edf.Events([edf.Events.type].' == SACCADE_END_TYPE).gsty];
posYend  = [edf.Events([edf.Events.type].' == SACCADE_END_TYPE).geny];

% Sacc.EuclDist = sqrt([edf.Events.Esacc.posX(:) - edf.Events.Esacc.posXend(:)].^2 + [edf.Events.Esacc.posY(:) - edf.Events.Esacc.posYend(:)].^2);

Sacc.EuclDist = sqrt([posX(:) - posXend(:)].^2 + [posY(:) - posYend(:)].^2)

Sacc.Speed = Sacc.EuclDist ./Sacc.Dur;

% this could also be parameteric modulator

%% Other possible variables are absolute position

 

output.Sacc.Ons = Sacc.Ons %this one goes into SPM as event onset

output.Sacc.Size = Sacc.EuclDist % this could be paramteric modulator

output.Sacc.Speed = Sacc.Speed; % this one could also be parametric modulator

% possible other variables are X and Y position of eye

 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% determine blink parameters
BLINKSTARTTYPE = 3; % see edf_data.h to have an overview of available types
        BLINKENDTYPE   = 4;
blinkStart     = double([edf.Events([edf.Events.type].' == BLINKSTARTTYPE).sttime].');
blinkEnd       = double([edf.Events([edf.Events.type].' == BLINKENDTYPE).entime].');

Blink.Ons  = blinkStart - start_time;

Blink.Ends = blinkEnd - start_time;

Blink.Dur  = Blink.Ends - Blink.Ons; 

% 
% Blink.Ons  = edf.Events.Eblink.start - edf.Events.Start.time;
% 
% Blink.Ends = edf.Events.Eblink.end - edf.Events.Start.time;
% 
% Blink.Dur  = BlinkEnds - BlinkOns; 

 

%% determine improbable signal dropouts

% you need to play with these numbers based on the literature

blinkBelow80msID = Blink.Dur < 80; 

blinkAbove2000msID = Blink.Dur > 2000;

 

%% Remove improbable blinks

Blink.OnsCorr = Blink.Ons(~blinkBelow80msID & ~blinkAbove2000msID);

 

output.Blink.Ons = Blink.OnsCorr; %this one goes into SPM as event onset

output.Blink.Dur = Blink.Dur; % this could be a parametric modulator

 

%%%%%%%%%%%%%%%%%%%%%

%% derive pupil signal

% first we need to interpolate across blinks

% Pupil.sizeRaw = edf.Samples.pupilSize;
Pupil.sizeRaw = edf.Samples.pa(2,:).';

Pupil.sizeFiltered = Pupil.sizeRaw;

 

for iBlinks = 1:Blink.Ons

    samplesBeforeBlink = min([3 Blink.Ons/edfTimeStepRes])

    Blink.startInd(iBlinks) = find(edfTime == Blink.Ons(iBlinks)) - samplesBeforeBlink;

    samplesAfterBlink = min([3 (edfTime(end)-Blink.Ends)/edfTimeStepRes])

    Blink.stopInd(iBlinks) = find(edfTime == Blink.Ons(iBlinks)) + samplesAfterBlink;

    Blink.OnsPupil(iBlinks) = Pupil.sizeRaw(Blink.startInd(iBlinks));

    Blink.EndsPupil(iBlinks) = Pupil.sizeRaw(Blink.stopInd(iBlinks));

    pupilInterp = linspace(Blink.OnsPupil(iBlinks),Blink.EndsPupil(iBlinks),Blink.Dur/edfTimeStepRes);

    Pupil.sizeFiltered(Blink.startInd(iBlinks):Blink.stopInd(iBlinks)) = pupilInterp;

end

 

% build pupil regressor

% crop pupil signal 

TR = 2.6; microTimeRes = 54; volNr = 147;

microTimeBINS = linspace(0, TR*volNr, microTimeRes*volNr);

Pupil.sizeFiltered = Pupil.sizeFiltered(Pupil.sizeFiltered<=microTimeBINS(end));

 

P = length(Pupil.sizeFiltered);

Q = noVolumes*microTimeRes;

 

Pupil.sizeFilteredfMRI = resample(Pupil.sizeFiltered,P,Q);

 

output.Pupil.RegBase = microTimeBINS; % to have a bse for parametric modulation

output.Pupil.Size = Pupil.sizeFilteredfMRI % This one needs to go in as parameteric regressor

 

 end
