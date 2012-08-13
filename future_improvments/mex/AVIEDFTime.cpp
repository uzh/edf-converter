#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "AVIEDFTime.h"
#define debug_log printf
#define stricmp _stricmp
#define putenv _putenv




AVIEDFTimeLookupTime *lookuptable = NULL;
AVIEDFTime *lastreturn = NULL;


AVIEDFTime::AVIEDFTime(unsigned int frame, int field, int edfTime, int correctedTime, bool corrected, bool stalled)
{
    frameAndField = field;
    frameAndField = (frameAndField <<31)|frame;
    this->edfTime = edfTime;
    if(stalled)  // stalled
        correctedEDFTime = 1;
    else
        correctedEDFTime = 0;

    correctedEDFTime = correctedEDFTime<<1;
    if(corrected)
        correctedEDFTime |= 1; // corrected
    else
        correctedEDFTime |= 0;

    short diff = (correctedTime-edfTime);
    if(diff >0) // corrected diff
    {
        correctedEDFTime = correctedEDFTime<<14;
        correctedEDFTime |= diff;
    }
    else
    {
        correctedEDFTime = correctedEDFTime<<1;
        correctedEDFTime |= 1;
        correctedEDFTime = correctedEDFTime<<13;
        correctedEDFTime |= (0-diff); // make it pos
    }
    //debug_log("%d\t%d\t%d\t%d\t%d\t%d\n", frame, field,edfTime,correctedTime,corrected,stalled);
}

void AVIEDFTime::print(FILE *file)
{
    if(!file) file = stdout;
    fprintf(file,"%d\t%d\t%d\t%d\t%d\t%d\n", getFrame(), getField(),getEDFTime(),getCorrectedTime(),getCorrection(),getStalled());
}


AVIEDFTimeLookupTime::AVIEDFTimeLookupTime(int trial)
{
    currentIndex = 0;
    recordCount = 0;
    timeTable = NULL;
	currentTrialIndex = trial;
}
AVIEDFTimeLookupTime::~AVIEDFTimeLookupTime()
{
    if(timeTable != NULL)
        delete [] timeTable;
}

void AVIEDFTimeLookupTime::print(FILE *file)
{
    for(int i =0; i <recordCount; i++)
    {
        timeTable[i].print(file);
    }
}

/*
    if the field == -1 - not specified
    look at both fields.

    if a particular field is specified and is found but the field is not matched
    the matched entry is returned.
*/

/*
This is a sequential search.
*/
AVIEDFTime * AVIEDFTimeLookupTime::secfindAVIEDFEntry(int edfTime, int field)
{

    for(int i =0; i <recordCount; i++)
    {
        if(timeTable[i].getEDFTime() == edfTime) // this will always match the first one.
        {
            if(timeTable[i].getField() == field || field == -1)
                return &timeTable[i];
            else if(i+1 < recordCount && timeTable[i+1].getField() == field && timeTable[i+1].getEDFTime() == edfTime)
                return &timeTable[i+1];
            else
                return &timeTable[i];
        }
    }
    return NULL;
}




AVIEDFTime * AVIEDFTimeLookupTime::binfindAVIEDFEntry(int start, int end, int edfTime, int field)
{

    // The edf time is out of bound no point in checking for it any where. and the field better be 0 or 1
    assert(field  == 0 || field == 1 || field ==-1);
    if(!recordCount || edfTime > timeTable[recordCount-1].getEDFTime() || edfTime < timeTable[0].getEDFTime()  || start>end)
        return NULL;


    int middle = start + (end-start)/2;

    if(timeTable[middle].getEDFTime() == edfTime )
    { //careful, this may not match the first one all the time. so we better find i such that i is the first one.
        int i = middle;
        while(timeTable[i].getEDFTime() == edfTime && i >=0) i--;
        i++;
        assert(timeTable[i].getEDFTime() == edfTime); // this should be the first match.
        if(timeTable[i].getField() == field || field == -1)
            return &timeTable[i];
        else if(i+1 < recordCount && timeTable[i+1].getField() == field && timeTable[i+1].getEDFTime() == edfTime)
            return &timeTable[i+1];
        else
            return &timeTable[i];
    }
    else if(timeTable[middle].getEDFTime() > edfTime)
        return binfindAVIEDFEntry(start,middle-1,edfTime, field);
    else
        return binfindAVIEDFEntry(middle+1,end,edfTime, field);




}
AVIEDFTime * AVIEDFTimeLookupTime::findFrameForEDFTime(int edfTime)
{
	return findFrameForEDFTime(0,recordCount,edfTime);
}
AVIEDFTime * AVIEDFTimeLookupTime::findFrameForEDFTime(int start, int end, int edfTime)
{

    // The edf time is out of bound no point in checking for it any where. and the field better be 0 or 1
    if(!recordCount || edfTime > timeTable[recordCount-1].getCorrectedTime() || edfTime < timeTable[0].getCorrectedTime()  || start>end)
        return NULL;


    int middle = start + (end-start)/2;

	//fprintf(stderr,"findFrameForEDFTime %d %d %d %d \n", start, end, edfTime, timeTable[middle].getCorrectedTime());
	//fflush(NULL);
    if(timeTable[middle].getCorrectedTime() >= (edfTime-30) && timeTable[middle].getCorrectedTime() <= (edfTime+30))
    { // it is possible to have more than one value in here. For now we will just return the middle. later on we will
		// find the lowest.
        return &timeTable[middle];
    }
    else if(timeTable[middle].getCorrectedTime() > edfTime)
        return findFrameForEDFTime(start,middle-1,edfTime);
    else
        return findFrameForEDFTime(middle+1,end,edfTime);

}

