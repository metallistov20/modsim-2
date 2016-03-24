/*
 (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT any WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 MA 02111-1307 USA
*/

/* stdout, NULL */
#include <stdio.h>

/* strcpy() */
#include <string.h>

/* getopt_long() */
#include <getopt.h>

/* abort(), SIGABT */
#include <stdlib.h>


/* Own interface, definitions */
#include "modsim.h"

/* Data structures, mocros */
#include "datastruct.h"

/* Port D definitions and prototypes */
#include "port_d.h"

/* Pointer to raw data file */
FILE *fp = NULL;

/* Bufer to store scanned raw data */
char cBuf [LARGE_BUF_SZ];

#if !defined(QUASIFLOAT) 
	float fltTM, fltDIn, fltDOut;
#else
	QuasiFloatType qfltTM, qfltDIn, qfltDOut;
#endif /* !defined(QUASIFLOAT) */

/* Pointer to a dynamic structure to store raw data to */
pTimepointType pTimeChain;

/* Aux. var to tell whether it's a first time we output a message for this 'second'  */
int iOldSec;


/* Single operation names */
static struct option long_options[] =
{
	/* USB 1.1 on CPE#0 Gate */
	{"USB1",  no_argument, 		0,'1'},

	/* USB 2.0 on CPE#1 Gate */
	{"USB2",  no_argument, 		0,'2'},

	/* End of array */
	{0, 0, 0, 0}
};

#if defined(UCSIMM)

/* Old UCLIBC not have <abort()> within <stdlib>  */
void abort()
{
	exit (-1);
}
#define SWAP16(x) 	((unsigned short)	((x&0xFF00)>>8)	|	((x&0x00FF)<<8)			)
#endif /* (defined(UCSIMM) ) */

