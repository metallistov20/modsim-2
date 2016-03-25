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

/* printf(), stdout */
#include <stdio.h>

/* realloc() */
#include <stdlib.h>

/* struct timeval, gettimeofday() */
#include <sys/time.h>

/* Basic project definitions  */
#include "modsim.h"

/* Type 'pTimepointType' definition, <iFIRST> integer */
#include "datastruct.h"

/* PortD_Up(), PortD_Down(), PD0, PD1 */
#include "port_d.h"

/* Time measurement variable to define current position on time scale */
struct timeval endtimePROC;

/* Var. defined in 'datastruct.c' */
extern struct timeval starttimePROC;

/* Var. to check if current second already was 'displayed' */
int iOldSecPRC;

/* Anything under this voltage is regarded to be <logical 0> for USB 1.1 */
#define LOGIC_0_CURR		0.4
/* Integer part of <LOGIC_0_CURR> */
#define LOGIC_0_CURR_INTGR	0
/* Fraction  of <LOGIC_0_CURR> */
#define LOGIC_0_CURR_FRACT	4

/* Anything above this voltage is regarded to be <logical 1> for USB 1.1 */
#define LOGIC_1_CURR		2.4
/* Integer part of <LOGIC_1_CURR> */
#define LOGIC_1_CURR_INTGR	2
/* Fraction  of <LOGIC_1_CURR> */
#define LOGIC_1_CURR_FRACT	4

/* Check if raw value is USB 1.0 <logical 0> and return '1' if so, return '0' otherwise */
int iChkUsb10Lg0( QuasiFloatType qfltVal )
{
	/* USB 1.0 levels. Logical '0'. LOGIC_0_CURR */
	if (
		(USB11_LOGIC_0_CURR_INTGR == qfltVal.integer) && (USB11_LOGIC_0_CURR_FRACT >= qfltVal.fraction) 

	) return 1;

	return 0;
}

/* Check if raw value is USB 1.0 <logical 1> and return '1' if so, return '0' otherwise */
int iChkUsb10Lg1( QuasiFloatType qfltVal )
{
	/* USB 1.0 levels. Logical '1'.  LOGIC_1_CURR */
	if (
		(  (USB11_LOGIC_1_CURR_INTGR == qfltVal.integer) && (USB11_LOGIC_1_CURR_FRACT <= qfltVal.fraction)   ) 
			||
		(    USB11_LOGIC_1_CURR_INTGR <= qfltVal.integer   )    

	) return 1;

	return 0;
}

/* Check if raw value is USB 2.0 <logical 0> and return '1' if so, return '0' otherwise */
int iChkUsb20Lg0(QuasiFloatType qfltVal)
{
	/* USB 2.0 levels. Logical '0'. LOGIC_0_CURR. -10 mV .. 10 mV */
	if (
		(  (USB20_LOGIC_0_LO_CURR_INTGR == qfltVal.integer) && (USB20_LOGIC_0_LO_CURR_FRACT >= qfltVal.fraction)   ) 		
		/* Values from raange -10mV .. 0V are here due inability to keep integer <-0.0> inside <qfltVal.integer> . TODO: resolve */

	)  return 1;

	return 0;
}

/* Check if raw value is USB 2.0 <logical 1> and return '1' if so, return '0' otherwise */
int iChkUsb20Lg1(QuasiFloatType qfltVal)
{
	/* USB 2.0 levels. Logical '1'. LOGIC_1_CURR. 0.36V .. 0.44V */
	if (
		(  (USB20_LOGIC_1_LO_CURR_INTGR == qfltVal.integer) && (USB20_LOGIC_1_LO_CURR_FRACT <= qfltVal.fraction)   ) 
		&& 
		(  (USB20_LOGIC_1_UP_CURR_INTGR == qfltVal.integer) && (USB20_LOGIC_1_UP_CURR_FRACT >= qfltVal.fraction)   ) 

	) return 1;

	return 0;
}

/*  Check if current value is <logical 0> in terms of appropriate USB protocol */
int iChkUsbLg0(QuasiFloatType qfltVal)
{
	/* it is assumes that here the <iOperation> value is either <DO_GATE0_OP> or <DO_GATE1_OP> */

	if (DO_GATE0_OP == iOperation)

		return iChkUsb10Lg0(qfltVal);
	else

		return iChkUsb20Lg0(qfltVal);
}

/*  Check if current value is <logical 1> in terms of appropriate USB protocol */
int iChkUsbLg1(QuasiFloatType qfltVal)
{
	/* it is assumes that here the <iOperation> value is either <DO_GATE0_OP> or <DO_GATE1_OP> */

	if (DO_GATE0_OP == iOperation)

		return iChkUsb10Lg1(qfltVal);
	else

		return iChkUsb20Lg1(qfltVal);
}

