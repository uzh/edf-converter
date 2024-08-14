/*  EYELINK APPLICATION CODE      */
/*  (c) 1995 by SR Research Ltd.  */
/*  9 Dec '95 by Dave Stampe      */
/*  PLATFORM-PORTABLE TYPES       */
/*  MAY NEED TO BE MODIFIED       */
/*  FOR Mac, etc.                 */

/*#define FARTYPE _far */  /* for some mixed-model builds */
#define FARTYPE            /* make blank for most DOS, 32-bit, ANSI C */

#ifdef __cplusplus 	/* For C++ definitions */
extern "C" {
#endif

#ifndef _BASETSD_H_ /* windows header */
	#ifndef BYTEDEF
		#define BYTEDEF 1

		typedef unsigned char  byte;
		typedef short          INT16;
		typedef int           INT32;
		typedef unsigned short UINT16;
		typedef unsigned int  UINT32;
	#endif
#endif

#ifndef _MSC_VER
typedef unsigned long long UINT64 ;
typedef long long INT64 ;
#else
typedef unsigned __int64 UINT64 ;
typedef __int64 INT64 ;
#endif

#ifndef MICRODEF
	#define MICRODEF 1
	typedef struct {
		INT32  msec;	/* SIGNED for offset computations */
		INT16  usec;
	} MICRO ;
#endif

#ifdef __cplusplus	/* For C++ definitions */
}
#endif
