# Edf2Mat© Matlab Toolbox

Converts EyeLink 1000 Edf files into Matlab

```txt
  Version 1.20

  Adrian Etter
  Marc Biedermann

  University of Zurich
  Department of Economics
  Schönberggasse 1
  CH-8001 Zurich
```

E-Mail: <engineering@econ.uzh.ch>

URL:

- [https://www.econ.uzh.ch/en/people/staff/etter.html](https://www.econ.uzh.ch/en/people/staff/etter.html)
- [https://www.econ.uzh.ch/en/people/staff/biedermann.html](https://www.econ.uzh.ch/en/people/staff/biedermann.html)

## Abstract

Edf2Mat is a Matlab Toolbox for easy conversion of EyeLink Edf result files. For fast verification of valid data, there is an included plot function, which displays eye movement,  pupil size and a heatmap of the eye movement. There are a few examples in the example file which help analyze eye data.

## Copyright

Copyright © 2007-2016 Adrian Etter, Marc Biedermann, University of Zurich. All rights reserved.

This document may be copied, modified, reproduced and redistributed for educational and personal use as long as the original author is mentioned and cited.

MATLAB® is a registered Trademark of MathWorks, Inc.™ (<http://www.mathworks.com>).
EyeLink® is a registered trademark of SR Research Ltd., Mississauga, Ontario, Canada (<http://www.sr-research.com>)

## Acknowledgment

You are allowed to use this software for free, but please acknowledge if you are using this software to process Edf-files:

The conversion of the EyeLink® 1000 Edf files was done with the Edf2Mat Matlab Toolbox designed and developed by Adrian Etter and Marc Biedermann at the University of Zurich.

## License

Edf2Mat Toolbox is Licensed under the MIT License.
The Edf2Mat Toolbox uses slightly modified code (Kovach, 2011) from C. Kovach 2007.

## Installation

### Requirements

#### On Windows: Matlab

- Ensure the [Visual C++ Redistributable for Visual Studio 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145) is installed
- Ensure the [Visual C++ Redistributable for Visual Studio 2008](https://www.microsoft.com/en-us/download/details.aspx?id=29) is installed

#### On Mac

the edfapi.framework must be in `/Library/Frameworks`. The Library can be found in the Package. Attention: If the Zip file was unpacked on windows first, and then copied, the folder structure will be broken. The zip-file must be extracted on a Mac in order to work. Otherwise the symlinks will be broken.

on os x in `/Library/Frameworks` it should look like the following

```txt
edfapi.framework/
├── Headers -> Versions/Current/Headers
├── Resources -> Versions/Current/Resources
├── Versions
│   ├── A
│   │   ├── Headers
│   │   │   ├── edf.h
│   │   │   ├── edf_data.h
│   │   │   └── edftypes.h
│   │   ├── Resources
│   │   │   └── Info.plist
│   │   └── edfapi
│   └── Current -> A
└── edfapi -> Versions/Current/edfapi
```

### Files needed

- The Edf2Mat Class
- All files in the private folder
- All Dlls in the private folder

### Edf2Mat with Linux using wine

Edf2Mat on Linux with `wine` relies on the Windows executable `edf2asc.exe`. Check the additional comments about it:

- Matlab needs to be run without admin privileges (no sudo). This is a safety feature of `wine`.
- The user running the Matlab instance needs permission to read and write the edf files.

## How to use Edf2Mat – Toolbox

There is an `Example.m` script. Have a look at it.
Type `help` for help

```bash
  help Edf2Mat


    Edf2Mat is a converter to convert Eyetracker data files to
    Matlab file and perform some tasks on the data

    The new procedure uses code from SR-Research that returns all info of
    the edf and not just part of it. The new routine is based on the work
    of C. Kovach 2007 and is only for non-commercial use!



    Syntax: Edf2Mat(filename);
          Edf2Mat(filename, verbose);


   Inputs:
      filename:           must be of type *.edf
      useOldProcedure:    If you want to use the old procedure with
                          edf2asc.exe, you can set this argument to
                          true, default is false
      verbose:            logical, can be true or false, default is true.
                          If you want to suppress output to console,
                          verbose has to be false


  The Basic functionality is as follows:
  Convert Edf File

  edf1 = Edf2Mat('fMRI_Results_sub_025_270712EYE25r1.edf');
```

Calling the Edf2Mat with a filename converts the given edf file to a Matlab structure, which will be available in the Matlab workspace.
In order to save the produced structure to a matfile, just call “save(edf1)”, whereas edf1 is the variable assign when calling the Edf2Mat Class.

### Plot

The Edf2Mat class has its own plot functionality to plot the content. It’s more for a fast forward validation of data than actually the way you should plot your data.

```matlab
plot(edf1);
```

![alt text](./images/plotEdf.png "Example of the function plot(edf1)")

#### Plot last 2000 Elements

In order to plot eye movement only in a specified time range, the Matlab builitin plot command could be used as following:

```matlab
figure();
plot(edf1.Samples.posX(end - 2000:end), edf1.Samples.posY(end - 2000:end), 'o');
```

#### Plot the pupil size

To simply plot the pupil size for a given time window, the pupil size array can be accessed as stated in the next line.

```matlab
figure();
plot(edf1.Samples.pa(2, end - 500:end));
```

#### Plot just the heatmap of the eye movement

```matlab
plotHeatmap(edf1);
```

![alt text](./images/heatmapExample.png "Example of the function plotHeatmap(edf1)")

## Bibliography

Kovach, C. (2011, 01 12). SR Research. Retrieved from SR Research Support: [https://www.sr-support.com/showthread.php?255-Import-of-EDF-file-into-Matlab&p=6781#post6781](https://www.sr-support.com/showthread.php?255-Import-of-EDF-file-into-Matlab&p=6781#post6781)
