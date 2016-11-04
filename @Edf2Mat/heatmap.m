function [heatMap, gaze] = heatmap(obj, startIdx, endIdx)
%% heatMap
% generating heatMap data
%
%% Syntax
%# heatMap(obj);
%# obj.heatMap();
%# [h, g] = heatMap(obj, startIdx, endIdx);
%
%% Description
%   This function generates the data array (heatMap) for visualize a heatmap of the
%   eyetracker. As a second output it returns an array (gaze) with all
%   positions rated the same.
%
%
%% Example
%# [h, g] = edf.heatMap();
%
%% see also
%# Edf2Mat.plotHeatmap()
%# Edf2Mat.plot()

% Initialization,checking 
assert(isa(obj, 'Edf2Mat'), 'Edf2Mat:plot', ...
    'Only objects of type Edf2Mat can be plotted!');

    if ~exist('startIdx', 'var')
        startIdx = 1;
    end

    if ~exist('endIdx', 'var')
    endIdx = numel(obj.Samples.posX);
    end
    
range = startIdx:endIdx;

assert(numel(range) > 0, 'Edf2Mat:plot:range', ...
    'Start Index == End Index, nothing do be plotted');

%variables
gaussSize = 80;
gaussSigma = 20;

posX = obj.Samples.posX(range);
posY = obj.Samples.posY(range);

%generating data for heatmap
gazedata = [posY, posX];
gazedata = gazedata(~isnan(gazedata(:, 1)), :);
gazedata = gazedata + abs(min(gazedata(:)));
gazedata = ceil(gazedata) + 1;
data = accumarray(gazedata, 1);
gaze = zeros(size(data));
cut = mean(data(:));
data(data > cut) = cut;

kernel = createGauss(gaussSize, gaussSigma);
heatMap = conv2(data, kernel, 'same');

gaze(data > 0) = mean(heatMap(:));

end

