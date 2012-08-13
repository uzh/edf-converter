#ifndef __SR__SCENECAMERA__AVIEDFTIME__
#define __SR__SCENECAMERA__AVIEDFTIME__
#define EDFAVITIMEMAGIC 0xAE5A1000
class AVIEDFTime
{
    /*
    bit     description     Possible values
    1       field           0=first field 1=second field
    2-32    frame number    number >=1
    */
    unsigned int frameAndField;
    /*
    edf time in unsigned int in msec
    */
    unsigned int edfTime;

    /*
    bit     description         Possible values
    1       correction status    0=not corrected 1=corrected
    2       stalled field       0=not stalled   1=stalled
    3-16    offset from edftime To get the predicted edf time use edfTime+offset
    */
    unsigned short correctedEDFTime;


public:
    inline AVIEDFTime() {frameAndField=edfTime=correctedEDFTime=0; }

    AVIEDFTime(unsigned int frame, int field, int edfTime, int correctedTime, bool corrected, bool stalled);


    inline int getField() const { return (int)((frameAndField &0x80000000)>>31); }
    inline int getFrame() const { return (int)(frameAndField & 0x7FFFFFFF); }
    inline int getEDFTime() const { return edfTime; }
    inline bool getStalled() const { return ((correctedEDFTime & 0x8000) == 0x8000); }
    inline bool getCorrection() const { return ((correctedEDFTime & 0x4000) == 0x4000); }
    inline short getOffset() const
    {
        short diff = correctedEDFTime &0x7FFF;
        if(diff &0x2000)
            diff = 0-(diff &0x1FFF);
        else
            diff = (diff &0x1FFF);
        return diff;
    }
    inline int getCorrectedTime() const { return getEDFTime() + getOffset(); }
    void print(FILE *file=NULL);
};


class AVIEDFTimeLookupTime
{
    int currentIndex;
    int recordCount;
    AVIEDFTime* timeTable;
    char sessionName[20]; // maximum really 8 characters
	AVIEDFTime * findFrameForEDFTime(int start, int end, int edfTime);
	int currentTrialIndex;
	char pathofettfile[1024]; // this is just the path not the absolute path of the ett file.
public:
    AVIEDFTimeLookupTime(int trial);
    virtual ~AVIEDFTimeLookupTime();
    /*
    if the field == -1 - not specified
    look at both fields.

    if a particular field is specified and is found but the field is not matched
    the matched entry is returned.
    */
    AVIEDFTime * findAVIEDFEntry(int edfTime, int field=-1);
    bool read(FILE *file);
    void print(FILE *file =NULL);
    AVIEDFTime * binfindAVIEDFEntry(int start, int end, int edfTime, int field);
    AVIEDFTime * secfindAVIEDFEntry(int edfTime, int field);
    AVIEDFTime * getNextEntry();
    void stepBack();
    inline char *getSessionName() const { return (char *)sessionName; }
	inline char *getSessionPath() const { return (char *)pathofettfile; }
    void setSessionName(char *ses);
	void setSessionPath(char *ses);

	AVIEDFTime * AVIEDFTimeLookupTime::findFrameForEDFTime(int edfTime);
	int getCurrentTrialIndex() const { return currentTrialIndex; }
};
#endif
