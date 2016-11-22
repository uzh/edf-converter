function plotBlinkData(obj, startIdx, endIdx)

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

    timeRange = obj.Samples.time(range);

    
    [timeline, startPoint] = obj.getTimeline();
    timeLineRange = timeline >= min(timeRange) & ...
        timeline <= max(timeRange);
    timeline = timeline(timeLineRange);
    timeline = timeline - startPoint;
    
    legendString = {};
    blinkTimeline =  obj.getBlinkTimeline();
    
    if ~isempty(blinkTimeline) && numel(blinkTimeline) > 0
        blinkTimeline = blinkTimeline(timeLineRange);
        plot(timeline, blinkTimeline, 'r-', 'DisplayName','Blinks');
        legendString{end+1, 1} = 'Blinks';
        hold on;
    end
    messageTimeline = obj.getMessageTimeline();
    if ~isempty(messageTimeline) && numel(messageTimeline) > 0
        messageTimeline = messageTimeline(timeLineRange);
        plot(timeline, messageTimeline, 'bo', 'DisplayName','Message Occurrence');
        legendString{end+1, 1} = 'Message Occurrence';        
    end
    legend(legendString{:});
end