function deployPublic()
    %%
    destination = './public/edf-converter/';

    files2copy = {'@Edf2Mat', ...
        'README.md', ...
        'LICENSE', ...
        'eyedata.edf', ...
        'images', ...
        'Edf_Matlab_Converter_Manual.pdf', ...
        'Example.m'};

    for iFile = 1:numel(files2copy)
        copyfile(files2copy{iFile}, [destination filesep files2copy{iFile}], 'f');
    end

    system(['git -C ', destination, ' add . ']);
    system(['git -C ', destination, ' commit -m "Release: ', datestr(now, 'YYYYMMDDHH'), '"']);
    system(['git -C ', destination, ' push origin']);
    system(['git -C ', destination, ' push github']);
end
