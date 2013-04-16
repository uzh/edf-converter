function build()

    sourceFolder        = 'source';
    sourceFile          = 'edfimporter.cpp';

    switch computer
        case 'PCWIN'
            makeHeader = @makeHeader_Win32;
            folder = 'win32';
            include = '-largeArrayDims'; 
            lib = [folder filesep 'edfapi.lib'];
        case 'PCWIN64'
            makeHeader = @makeHeader_Win64;
            folder = 'win64';
            include = '-largeArrayDims'; 
            lib = [folder filesep 'edfapi64.lib'];
        case 'MACI64'
            makeHeader = @makeHeader_MacOSX;
            folder  = 'osx';
            lib     = 'CXXLIBS=\$CXXLIBS -framework edfapi -F/Library/Frameworks';
            include = 'CXXFLAGS=\$CXXFLAGS -I/Library/Frameworks/edfapi.framework/Headers';
        otherwise
            error('EDF_Build:Computer:unsupported', 'This system is not supported');
    end    
    
    addpath(folder);
    addpath(sourceFolder);
    makeHeader();
    try
        %status = movefile([pwd filesep folder filesep sysDependentHeader], [pwd filesep sourceFolder filesep], 'f');
        %assert(~logical(status), 'Build:movefile', 'Move system dependent header failed');            
       mex([sourceFolder filesep sourceFile], lib, include)
    catch err
        rmpath(sourceFolder);
        rethrow(err);
    end
    
    rmpath(folder);
    rmpath(sourceFolder);
    
end