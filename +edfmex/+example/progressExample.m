    %%
    nrJobs = 1000;
    progress = 0;
    fprintf('Progress: %3.0f%%\n', 0);
    while progress < nrJobs
        progressInPercentage = 100*progress/nrJobs; 
        fprintf('\b\b\b\b\b%3.0f%%\n', progressInPercentage);
        drawnow();
        progress = progress + 1;       
    end
