function deployPublic()
    %%
    destination = './public/edf-converter/';

    files2copy = {'@Edf2Mat', ...
        'Readme.md', ...
        'eyedata.edf', ...
        'Edf_Matlab_Converter_Manual.pdf', ...
        'edf2mat_example.m'};

    for iFile = 1:numel(files2copy)
        copyfile(files2copy{iFile}, [destination filesep files2copy{iFile}], 'f');
    end

    
end
