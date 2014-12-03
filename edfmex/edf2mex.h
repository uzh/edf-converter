

char *  build_version = "3.1 MacOS X Jul 13 2010";


class BuildMexArrays : public BuildMexArraysBaseClass
{


public:

int PopulateArrays( int Offset = 0, int MaxRec = 0 );

~BuildMexArrays();

int NofFSAMPLE;
mxArray * mxFSAMPLE;
int NofFEVENT;
mxArray * mxFEVENT;
int NofIOEVENT;
mxArray * mxIOEVENT;
int NofRECORDINGS;
mxArray * mxRECORDINGS;


/*************************************
*	Functions and objects to handle events of type FSAMPLE
*************************************/

 struct 	FSAMPLEtype
{
	const char * fieldnames[28];
	UINT32* time;
	single* px;
	single* py;
	single* hx;
	single* hy;
	single* pa;
	single* gx;
	single* gy;
	single* rx;
	single* ry;
	single* gxvel;
	single* gyvel;
	single* hxvel;
	single* hyvel;
	single* rxvel;
	single* ryvel;
	single* fgxvel;
	single* fgyvel;
	single* fhxvel;
	single* fhyvel;
	single* frxvel;
	single* fryvel;
	INT16* hdata;
	UINT16* flags;
	UINT16* input;
	UINT16* buttons;
	INT16* htype;
	UINT16* errors;

