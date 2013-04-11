function build()

    sourceFolder        = 'source';
    sysDependentHeader  = 'edf2mex.h';
    sourceFile          = 'edfimporter.cpp';

    switch computer
        case 'PCWIN'
            makeHeader = @makeHeader_Win32;
            folder = 'win32';
        case 'PCWIN64'
            makeHeader = @makeHeader_Win64;
            folder = 'win64';
        case 'MACI64'
            makeHeader = @makeHeader_MacOSX;
            folder  = 'osx';
            lib     = 'CXXLIBS=\$CXXLIBS -framework edfapi -F/Library/Frameworks';
            include = 'CXXFLAGS=\$CXXFLAGS -I/Library/Frameworks/edfapi.framework/Headers';
        otherwise
            error('EDF_Build:Computer:unsupported', 'This system is not supported');
    end    
    
    addpath(sourceFolder);
    makeHeader();
    try
        %status = movefile([pwd filesep folder filesep sysDependentHeader], [pwd filesep sourceFolder filesep], 'f');
        %assert(~logical(status), 'Build:movefile', 'Move system dependent header failed');            
       mex(lib, include, [sourceFolder filesep sourceFile])
    catch err
        rmpath(sourceFolder);
        rethrow(err);
    end
    
    rmpath(sourceFolder);
    
end