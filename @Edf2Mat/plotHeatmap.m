function plotHeatmap(obj, startIdx, endIdx, image)
% This function generates one figure with the heatmap of the gazepositions.
% The Colorbar can be adapt with click into the figure and move the cursor 
% up and down or sideways. To get back to the default values for the
% colorbar, click the right mouse button.
    
    % clear persistent variables in used function
    clear mouseMove
    
    % validate the input data
    assert(isa(obj, 'Edf2Mat'), 'Edf2Mat:plot', ...
        'Only objects of type Edf2Mat can be plotted!');
    
    if ~exist('startIdx', 'var')
        startIdx = 1;
    end
    
    if ~exist('endIdx', 'var')
        endIdx = numel(obj.Samples.posX);
    end
    
    range = startIdx:endIdx;
    
    assert(numel(range) > 0, ...
        'Edf2Mat:plot:range','Start Index == End Index, nothing do be plotted');
    
    % create the heatmap figure
    figure;
    hold on;
    [heatmap, ~, axisRange] = obj.heatmap(startIdx, endIdx);
    obj.imhandle = imagesc(heatmap);
    
    axis(axisRange);
	colorbar;
    title('HeatMap of the eye movement');

    % load image for layering with heatmap
    if exist('image', 'var')
        imageI = imread(image);
    
        % validate image
        assert(any(strcmp(image(end - 3:end), {'.jpg', '.png', '.bmp', '.gif'})), ...
            'Edf2Mat:plotHeatmap:validateIMG', ...
            'Filename must be of type jpg, jpeg, png, bmp, gif!');
        assert(obj.imhandle.XData(2) == size(imageI,1), ...
            'Edf2Mat:plot:image:XData', 'Nummber of pixels in x direction is not equal');
        assert(obj.imhandle.YData(2) == size(imageI,2), ...
            'Edf2Mat:plot:image:XData', 'Nummber of pixels in x direction is not equal');
        
        %visualize image    
        imageNew = imagesc(imageI);
        set(imageNew, 'AlphaData', 0.3);
    else
        % if no image will be imported, it returns just the heatmap
    end
        
    % Register the click callbacks:
    % register mouse down function handle
    obj.imhandle.Parent.Parent.WindowButtonDownFcn = @(object, eventdata)obj.mouseClicked(true);
    % register mouse up mouse
    obj.imhandle.Parent.Parent.WindowButtonUpFcn = @(object, eventdata)obj.mouseClicked(false);
    
    hold off;
end