AVIEDFTime * AVIEDFTimeLookupTime::findAVIEDFEntry(int edfTime, int field)
{

    AVIEDFTime * rv1 =binfindAVIEDFEntry(0, recordCount,edfTime,field);
#ifdef _DEBUG
    AVIEDFTime * rv2 =secfindAVIEDFEntry(edfTime,field);
    assert(rv1==rv2);
#endif
    return rv1;
}
void AVIEDFTimeLookupTime::setSessionName(char *ses)
{
    strcpy(this->sessionName,ses);
}

void AVIEDFTimeLookupTime::setSessionPath(char *ses)
{
    strcpy(this->pathofettfile,ses);
}
bool AVIEDFTimeLookupTime::read(FILE *file)
{
  int rv = fread(&recordCount,1,sizeof(recordCount),file);
  if(rv !=4) // write the recordNumbers
  {
        debug_log("Read Error. Cannot read the number of records\n");
        return false;
  }

  int magic=-1;


  if(fread(&magic,1,sizeof(magic),file) != 4 || magic != EDFAVITIMEMAGIC)
  {
      debug_log("Bad magic! aborting read!!! \n");
      return false;
  }

  timeTable = new AVIEDFTime[recordCount];
  for(int i =0; i < recordCount; i++)
  {
      if(fread(&(timeTable[i]),1,sizeof(AVIEDFTime),file) != sizeof(AVIEDFTime))
      {
        debug_log("Read Error. Cannot read record %d! Aborting....\n", i);
        return false;
      }
  }
  return true;
}
AVIEDFTime * AVIEDFTimeLookupTime::getNextEntry()
{
    if(currentIndex < recordCount && currentIndex>=0)
        return &timeTable[currentIndex ++];
    return NULL;
}

void AVIEDFTimeLookupTime::stepBack()
{
    if(currentIndex >0)
        currentIndex--;
}



extern "C"{
void closeLookupTable();
int openLookupTable(char *edffile, int trial, int  retry);
unsigned int getNextFrameTime();
int getLastFrameNumber();
void setupPath();
extern "C" char *getCurrentSessionName();
};

void closeLookupTable()
{
    if(lookuptable)
        {
			lastreturn = NULL;
            delete lookuptable;
            lookuptable = NULL;
        }
}