int main (int argc, char **argv)
{
int iOperation, iOption;

/* Index of the option */
int iOptionIdx = 0;

char cArg0[LARGE_BUF_SZ];

#if defined(HW_PORTD_TEST)

/* Set digital PIOs 1-4 as outputs */
PortD_Prepare( );

/* Set up and set down, eternally */
//while (1) { PortD_Toggle(0xF0 ); usleep(10) ; }
// PRIOD: 24.8 us, Freq: 40 kHz   while (1) { PortD_Toggle(0xF0 );  }
// PRIOD: 560.0 us, Freq: 1.8 kHz  <== iHundred = 25;
// PRIOD: 146.0 us, Freq:   <== iHundred = 5;
// PRIOD: 64.0 us, Freq: 15kHz  <== iHundred = 1;
//while (1) {
//volatile int iHundred;
//volatile int iAlfa;
//iHundred = 1; while (iHundred--) {iAlfa += 1;} PortD_Toggle(0xF0 );
//}


// PRIOD: 40.0 us, Freq: 25 kHz  <== iHundred = 1;
while (1) {
volatile int iHundred=1;
//do {;} while (iHundred--);  PortD_Toggle(0xF0 );
iHundred--;  PortD_Toggle(0xF0 );
}
// while (1) {volatile int iHundred=1; iHundred--; }

#endif /* (defined(HW_DUMB_TEST) ) */

#if defined(HW_AD53_TEST)

/* Also it prepares PIOs, theres no need to do <PortD_Prepare()> */
ConverterInit();

/* Checking on values close to limit, on limits, in the middle, and at random valuoes from range  */
//while (1) { ConverterWrite( 0 ) ; ConverterWrite( 255);  }
//while (1) {volatile int iHundred = 100; ConverterWrite0FF0( 0 ) ; while (iHundred--) {iHundred += 0;}   }
//while (1) {volatile int iHundred = 25; ConverterWrite0000( 0 ) ; while (iHundred--) {iHundred += 0;}    }

while (1) {
volatile int iHundred;
volatile int iAlfa;

iHundred = 150*3; while (iHundred--) {iAlfa += 1;}
	ConverterWrite( 0 );// ConverterWrite0000( 0 );
	iHundred = 150; while (iHundred--) {iAlfa += 1;}
	ConverterWrite(0x11);//ConverterWrite0110( 0 );
	iHundred = 150; while (iHundred--) {iAlfa += 1;}
	ConverterWrite(0xAA);//ConverterWrite0AA0( 0 ) ;
	iHundred = 150; while (iHundred--) {iAlfa += 1;}

	ConverterWrite(0xFF);//ConverterWrite0FF0( 0 ) ;
	iHundred = 150; while (iHundred--) {iAlfa += 1;}

iHundred = 150*3; while (iHundred--) {iAlfa += 1;}
	ConverterWrite0000( 0 );
	iHundred = 150; while (iHundred--) {iAlfa += 1;}
	ConverterWrite0110( 0 );
	iHundred = 150; while (iHundred--) {iAlfa += 1;}
	ConverterWrite0AA0( 0 ) ;
	iHundred = 150; while (iHundred--) {iAlfa += 1;}
	ConverterWrite0FF0( 0 ) ;
	iHundred = 150; while (iHundred--) {iAlfa += 1;}

}

#endif /* (defined(HW_DUMB_TEST) ) */

	/* Avoid dafault 0 value */
	iOperation=DO_NO_OP;

	/* Assign program name, requirted for output*/
	strcpy (cArg0, argv[0]);

	/* Parsing command line arguments */
	while (1)
	{
		/* Get each paramter */
		iOption = getopt_long (argc, argv, "12", long_options, &iOptionIdx);

		/* Unsless under current arch're. TODO: check & remove */
		iOptionIdx = 0;

		/* End of the options reached? */
		if (-1 == iOption)
		{
			/* Then break the parsing cycle */
			break;
		}

		/* Parce each parameter */
		switch (iOption)
		{
			/* Single: open site */
			case '1':
				printf("%s: OK. option is  <%c>\n", cArg0, iOption);
				iOperation = DO_GATE0_OP;
				break;

			/* Single: close site */
			case '2':
				printf("%s: OK. option is  <%c>\n", cArg0, iOption);
				iOperation = DO_GATE1_OP;
				break;

			case '?':
				/* getopt_long prints error message, we don't */
				break;

			default:
				printf("%s: bad usage, exiting", cArg0);
				abort ();
		}
	} /* Command line arguments were parsed */

	/* Try to open Raw Data file at place defined by 'FILE_NAME' */
	if ( NULL == (fp = fopen (FILE_NAME, "r") ) )
	{
		printf("[%s] %s: can't open file <%s> \n", __FILE__, __func__ , FILE_NAME);

		return P_ERROR;
	}

	printf("[%s] %s: loading USB-curve-data via NFS from file <%s>\n", __FILE__, __func__, FILE_NAME);

	/* For each string of Raw Data file */
	while ( ! (feof (fp) ) ) 
	{
		/* Try to scan a whole string into temp. buffer */
		if (0 > fscanf (fp, "%s", cBuf ) )
		{
			// eof reached
		}
		else
		{
		/* Aux. buffer to keep results of parsing */
		char * cpTmp = cBuf;
#if DEBUG_DATA
			printf("[%s] %s: scanned: < %s >\n", __FILE__, __func__, cBuf);
#endif /* (DEBUG_DATA) */

#if !defined(QUASIFLOAT) 
			/* Set default values. MISRA RULE #TODO */
			fltTM = fltDIn = fltDOut = 0.0f;
#else
			/* Set default values. MISRA RULE #TODO */
			memset (&qfltTM, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDIn, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDOut, 0, sizeof (QuasiFloatType) ) ;

#endif /* !defined(QUASIFLOAT) */

			/* For each character in aux. buffer */
			while (*cpTmp)

				/* replace all commas with spaces, to let the <scanf()> parse it */
				{ if (',' == *cpTmp) *cpTmp = ' '; cpTmp++; }
	

#if !defined(QUASIFLOAT) 
			/* Find 3 floats separated by spaces in aux. buffer */
			sscanf(cBuf, "%f %f %f,", &fltTM,     &fltDIn,   &fltDOut );
#else
			/* Find 3 floats separated by spaces in aux. buffer. Each float represented as <INT>.<INT>e<SIGN>0<INT> */
			sscanf(cBuf, "%d.%de%c0%d %d.%de%c0%d %d.%de%c0%d,",
					&(qfltTM.integer),&(qfltTM.fraction),&(qfltTM.sgn),&(qfltTM.power),
					&(qfltDIn.integer),&(qfltDIn.fraction),&(qfltDIn.sgn),&(qfltDIn.power),
					&(qfltDOut.integer),&(qfltDOut.fraction),&(qfltDOut.sgn),&(qfltDOut.power)  );

/* For each 'second' value do output. Only once. */ /* TODO: remove? */
if (0 == qfltTM.power) if (iOldSec!= qfltTM.integer){iOldSec=qfltTM.integer; printf("sec: %d; ", iOldSec); fflush(stdout); }

#endif /* !defined(QUASIFLOAT) */

#if DEBUG_DATA
#if !defined(QUASIFLOAT) 
			printf("[%s] %s: parsed :  <%f> <%f> <%f>\n", __FILE__, __func__, fltTM, fltDIn, fltDOut );
#else
			printf("[%s] %s: parsed :  <%d.%de%c0%d>  <%d.%de%c0%d>  <%d.%de%c0%d> \n",
					__FILE__, __func__, 
					qfltTM.integer,qfltTM.fraction,qfltTM.sgn,qfltTM.power,
					qfltDIn.integer,qfltDIn.fraction,qfltDIn.sgn,qfltDIn.power,
					qfltDOut.integer,qfltDOut.fraction,qfltDOut.sgn,qfltDOut.power  );
#endif /* !defined(QUASIFLOAT) */
#endif /* (DEBUG_DATA) */

#if !defined(QUASIFLOAT) 
			/* Attach just scanned data (three floats) to tail of dynamic structure */
			EnrollPoint(&pTimeChain, &fltTM, &fltDIn, &fltDOut, "N/A");
#else
			/* Attach just scanned data (three floats represented as <INT>.<INT>e<SIGN>0<INT>) to tail of dynamic structure */
			EnrollPoint(&pTimeChain, &qfltTM, &qfltDIn, &qfltDOut, "N/A");
#endif /* !defined(QUASIFLOAT) */
		}
	}

	/* Dispose pointer to Raw Data file */
	fclose(fp);

	printf("\n[%s] %s: issuing USB-curve-data on Pin #0 Port 'D'\n", __FILE__, __func__);

#if defined(UCSIMM) 
	/* Set pins of Port D as inputs/outputs, sets rest platform registers */
	PortD_Prepare( );
#endif /* (defined(UCSIMM) ) */

	/* Process data stored in dynamic structure pointed by 'pTimeChain' */
	ProcessPoints(pTimeChain);

	printf("\n[%s] %s: disposing memory allocations\n", __FILE__, __func__);

	/* Free memory occupied by dynamically stored raw data */
	DeletePoints(&pTimeChain);

	printf("[%s] %s: done (success) \n", __FILE__, __func__); fflush(stdout);

	return P_SUCCESS;
}


