function build(debug)
    %% validate input
    currentFileLocation = fileparts(mfilename("fullpath"));
    
    flags = {'-largeArrayDims', "-outdir", currentFileLocation};
    cleanup = true;
    if nargin >= 1 && logical(debug)
        flags = [{'-v'}, flags];
        cleanup = false;
    end

    %% variables
    createPath = @(subfolder)fullfile(currentFileLocation, subfolder);

    name = "edfimporter";
    headerFilename = "edf2mex.h";

    sourceFolder = createPath("source");
    sourceFile = fullfile(sourceFolder, sprintf("%s.cpp", name));

    mexFile = createPath(sprintf("%s.%s", name, mexext));
    edf2matPrivateFolder = fullfile(fileparts(currentFileLocation), "@Edf2Mat", "private");

    % related for both MACOS cases
    framework.systemDir = fullfile("/", "Library", "Frameworks");
    framework.name = "edfapi";
    framework.path = fullfile(framework.systemDir, sprintf("%s.framework", framework.name));
    framework.headers = fullfile(framework.path, "Headers");

    %% define mex arguments and makeHeader function

    include = '';
    switch computer
        case 'PCWIN'
            path = createPath("win32");
            lib = fullfile(path, 'edfapi.lib');
        case 'PCWIN64'
            path = createPath( "win64");
            lib = fullfile(path, 'edfapi64.lib');
        case 'MACI64'
            path = createPath("osx");
            lib     = sprintf('CXXLIBS=$CXXLIBS -F%s -framework %s', framework.systemDir, framework.name);
            include = sprintf('CXXFLAGS=$CXXFLAGS -I%s', framework.headers);
        case 'MACA64'
            path = createPath("osx_amd");
            lib = sprintf('CXXLIBS=$CXXLIBS -framework %s', framework.name);
            include = sprintf('CXXFLAGS=$CXXFLAGS -I%s', framework.headers);
        otherwise
            error('EDF_Build:Computer:unsupported', 'This system is not supported');
    end

    %% create and move header file
    addpath(path);
    makeHeader();
    rmpath(path);

    %% run mex
    mex(flags{:}, sourceFile, lib, include);

    %% mex callback - correct paths
    switch computer
        case 'MACA64'
            % The compiler somehow links the edfapi framework, as it would
            % be located in the matlabs "@rpath"
            % -> check framework path and adjust if needed

            [~, cmdout] = system(sprintf('otool -L %s', mexFile));

            wrongFrameworkPath = '@rpath/edfapi.framework/Versions/A/edfapi';
            correctFrameworkPath = fullfile(framework.path, "Versions", "A", framework.name);

            if contains(cmdout, wrongFrameworkPath)
                disp('MEX linked the wrong path, correcting it.');
                system(sprintf('install_name_tool -change %s %s %s', wrongFrameworkPath, correctFrameworkPath, mexFile));
            end
    end

    %% add mex to Edf2mat Toolbox private folder
    movefile(mexFile, edf2matPrivateFolder, "f");

    %% cleanup
    if cleanup
        fprintf("Delete generated files: %s\n", headerFilename);
        delete(fullfile(sourceFolder, headerFilename));
    end
end