int runCommand(char *dir, char *cmd)
{
	static int ransetup =0;
     char cwd[1024] = ".";
	 if(ransetup ==0)
	 {
		 setupPath();
		 ransetup = 1;

	 }
    GetCurrentDirectory(1024,cwd);
    SetCurrentDirectory(dir);
    int rv = system(cmd);
    SetCurrentDirectory(cwd);
    return rv;
}
int openLookupTable(char *edffile, int trial, int  retry)
{

    char edfsession[1024];
    char ettfile[1024];

    int ln = strlen(edffile);
    if(ln <=4 || stricmp(&(edffile[strlen(edffile)-3]),"edf") !=0)
    {
        printf("given edf file is not valid %s \n", edffile);
        return -1;
    }

    strcpy(edfsession,edffile);
    edfsession[strlen(edfsession)-4] =0;



    sprintf(ettfile,"%s%d.ett",edfsession, trial);
	printf("Loading ett file %s \n", ettfile);
	fflush(NULL);

    FILE *file = fopen(ettfile,"rb");
    if(file)
    {
        if(lookuptable)
        {
            delete lookuptable;
            lookuptable = NULL;
        }
        lookuptable = new AVIEDFTimeLookupTime(trial);
        if(!lookuptable->read(file))
        {
            delete lookuptable;
            lookuptable = NULL;
			return -1;
        }

        ln = strlen(edfsession);
        int st = 0;
        for(int i =ln-1; i>=0; i--)
        {
            if(edfsession[i] == '\\' ||edfsession[i] == '/')
            {
                st = i+1;
                break;
            }
        }
		if(st)
		{
			edfsession[st-1]=0;
			lookuptable->setSessionPath(edfsession);
		}
		else
			lookuptable->setSessionPath("");
        lookuptable->setSessionName(&edfsession[st]);

		printf("Done Loading ett file %s \n", ettfile);
		fflush(NULL);

        return 0;
    }
    else if(retry)
    {
        char argv[1024];
        ln = strlen(edfsession);
        int st = 0;
        for(int i =ln-1; i>=0; i--)
        {
            if(edfsession[i] == '\\' ||edfsession[i] == '/')
            {
                st = i;
                break;
            }
        }
        edfsession[st]=0;
        sprintf(argv,"avi2ett %s %d", edfsession+st+1,trial+1);
        printf("creating lookup table file %s \n", argv);

        if(runCommand(edfsession,argv) ==0)
            return openLookupTable(edffile,trial,0);
        else
        {
            printf("failed to create lookup table. %s \n", argv);
            return -1;
        }
    }
    else
    {
        printf( "Cannot read file %s\n", ettfile);
        return -1;
    }

}


int getLastFrameNumber()
{
    if(lastreturn)
        return lastreturn->getFrame();
    else
        return -1;
}

int findFrameNumberForEDFTimeInCurrentLookupTable(int time)
{
	if(lookuptable)
	{
		AVIEDFTime * rv = lookuptable->findFrameForEDFTime(time);
		if(rv)
			return rv->getFrame();
	}
	return -1;
}
int findFrameNumberForEDFTime(int trial, int time)
{
    if(lookuptable)
	{
		if(lookuptable->getCurrentTrialIndex() == trial)
		{
			return findFrameNumberForEDFTimeInCurrentLookupTable(time);
		}
		else
		{
			char fname[2048];
			strcpy(fname,lookuptable->getSessionPath());
			if(strlen(fname) > 0)
				strcat(fname,"\\");
			strcat(fname,lookuptable->getSessionName());
			strcat(fname,".edf");
			openLookupTable(fname,trial,0);
			return findFrameNumberForEDFTimeInCurrentLookupTable(time);
		}

	}
    return -1;
}
unsigned int getNextFrameTime()
{
    lastreturn = NULL;
    if(!lookuptable)
        return -1;
    AVIEDFTime *field1= lookuptable->getNextEntry();
    AVIEDFTime *field2= lookuptable->getNextEntry();

    if(field1 && field2)
    {
        if(field1->getFrame() != field2->getFrame())
        { // different frame for each fields so step back one and return the lowest frame time
            lookuptable->stepBack();
            lastreturn = field1;
            return field1->getCorrectedTime();
        }
        else
        {
            // same frame so, return the lowest time
            if(field1->getCorrectedTime() <= field2->getCorrectedTime())
            {
                lastreturn = field1;
                return field1->getCorrectedTime();
            }
            else
            {
                lastreturn = field2;
                return field2->getCorrectedTime();
            }
        }
    }
    else
    {
        if(field1)
        {
            lastreturn = field1;
            return field1->getCorrectedTime();
        }
        else if(field2)
        {
            lastreturn = field2;
            return field2->getCorrectedTime();
        }
        else
        {
            lastreturn = NULL;
            return -1;
        }
    }
}

char *getCurrentSessionName()
{
    return lookuptable->getSessionName();
}

#ifdef WIN32
#include <windows.h.>
void setupPath()
{
		int i=0;
		char *cpath = NULL;
        char progloc[1024];
        GetModuleFileName(GetModuleHandle(NULL),progloc,1024);

        for(i = strlen(progloc); i >=0 && !(progloc[i] == '/' || progloc[i] == '\\'); i--);
        progloc[i] =0;
		cpath = getenv("PATH");
        if(cpath && i>0 && (strlen(cpath)+strlen(progloc)+10)<1024)
        {
            static char path[1024];
            sprintf(path,"PATH=%s;%s",getenv("PATH"),progloc);
            putenv(path);
        }
		else
			printf("Failed to set the path. Using current path. \n");
}
#else
void setupPath(char *exe)
{// will implement later
}
#endif