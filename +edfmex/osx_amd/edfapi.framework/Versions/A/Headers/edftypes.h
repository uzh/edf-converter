/**
 * Copyright (c) 1996-2023, SR Research Ltd., All Rights Reserved
 *
 * For use by SR Research licencees only. Redistribution and use in source
 * and binary forms, with or without modification, are NOT permitted.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the distribution.
 *
 * Neither name of SR Research Ltd nor the name of contributors may be used
 * to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*  EYELINK APPLICATION CODE      */
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
