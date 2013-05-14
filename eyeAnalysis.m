function output = eyeAnalysis(fileWithPath) 

 

 

%% set path to Adi's toolbox 

% addpath('/Users/ccr/Dropbox/work/teaching/spm/2013_Zurich_internal/session_10_08052013/eye/Edf2Mat_Toolbox'); 

 

 

%% read in data 

% hardcode just for demo

%  fileWithPath = '/Users/ccr/Dropbox/work/teaching/spm/2013_Zurich_internal/session_10_08052013/eye/edf.mat';

 

[p f e] = fileparts(fileWithPath);

if strfind(e,'.edf')

    edf = Edf2Mat(fileWithPath);

elseif strfind(e,'.mat')

   load(fileWithPath);

else

    error('unkown filetype!');

    return

end

 

 

%% set first timestamp to zero 

edfTime = (edf.Samples.time - edf.Samples.time(1)).'; %%% first entry is now t=0: start of experiment%%% 

 

 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

%% determine start and temporal resolution 

edfTimeStepRes = edfTime(2) - edfTime(1); 

 

START_PHASE_TYPE = 1; 

 

startTime = edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type] == START_PHASE_TYPE); 

 

startTime = double(startTime(1).sttime); % (1) if multiple start   

  

  

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

%% determine saccade parameters 

SACCADE_START_TYPE = 5; 

 

saccStarts = double([edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type].' == SACCADE_START_TYPE).sttime].'); 

 

Sacc.Ons  = saccStarts - startTime; 

  

SACCADE_END_TYPE = 6; 

 

saccStops   = double([edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type].' == SACCADE_END_TYPE).entime].'); 

 

Sacc.Ends = saccStops - startTime; 

  

Sacc.Dur  = Sacc.Ends - Sacc.Ons; 

  

posX = [edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type].' == SACCADE_END_TYPE).gstx]; 

 

posXend  = [edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type].' == SACCADE_END_TYPE).genx]; 

 

posY = [edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type].' == SACCADE_END_TYPE).gsty]; 

 

posYend  = [edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type].' == SACCADE_END_TYPE).geny]; 

  

Sacc.EuclDist = sqrt([posX(:) - posXend(:)].^2 + [posY(:) - posYend(:)].^2) 

 

Sacc.Speed = Sacc.EuclDist./Sacc.Dur; 

 

 

% determine regressors  

output.Sacc.Ons = Sacc.Ons/1000; % in seconds; this one goes into SPM as event onset 

 

output.Sacc.Size = Sacc.EuclDist; % this could be paramteric modulator 

 

output.Sacc.Speed = Sacc.Speed; % this one could also be parametric modulator 

 

output.Sacc.Dur = Sacc.Dur; % this one could also be parametric modulator 

 

% possible other variables are X and Y position of eye 

  

  

%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

%% determine blink parameters 

BLINKSTARTTYPE = 3; % see edf_data.h to have an overview of available types 

BLINKENDTYPE   = 4; 

 

blinkStart     = double([edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type].' == BLINKSTARTTYPE).sttime].'); 

 

blinkEnd       = double([edf.RawEdf.FEVENT([edf.RawEdf.FEVENT.type].' == BLINKENDTYPE).entime].'); 

  

Blink.Ons  = blinkStart - startTime; 

 

Blink.Ends = blinkEnd - startTime; 

 

Blink.Dur  = Blink.Ends - Blink.Ons; 

 

 

%% determine improbable signal dropouts 

% you need to play with these numbers based on the literature 

blinkBelow80msID = Blink.Dur < 80; 

 

blinkAbove2000msID = Blink.Dur > 2000; 

 

 

%% Remove improbable blinks 

Blink.OnsCorr = Blink.Ons(~blinkBelow80msID & ~blinkAbove2000msID); 

Blink.DurCorr = Blink.Dur(~blinkBelow80msID & ~blinkAbove2000msID); 

 

 

 

%% Determine regressors  

output.Blink.Ons = Blink.OnsCorr/1000; %this one goes into SPM as event onset 

 

output.Blink.Dur = Blink.DurCorr; % this could be a parametric modulator 

  

    

%%%%%%%%%%%%%%%%%%%%% 

%% derive pupil signal 

% first we need to interpolate across blinks 

Pupil.sizeFiltered = edf.Samples.pa(:,1); 

 

for iBlinks = 1:length(Blink.Ons) 

    samplesBeforeBlink = min([8 Blink.Ons(iBlinks)/edfTimeStepRes]); 

  

    Blink.startInd(iBlinks) = find(edfTime == Blink.Ons(iBlinks)) - samplesBeforeBlink*edfTimeStepRes; 

  

    samplesAfterBlink = min([8 (edfTime(end)-Blink.Ends(iBlinks))/edfTimeStepRes]); 

  

    Blink.stopInd(iBlinks) = find(edfTime == Blink.Ons(iBlinks)+Blink.Dur(iBlinks)) + samplesAfterBlink*edfTimeStepRes; 

  

    Blink.OnsPupil(iBlinks) = Pupil.sizeFiltered(Blink.startInd(iBlinks)); 

  

    Blink.EndsPupil(iBlinks) = Pupil.sizeFiltered(Blink.stopInd(iBlinks)); 

  

    pupilInterp = linspace(Blink.OnsPupil(iBlinks),Blink.EndsPupil(iBlinks),Blink.stopInd(iBlinks)-Blink.startInd(iBlinks)); 

  

    Pupil.sizeFiltered(Blink.startInd(iBlinks)+1:Blink.stopInd(iBlinks)) = pupilInterp; 

  

end 

  

 

%% build pupil regressor 

  

TR = 2600; microTimeRes = 54; volNr = 147; 

  

microTimeBINS = linspace(0, TR*volNr, microTimeRes*volNr); 

  

Pupil.sizeFiltered = double(Pupil.sizeFiltered(edfTime<=microTimeBINS(end)));   

  

P = length(Pupil.sizeFiltered); 

  

Q = volNr*microTimeRes;   

  

Pupil.sizeFilteredfMRI = resample(Pupil.sizeFiltered,P,Q); 

    

output.Pupil.RegBase = microTimeBINS/1000; % to have a base for parametric modulation 

  

output.Pupil.Size = Pupil.sizeFilteredfMRI % This one needs to go in as parameteric regressor 

    

  

 end 


