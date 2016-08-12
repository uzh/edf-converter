function mouseMove(obj, firstClick, reset)

    persistent fullRange prevPoint range width height ;

    if ~exist('reset', 'var')
        reset = false;
    end
    
    if ~exist('firstClick', 'var')
        firstClick = false;
    end

    % get mouse position
    point = obj.imhandle.Parent.CurrentPoint;
    point = [point(1, 1), point(1, 2)];
    
    if firstClick
        prevPoint = point;
    end
    
    if isempty(fullRange)
        clim        = obj.imhandle.Parent.CLim;
        fullRange   = [clim(1), clim(2)];
        range       = [0, 1];
        xlim        = obj.imhandle.Parent.XLim;
        ylim        = obj.imhandle.Parent.YLim;
        width       = abs(diff(xlim));
        height      = abs(diff(ylim));
    end
    if reset
        boundaries  = fullRange;
        prevPoint   = point;
        range       = [0, 1];
    else        
        delta       = point - prevPoint; % mouse movement change
        delta       = delta  .* [1, -1]; % invert y change to make it similar like the philips console
        prevPoint   = point;             % store acutal point to previous point
        
        imRange     = abs(diff(fullRange));
       
        delta       = delta ./ 1000; % normed delta - Range between 0 and 1;
        range       = range + delta; % update range with delta
        range       = min(1, max(0, range)); % scale between 0 and 1
        
        % update boundaries in range 0 - to full range
        boundaries(1) = min(imRange - 1, max(0, range(1) * imRange)); % value between 0 and imRange - 1
        boundaries(2) = min(imRange, max(boundaries(1) + 1, max(1, range(2) * imRange))); % value between boundaries(1) + 1 and imRange
        
        % display range
        plotTitle = sprintf('range =[%1.2f, %1.2f]', boundaries);
        obj.imhandle.Parent.Title.String = plotTitle;

    end
    % set set dynamic range
    obj.imhandle.Parent.CLim = boundaries;
end