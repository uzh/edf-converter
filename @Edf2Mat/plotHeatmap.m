function plotHeatmap(obj, startIdx, endIdx)
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
    obj.imhandle = imagesc(obj.heatmap(startIdx, endIdx));
    
    axis image off ij;
	colorbar;
    title('HeatMap of the eye movement');

    % Register the click callbacks:
    % register mouse down function handle
    obj.imhandle.Parent.Parent.WindowButtonDownFcn = @(object, eventdata)obj.mouseClicked(true);
    % register mouse up mouse
    obj.imhandle.Parent.Parent.WindowButtonUpFcn = @(object, eventdata)obj.mouseClicked(false);

end