	FSAMPLEtype(){

			fieldnames[0] = "time";
			fieldnames[1] = "px";
			fieldnames[2] = "py";
			fieldnames[3] = "hx";
			fieldnames[4] = "hy";
			fieldnames[5] = "pa";
			fieldnames[6] = "gx";
			fieldnames[7] = "gy";
			fieldnames[8] = "rx";
			fieldnames[9] = "ry";
			fieldnames[10] = "gxvel";
			fieldnames[11] = "gyvel";
			fieldnames[12] = "hxvel";
			fieldnames[13] = "hyvel";
			fieldnames[14] = "rxvel";
			fieldnames[15] = "ryvel";
			fieldnames[16] = "fgxvel";
			fieldnames[17] = "fgyvel";
			fieldnames[18] = "fhxvel";
			fieldnames[19] = "fhyvel";
			fieldnames[20] = "frxvel";
			fieldnames[21] = "fryvel";
			fieldnames[22] = "hdata";
			fieldnames[23] = "flags";
			fieldnames[24] = "input";
			fieldnames[25] = "buttons";
			fieldnames[26] = "htype";
			fieldnames[27] = "errors";
	};

} strFSAMPLE;

int mxFSAMPLEappend()
{
	memcpy( &(strFSAMPLE.time[NofFSAMPLE]) , &(CurrRec->fs.time), 1*sizeof(UINT32));
	memcpy( &(strFSAMPLE.px[2*NofFSAMPLE]) ,  &(CurrRec->fs.px[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.py[2*NofFSAMPLE]) ,  &(CurrRec->fs.py[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.hx[2*NofFSAMPLE]) ,  &(CurrRec->fs.hx[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.hy[2*NofFSAMPLE]) ,  &(CurrRec->fs.hy[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.pa[2*NofFSAMPLE]) ,  &(CurrRec->fs.pa[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.gx[2*NofFSAMPLE]) ,  &(CurrRec->fs.gx[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.gy[2*NofFSAMPLE]) ,  &(CurrRec->fs.gy[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.rx[NofFSAMPLE]) , &(CurrRec->fs.rx), 1*sizeof(single));
	memcpy( &(strFSAMPLE.ry[NofFSAMPLE]) , &(CurrRec->fs.ry), 1*sizeof(single));
	memcpy( &(strFSAMPLE.gxvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.gxvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.gyvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.gyvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.hxvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.hxvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.hyvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.hyvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.rxvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.rxvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.ryvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.ryvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.fgxvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.fgxvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.fgyvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.fgyvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.fhxvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.fhxvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.fhyvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.fhyvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.frxvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.frxvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.fryvel[2*NofFSAMPLE]) ,  &(CurrRec->fs.fryvel[0]), 2*sizeof(single));
	memcpy( &(strFSAMPLE.hdata[8*NofFSAMPLE]) ,  &(CurrRec->fs.hdata[0]), 8*sizeof(INT16));
	memcpy( &(strFSAMPLE.flags[NofFSAMPLE]) , &(CurrRec->fs.flags), 1*sizeof(UINT16));
	memcpy( &(strFSAMPLE.input[NofFSAMPLE]) , &(CurrRec->fs.input), 1*sizeof(UINT16));
	memcpy( &(strFSAMPLE.buttons[NofFSAMPLE]) , &(CurrRec->fs.buttons), 1*sizeof(UINT16));
	memcpy( &(strFSAMPLE.htype[NofFSAMPLE]) , &(CurrRec->fs.htype), 1*sizeof(INT16));
	memcpy( &(strFSAMPLE.errors[NofFSAMPLE]) , &(CurrRec->fs.errors), 1*sizeof(UINT16));

	NofFSAMPLE++;

	return 0;

};


/*************************************
*	Functions and objects to handle events of type FEVENT
*************************************/

 struct 	FEVENTtype
{
	const char * fieldnames[35];

	FEVENTtype(){

			fieldnames[0] = "time";
			fieldnames[1] = "type";
			fieldnames[2] = "read";
			fieldnames[3] = "sttime";
			fieldnames[4] = "entime";
			fieldnames[5] = "hstx";
			fieldnames[6] = "hsty";
			fieldnames[7] = "gstx";
			fieldnames[8] = "gsty";
			fieldnames[9] = "sta";
			fieldnames[10] = "henx";
			fieldnames[11] = "heny";
			fieldnames[12] = "genx";
			fieldnames[13] = "geny";
			fieldnames[14] = "ena";
			fieldnames[15] = "havx";
			fieldnames[16] = "havy";
			fieldnames[17] = "gavx";
			fieldnames[18] = "gavy";
			fieldnames[19] = "ava";
			fieldnames[20] = "avel";
			fieldnames[21] = "pvel";
			fieldnames[22] = "svel";
			fieldnames[23] = "evel";
			fieldnames[24] = "supd_x";
			fieldnames[25] = "eupd_x";
			fieldnames[26] = "supd_y";
			fieldnames[27] = "eupd_y";
			fieldnames[28] = "eye";
			fieldnames[29] = "status";
			fieldnames[30] = "flags";
			fieldnames[31] = "input";
			fieldnames[32] = "buttons";
			fieldnames[33] = "parsedby";
			fieldnames[34] = "message";
	};

} strFEVENT;

int mxFEVENTappend()
{

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 0,  mxCreateNumericMatrix(1,1, mxUINT32_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 0)) ,&(CurrRec->fe.time),1*sizeof(mxUINT32_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 1,  mxCreateNumericMatrix(1,1, mxINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 1)) ,&(CurrRec->fe.type),1*sizeof(mxINT16_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 2,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 2)) ,&(CurrRec->fe.read),1*sizeof(mxUINT16_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 3,  mxCreateNumericMatrix(1,1, mxUINT32_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 3)) ,&(CurrRec->fe.sttime),1*sizeof(mxUINT32_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 4,  mxCreateNumericMatrix(1,1, mxUINT32_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 4)) ,&(CurrRec->fe.entime),1*sizeof(mxUINT32_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 5,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 5)) ,&(CurrRec->fe.hstx),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 6,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 6)) ,&(CurrRec->fe.hsty),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 7,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 7)) ,&(CurrRec->fe.gstx),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 8,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 8)) ,&(CurrRec->fe.gsty),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 9,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 9)) ,&(CurrRec->fe.sta),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 10,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 10)) ,&(CurrRec->fe.henx),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 11,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 11)) ,&(CurrRec->fe.heny),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 12,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 12)) ,&(CurrRec->fe.genx),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 13,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 13)) ,&(CurrRec->fe.geny),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 14,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 14)) ,&(CurrRec->fe.ena),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 15,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 15)) ,&(CurrRec->fe.havx),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 16,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 16)) ,&(CurrRec->fe.havy),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 17,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 17)) ,&(CurrRec->fe.gavx),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 18,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 18)) ,&(CurrRec->fe.gavy),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 19,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 19)) ,&(CurrRec->fe.ava),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 20,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 20)) ,&(CurrRec->fe.avel),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 21,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 21)) ,&(CurrRec->fe.pvel),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 22,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 22)) ,&(CurrRec->fe.svel),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 23,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 23)) ,&(CurrRec->fe.evel),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 24,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 24)) ,&(CurrRec->fe.supd_x),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 25,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 25)) ,&(CurrRec->fe.eupd_x),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 26,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 26)) ,&(CurrRec->fe.supd_y),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 27,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 27)) ,&(CurrRec->fe.eupd_y),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 28,  mxCreateNumericMatrix(1,1, mxINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 28)) ,&(CurrRec->fe.eye),1*sizeof(mxINT16_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 29,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 29)) ,&(CurrRec->fe.status),1*sizeof(mxUINT16_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 30,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 30)) ,&(CurrRec->fe.flags),1*sizeof(mxUINT16_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 31,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 31)) ,&(CurrRec->fe.input),1*sizeof(mxUINT16_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 32,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 32)) ,&(CurrRec->fe.buttons),1*sizeof(mxUINT16_CLASS) );

	mxSetFieldByNumber(mxFEVENT, NofFEVENT, 33,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxFEVENT, NofFEVENT, 33)) ,&(CurrRec->fe.parsedby),1*sizeof(mxUINT16_CLASS) );

	if (CurrRec->fe.message != NULL) mxSetField(mxFEVENT, NofFEVENT, "message",  mxCreateString(  &(CurrRec->fe.message->c) ) );
	mxSetField(mxFEVENT, NofFEVENT, "codestring",  mxCreateString(GetRecordCodeStr(GetDataCode())));

	NofFEVENT++;

	return 0;

};


