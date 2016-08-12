% This function is how to use mouse click callbacks on a figure
% figure
% To use this function, register the two callbacks by executing the two
% lines below
% obj.imhandle.Parent.WindowButtonUpFcn = @(object, eventdata)mouseClicked(false);
% obj.imhandle.Parent.WindowButtonDownFcn = @(object, eventdata)mouseClicked(true);

function mouseClicked(obj, isDown)
    if isDown      
        switch obj.imhandle.Parent.Parent.SelectionType
            case {'normal', 'extend'}                                  
                obj.mouseMove(true);
                obj.imhandle.Parent.Parent.WindowButtonMotionFcn = @(object, eventdata)obj.mouseMove();
            case 'alt' 
                obj.mouseMove(false, true);
            case 'open'            
            otherwise
        end
    else
        obj.imhandle.Parent.Parent.WindowButtonMotionFcn = '';
    end
end

% if you want to react on a certain button extend the switch as following 
%
% switch get(gcf,'SelectionType')
%     case 'normal'
%         % do something for left-click
%     case 'extend'
%         % do something for center-click
%     case 'alt'
%         % do something for right-click
%     case 'open'
%         % do something for double-click
%     otherwise
%         % never happens
% end

% see also:
% http://www.mathworks.ch/ch/help/matlab/ref/figure_props.html#SelectionType