int ProcessPoint( pTimepointType pTimepoint )
{

QuasiFloatType qfltRelTime;
int _left, _right;
QuasiFloatType qfltJiffy; 
qfltJiffy.fraction = 1;

#if 0
OBSOLETE
	/* Put marquee 'secPRC: xxx;' on the screen, so we are sure platform is still not hanged */
	if (0 == pTimepoint->qfltAbsTime.power)
#endif
		if (iOldSecPRC!= pTimepoint->qfltAbsTime.integer)

			{iOldSecPRC=pTimepoint->qfltAbsTime.integer; printf("secPRC: %d; ", iOldSecPRC); fflush(stdout); }



//printf("[%s] :  qfltAbsTime.integer = %d , qfltAbsTime.fraction = %d \n", __FILE__,pTimepoint->qfltAbsTime.integer, pTimepoint->qfltAbsTime.fraction );

	pTimepoint->qfltAbsTime.integer = (pTimepoint->qfltAbsTime.integer < 0)?
		(pTimepoint->qfltAbsTime.integer * 1000000) - (pTimepoint->qfltAbsTime.fraction):
		(pTimepoint->qfltAbsTime.integer * 1000000) + (pTimepoint->qfltAbsTime.fraction);

//printf("[%s] :  qfltAbsTime.integer = %d (PR) ,  \n", __FILE__,	pTimepoint->qfltAbsTime.integer );

	/* Take current time */
	gettimeofday(&endtimePROC,0);

	/* Compute time elapsed since head of list processing till now */
	qfltRelTime.integer = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec) 
		+ endtimePROC.tv_usec - starttimePROC.tv_usec - iFIRST;

//printf("[%s] :  qfltRelTime.integer = %d \n\n", __FILE__,qfltRelTime.integer );

#if defined(FAST_UCSIMM)
	printf("[%s] : <BEFORE TIME SHIFTING> real tm.: %d, shiftable tm.: %d \n", __FILE__,
		pTimepoint->qfltAbsTime.integer,	qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */

	/* If relative time stays on the left from 0 */
	if (qfltRelTime.integer < 0)

		/* then '_right' is relative time, '_left' is real time */
		_right = qfltRelTime.integer, _left = pTimepoint->qfltAbsTime.integer;

	/* If relative time stays on the right from 0, or is exctly 0 */
	else
		/* then '_right' is real time, '_left' is relative time */
		_left = qfltRelTime.integer, _right = pTimepoint->qfltAbsTime.integer;
#if 0
	if (0 == pTimepoint->qfltAbsTime.integer) return;

	/* TODO: make <do-while> instead of <while-do>, thus avoid a code duplication */
	while (_right < _left )
	{
		/* Wait for relative <fltRelTime> to catch up with absolute <fltRealTime>  */
		usleep (qfltJiffy.fraction);

		/* Take current time */
		gettimeofday(&endtimePROC,0);

		/* Compute how much time elapsed since head of list processing till now */
		qfltRelTime.integer = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec) 

			+ endtimePROC.tv_usec - starttimePROC.tv_usec  - iFIRST;

		/* If relative time stays on the left from 0 */
		if (qfltRelTime.integer < 0)

			/* then '_right' is relative time, '_left' is real time */
			_right = qfltRelTime.integer, _left = pTimepoint->qfltAbsTime.integer;

		/* If relative time stays on the right from 0, or is exctly 0 */
		else

			/* then '_right' is real time, '_left' is relative time */
			_left = qfltRelTime.integer, _right = pTimepoint->qfltAbsTime.integer;

#if defined(FAST_UCSIMM)
		printf("[%s] : <TIME SHIFTING> real tm.: %d, shiftable tm.: %d \n", __FILE__,
		pTimepoint->qfltAbsTime.integer,	qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */
	}
#endif /* (0) */

#if defined(FAST_UCSIMM)
	/* Now they're equal or least 'relative tm' is not less than 'real tm' */
	printf("[%s] : <AFTER TIME SHIFTING> will pretend like <%d>, is same as <%d> \n", __FILE__,
		pTimepoint->qfltAbsTime.integer,
		qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */

		/* Logical '1'.  LOGIC_1_CURR */
		if ( iChkUsbLg1(pTimepoint->qfltYval) )

			/* Pull terminal USB#0/USB#1 <dIN> line up. */
			Term_Up( );

		else
			/* Logical '0'. LOGIC_0_CURR */
			if ( iChkUsbLg0(pTimepoint->qfltYval) )

				/* Pull terminal USB#0/USB#1 <dIN> line down. */
				Term_Down( );
#if 0
			else
			{
				/* Over-voltages, under-voltages, some middle values and rest are marked as <not processed> */
#if 0
				TODO: find this realloc() within UCLIBC:
				pTimepoint->pcMarquee = realloc (pTimepoint->pcMarquee, strlen (UNPROC) +1 );
#else
				free(pTimepoint->pcMarquee);
				pTimepoint->pcMarquee = NULL;// TODO: really needed, or can be left for <mman>
				pTimepoint->pcMarquee = malloc (strlen (UNPROC) +1 );
#endif /* (0) */
				strcpy( pTimepoint->pcMarquee, UNPROC);
			}
#endif /* (0) */


#if defined(DIN_FEEDBACK)
	;
#endif /* (DIN_FEEDBACK) */


}