/*************************************
*	Functions and objects to handle events of type IOEVENT
*************************************/

 struct 	IOEVENTtype
{
	const char * fieldnames[3];

	IOEVENTtype(){

			fieldnames[0] = "time";
			fieldnames[1] = "type";
			fieldnames[2] = "data";
	};

} strIOEVENT;

int mxIOEVENTappend()
{

	mxSetFieldByNumber(mxIOEVENT, NofIOEVENT, 0,  mxCreateNumericMatrix(1,1, mxUINT32_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxIOEVENT, NofIOEVENT, 0)) ,&(CurrRec->io.time),1*sizeof(mxUINT32_CLASS) );

	mxSetFieldByNumber(mxIOEVENT, NofIOEVENT, 1,  mxCreateNumericMatrix(1,1, mxINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxIOEVENT, NofIOEVENT, 1)) ,&(CurrRec->io.type),1*sizeof(mxINT16_CLASS) );

	mxSetFieldByNumber(mxIOEVENT, NofIOEVENT, 2,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxIOEVENT, NofIOEVENT, 2)) ,&(CurrRec->io.data),1*sizeof(mxUINT16_CLASS) );
	mxSetField(mxIOEVENT, NofIOEVENT, "codestring",  mxCreateString(GetRecordCodeStr(GetDataCode())));

	NofIOEVENT++;

	return 0;

};


/*************************************
*	Functions and objects to handle events of type RECORDINGS
*************************************/

 struct 	RECORDINGStype
{
	const char * fieldnames[11];

	RECORDINGStype(){

			fieldnames[0] = "time";
			fieldnames[1] = "sample_rate";
			fieldnames[2] = "eflags";
			fieldnames[3] = "sflags";
			fieldnames[4] = "state";
			fieldnames[5] = "record_type";
			fieldnames[6] = "pupil_type";
			fieldnames[7] = "recording_mode";
			fieldnames[8] = "filter_type";
			fieldnames[9] = "pos_type";
			fieldnames[10] = "eye";
	};

} strRECORDINGS;

