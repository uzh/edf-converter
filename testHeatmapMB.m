% edf = Edf2Mat('arnaud.edf');
%% initialize and define Variables

edf = edf0;

posX = edf.Samples.posX;
posY = edf.Samples.posY;

gazedata = [posY, posX];
sizeX = ceil(max(gazedata(:,1))) + 1;
sizeY = ceil(max(gazedata(:,2))) + 1;
out = zeros(sizeX, sizeY);

data = hist3(gazedata, [sizeX, sizeY]);

kernel = createGauss(60, 1);
J = conv2(data, kernel, 'same');

heatMap = log(J);

subplot(1,3,1);
imagesc(heatMap);

subplot(1,3,2);
plot(gazedata(:,2), gazedata(:,1), '.');
set(gca, 'YDir','reverse');

subplot(1,3,3);
hold on;
imagesc(heatMap);
plot(gazedata(:,2), gazedata(:,1), '.');
set(gca, 'YDir','reverse');