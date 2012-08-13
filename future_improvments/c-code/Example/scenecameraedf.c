
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef EDF_DLL
#include "opt.h"
extern Opt options;
#define shouldGazeMap() options.gazeMap
#else
#define shouldGazeMap() 1
#endif

#define NaN 1e8                  /* missing floating-point values*/

float trackerLeft= NaN;
float trackerTop = NaN;
float trackerRight= NaN;
float trackerBottom = NaN;
float displayWidth= NaN;;
float displayHeight = NaN;



int hasValidDimensions(float val)
{
    if( val == NaN ||
        trackerLeft== NaN    ||
        trackerTop == NaN    ||
        trackerRight== NaN   ||
        trackerBottom == NaN ||
        displayWidth== NaN   ||
        displayHeight == NaN)
        return 0;
    else 
        return 1;

}


float getLeft()  { return trackerLeft -   (float)(((trackerRight -trackerLeft)/0.95) -(trackerRight -trackerLeft)); }
float getRight()  { return trackerRight+  (float)(((trackerRight -trackerLeft)/0.95) -(trackerRight -trackerLeft)); }
float getTop()  { return trackerTop-      (float)(((trackerBottom -trackerTop)/0.95)-(trackerBottom -trackerTop)); }
float getBottom() { return trackerBottom+ (float)(((trackerBottom -trackerTop)/0.95)-(trackerBottom -trackerTop)); }



float getMappedX(float x)
{
    if(!shouldGazeMap() || !hasValidDimensions(x)|| x == NaN)
        return x;
    else
    {
        return getLeft()+ ((x/displayWidth) * (getRight() - getLeft()));
    }
}
float getMappedY(float y)
{
    if(!shouldGazeMap() || !hasValidDimensions(y) || y == NaN)
        return y;
    else
    {
        return (getTop()+(y/displayHeight) * (getBottom() - getTop()));
    }
}

void parseDisplayAreaCoords(char *coords)
{
	if(coords)
	{
        int pixel_left=0;
		int pixel_top=0;
		int pixel_right=0;
		int pixel_bottom =0;
        int rv = -1;

		coords += strlen("DISPLAY_AREA_COORDS");

		rv = sscanf(coords, " %d %d %d %d", 
						&pixel_left,  &pixel_top, 
						&pixel_right, &pixel_bottom );
		if(rv == 4)
		{
			trackerLeft   = (float)pixel_left;
			trackerRight  = (float)pixel_right;
			trackerTop    = (float)pixel_top;
			trackerBottom = (float)pixel_bottom;
		}
        else
        {
             trackerLeft= NaN;
             trackerTop = NaN;
             trackerRight= NaN;
             trackerBottom = NaN;
        }

	}

}

void parseDisplayCoords(char *coords)
{
    int rv =0;
    int tracker_pixel_left=0;
	int tracker_pixel_top=0;
	int tracker_pixel_right=0;
	int tracker_pixel_bottom =0;

	coords += strlen("DISPLAY_COORDS");
	rv = sscanf(coords, " %d %d %d %d", 
			&tracker_pixel_left, &tracker_pixel_top, 
			&tracker_pixel_right, &tracker_pixel_bottom );
    if(rv == 4)
    {
	    displayWidth = (float)(tracker_pixel_right - tracker_pixel_left);
	    displayHeight = (float)(tracker_pixel_bottom-tracker_pixel_top);
    }
    else
    {
	    displayWidth = NaN;
	    displayHeight = NaN;
    }
		
	
}


void reinitMappings()
{
	trackerLeft= NaN;
	trackerTop = NaN;
	trackerRight= NaN;
	trackerBottom = NaN;
	displayWidth= NaN;;
	displayHeight = NaN;
}