int mxRECORDINGSappend()
{

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 0,  mxCreateNumericMatrix(1,1, mxUINT32_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 0)) ,&(CurrRec->rec.time),1*sizeof(mxUINT32_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 1,  mxCreateNumericMatrix(1,1, mxSINGLE_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 1)) ,&(CurrRec->rec.sample_rate),1*sizeof(mxSINGLE_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 2,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 2)) ,&(CurrRec->rec.eflags),1*sizeof(mxUINT16_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 3,  mxCreateNumericMatrix(1,1, mxUINT16_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 3)) ,&(CurrRec->rec.sflags),1*sizeof(mxUINT16_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 4,  mxCreateNumericMatrix(1,1, mxUINT8_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 4)) ,&(CurrRec->rec.state),1*sizeof(mxUINT8_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 5,  mxCreateNumericMatrix(1,1, mxUINT8_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 5)) ,&(CurrRec->rec.record_type),1*sizeof(mxUINT8_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 6,  mxCreateNumericMatrix(1,1, mxUINT8_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 6)) ,&(CurrRec->rec.pupil_type),1*sizeof(mxUINT8_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 7,  mxCreateNumericMatrix(1,1, mxUINT8_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 7)) ,&(CurrRec->rec.recording_mode),1*sizeof(mxUINT8_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 8,  mxCreateNumericMatrix(1,1, mxUINT8_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 8)) ,&(CurrRec->rec.filter_type),1*sizeof(mxUINT8_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 9,  mxCreateNumericMatrix(1,1, mxUINT8_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 9)) ,&(CurrRec->rec.pos_type),1*sizeof(mxUINT8_CLASS) );

	mxSetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 10,  mxCreateNumericMatrix(1,1, mxUINT8_CLASS,mxREAL));
	memcpy(mxGetData(mxGetFieldByNumber(mxRECORDINGS, NofRECORDINGS, 10)) ,&(CurrRec->rec.eye),1*sizeof(mxUINT8_CLASS) );
	mxSetField(mxRECORDINGS, NofRECORDINGS, "codestring",  mxCreateString(GetRecordCodeStr(GetDataCode())));

	NofRECORDINGS++;

	return 0;

};

char * GetRecordTypeStr(int CODE)
{

	switch(CODE) {
		case SAMPLE_TYPE:
			return "FSAMPLE";
		case STARTPARSE:
		case ENDPARSE:
		case BREAKPARSE:
		case STARTBLINK :
		case ENDBLINK:
		case STARTSACC:
		case ENDSACC:
		case STARTFIX:
		case ENDFIX:
		case FIXUPDATE:
		case MESSAGEEVENT:
		case STARTSAMPLES:
		case ENDSAMPLES:
		case STARTEVENTS:
		case ENDEVENTS:
			return "FEVENT";
		case BUTTONEVENT:
		case INPUTEVENT:
		case LOST_DATA_EVENT:
			return "IOEVENT";
		case RECORDING_INFO:
			return "RECORDINGS";
	}

	return NULL;

};



int AppendRecord()
{

	CurrRec = edf_get_float_data( edfptr);

	switch(DataCode) {
		case SAMPLE_TYPE:
			return mxFSAMPLEappend();
		case STARTPARSE:
		case ENDPARSE:
		case BREAKPARSE:
		case STARTBLINK :
		case ENDBLINK:
		case STARTSACC:
		case ENDSACC:
		case STARTFIX:
		case ENDFIX:
		case FIXUPDATE:
		case MESSAGEEVENT:
		case STARTSAMPLES:
		case ENDSAMPLES:
		case STARTEVENTS:
		case ENDEVENTS:
			return mxFEVENTappend();
		case BUTTONEVENT:
		case INPUTEVENT:
		case LOST_DATA_EVENT:
			return mxIOEVENTappend();
		case RECORDING_INFO:
			return mxRECORDINGSappend();
	}

	return 0;

};



