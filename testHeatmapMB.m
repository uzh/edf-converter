%% Open file, for the Heatmap

defaultfile = 'eyedata.edf';
[file,path] = uigetfile(defaultfile,'Heatmap for file name');

assert(strcmp(file(end - 3:end), '.edf'), 'hestHeatmapMB:filename', ...
                  'Filename must be of type .edf!');

% convert with Edf2Mat
edf = Edf2Mat(file);

%% initialize and define Variables
clf('reset');

%variables
gaussSize = 80;
gaussSigma = 20;

posX = edf.Samples.posX;
posY = edf.Samples.posY;

%generating data for heatmap
gazedata = [posY, posX];
gazedata = gazedata(~isnan(gazedata(:, 1)), :);
gazedata = ceil(gazedata) + 1;
data = accumarray(gazedata, 1);
gaze = zeros(size(data));
cut = mean(data(:));
data(data > cut) = cut;

kernel = createGauss(gaussSize, gaussSigma);
heatMap = conv2(data, kernel, 'same');

gaze(data > 0) = mean(heatMap(:));


%visualize heatmap
figure(1);
hold on;
imagesc(heatMap);
% contour(heatMap);
% plot(gazedata(:,2), gazedata(:,1), 'k');
set(gca, 'YDir','reverse');
colorbar;
alpha color;
hold off;