char * GetRecordCodeStr(int CODE)
{

	switch(CODE){
		case SAMPLE_TYPE:
			return "SAMPLE_TYPE";
		case STARTPARSE:
			return "STARTPARSE";
		case ENDPARSE:
			return "ENDPARSE";
		case BREAKPARSE:
			return "BREAKPARSE";
		case STARTBLINK :
			return "STARTBLINK ";
		case ENDBLINK:
			return "ENDBLINK";
		case STARTSACC:
			return "STARTSACC";
		case ENDSACC:
			return "ENDSACC";
		case STARTFIX:
			return "STARTFIX";
		case ENDFIX:
			return "ENDFIX";
		case FIXUPDATE:
			return "FIXUPDATE";
		case MESSAGEEVENT:
			return "MESSAGEEVENT";
		case STARTSAMPLES:
			return "STARTSAMPLES";
		case ENDSAMPLES:
			return "ENDSAMPLES";
		case STARTEVENTS:
			return "STARTEVENTS";
		case ENDEVENTS:
			return "ENDEVENTS";
		case BUTTONEVENT:
			return "BUTTONEVENT";
		case INPUTEVENT:
			return "INPUTEVENT";
		case LOST_DATA_EVENT:
			return "LOST_DATA_EVENT";
		case RECORDING_INFO:
			return "RECORDING_INFO";
	}

	return NULL;

};



BuildMexArrays( char * filenamein , int consistency_check , int load_events  , int load_samples)
{
	 RecordTypes[0] = "FSAMPLE";
	 RecordTypes[1] = "FEVENT";
	 RecordTypes[2] = "IOEVENT";
	 RecordTypes[3] = "RECORDINGS";

	Initialize( filenamein , consistency_check , load_events  , load_samples);


	OutputMexObject = mxCreateStructMatrix(1,1,4,RecordTypes);



	NofFSAMPLE = 0;

	 mxFSAMPLE = mxCreateStructMatrix(1,1,28,strFSAMPLE.fieldnames);

	mxSetFieldByNumber( mxFSAMPLE,0,0,  mxCreateNumericMatrix(1,Nrec, mxUINT32_CLASS,mxREAL) );
	strFSAMPLE.time = (UINT32 *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,0) );

	mxSetFieldByNumber( mxFSAMPLE,0,1,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.px = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,1) );

	mxSetFieldByNumber( mxFSAMPLE,0,2,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.py = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,2) );

	mxSetFieldByNumber( mxFSAMPLE,0,3,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.hx = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,3) );

	mxSetFieldByNumber( mxFSAMPLE,0,4,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.hy = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,4) );

	mxSetFieldByNumber( mxFSAMPLE,0,5,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.pa = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,5) );

	mxSetFieldByNumber( mxFSAMPLE,0,6,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.gx = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,6) );

	mxSetFieldByNumber( mxFSAMPLE,0,7,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.gy = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,7) );

	mxSetFieldByNumber( mxFSAMPLE,0,8,  mxCreateNumericMatrix(1,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.rx = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,8) );

	mxSetFieldByNumber( mxFSAMPLE,0,9,  mxCreateNumericMatrix(1,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.ry = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,9) );

	mxSetFieldByNumber( mxFSAMPLE,0,10,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.gxvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,10) );

	mxSetFieldByNumber( mxFSAMPLE,0,11,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.gyvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,11) );

	mxSetFieldByNumber( mxFSAMPLE,0,12,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.hxvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,12) );

	mxSetFieldByNumber( mxFSAMPLE,0,13,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.hyvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,13) );

	mxSetFieldByNumber( mxFSAMPLE,0,14,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.rxvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,14) );

	mxSetFieldByNumber( mxFSAMPLE,0,15,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.ryvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,15) );

	mxSetFieldByNumber( mxFSAMPLE,0,16,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.fgxvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,16) );

	mxSetFieldByNumber( mxFSAMPLE,0,17,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.fgyvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,17) );

	mxSetFieldByNumber( mxFSAMPLE,0,18,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.fhxvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,18) );

	mxSetFieldByNumber( mxFSAMPLE,0,19,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.fhyvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,19) );

	mxSetFieldByNumber( mxFSAMPLE,0,20,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.frxvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,20) );

	mxSetFieldByNumber( mxFSAMPLE,0,21,  mxCreateNumericMatrix(2,Nrec, mxSINGLE_CLASS,mxREAL) );
	strFSAMPLE.fryvel = (single *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,21) );

	mxSetFieldByNumber( mxFSAMPLE,0,22,  mxCreateNumericMatrix(8,Nrec, mxINT16_CLASS,mxREAL) );
	strFSAMPLE.hdata = (INT16 *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,22) );

	mxSetFieldByNumber( mxFSAMPLE,0,23,  mxCreateNumericMatrix(1,Nrec, mxUINT16_CLASS,mxREAL) );
	strFSAMPLE.flags = (UINT16 *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,23) );

	mxSetFieldByNumber( mxFSAMPLE,0,24,  mxCreateNumericMatrix(1,Nrec, mxUINT16_CLASS,mxREAL) );
	strFSAMPLE.input = (UINT16 *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,24) );

	mxSetFieldByNumber( mxFSAMPLE,0,25,  mxCreateNumericMatrix(1,Nrec, mxUINT16_CLASS,mxREAL) );
	strFSAMPLE.buttons = (UINT16 *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,25) );

	mxSetFieldByNumber( mxFSAMPLE,0,26,  mxCreateNumericMatrix(1,Nrec, mxINT16_CLASS,mxREAL) );
	strFSAMPLE.htype = (INT16 *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,26) );

	mxSetFieldByNumber( mxFSAMPLE,0,27,  mxCreateNumericMatrix(1,Nrec, mxUINT16_CLASS,mxREAL) );
	strFSAMPLE.errors = (UINT16 *) mxGetData( mxGetFieldByNumber(mxFSAMPLE,0,27) );


	NofFEVENT = 0;

	 mxFEVENT = mxCreateStructMatrix(1,Nrec,35,strFEVENT.fieldnames);

	 mxAddField(mxFEVENT,"codestring");


	NofIOEVENT = 0;

	 mxIOEVENT = mxCreateStructMatrix(1,Nrec,3,strIOEVENT.fieldnames);

	 mxAddField(mxIOEVENT,"codestring");


	NofRECORDINGS = 0;

	 mxRECORDINGS = mxCreateStructMatrix(1,Nrec,11,strRECORDINGS.fieldnames);

	 mxAddField(mxRECORDINGS,"codestring");

};

int CreateMexStruc()
{

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,0) ,strFSAMPLE.time );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,0),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,1) ,strFSAMPLE.px );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,1),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,2) ,strFSAMPLE.py );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,2),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,3) ,strFSAMPLE.hx );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,3),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,4) ,strFSAMPLE.hy );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,4),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,5) ,strFSAMPLE.pa );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,5),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,6) ,strFSAMPLE.gx );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,6),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,7) ,strFSAMPLE.gy );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,7),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,8) ,strFSAMPLE.rx );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,8),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,9) ,strFSAMPLE.ry );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,9),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,10) ,strFSAMPLE.gxvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,10),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,11) ,strFSAMPLE.gyvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,11),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,12) ,strFSAMPLE.hxvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,12),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,13) ,strFSAMPLE.hyvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,13),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,14) ,strFSAMPLE.rxvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,14),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,15) ,strFSAMPLE.ryvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,15),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,16) ,strFSAMPLE.fgxvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,16),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,17) ,strFSAMPLE.fgyvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,17),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,18) ,strFSAMPLE.fhxvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,18),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,19) ,strFSAMPLE.fhyvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,19),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,20) ,strFSAMPLE.frxvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,20),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,21) ,strFSAMPLE.fryvel );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,21),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,22) ,strFSAMPLE.hdata );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,22),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,23) ,strFSAMPLE.flags );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,23),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,24) ,strFSAMPLE.input );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,24),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,25) ,strFSAMPLE.buttons );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,25),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,26) ,strFSAMPLE.htype );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,26),NofFSAMPLE);

	mxSetData( mxGetFieldByNumber( mxFSAMPLE,0,27) ,strFSAMPLE.errors );
	mxSetN(mxGetFieldByNumber(mxFSAMPLE,0,27),NofFSAMPLE);

	mxSetField(OutputMexObject,0,"FSAMPLE",mxFSAMPLE);

	mxSetN(mxFEVENT,NofFEVENT);
	mxSetField(OutputMexObject,0,"FEVENT",mxFEVENT);

	mxSetN(mxIOEVENT,NofIOEVENT);
	mxSetField(OutputMexObject,0,"IOEVENT",mxIOEVENT);

	mxSetN(mxRECORDINGS,NofRECORDINGS);
	mxSetField(OutputMexObject,0,"RECORDINGS",mxRECORDINGS);

	return 0;
};

};//end of class def

