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

/* struct timeval, gettimeofday() */
#include <sys/time.h>

/* Basic project definitions  */
#include "modsim.h"

/* Type 'pTimepointType' definition */
#include "datastruct.h"

/* PortD_Up(), PortD_Down(), PD0, PD1 */
#include "port_d.h"

/* Time measurement variable to define current position on time scale */
struct timeval endtimePROC;

/* Var. defined in 'datastruct.c' */
extern struct timeval starttimePROC;

/* Var. to check if current second already was 'displayed' */
int iOldSecPRC;


/* Check if raw value is USB 1.0 <logical 0> and return '1' if so, return '0' otherwise */
int iChkUsb10Lg0( QuasiFloatType qfltVal )
{
	/* USB 1.0 levels. Logical '0'. LOGIC_0_CURR */
	if (
		('+' == qfltVal.sgn) && (0 == qfltVal.power) &&

		 (  (0 == qfltVal.integer)&&(4 >= qfltVal.fraction)   ) 

	) return 1;

	return 0;
}

/* Check if raw value is USB 1.0 <logical 1> and return '1' if so, return '0' otherwise */
int iChkUsb10Lg1( QuasiFloatType qfltVal )
{
	/* USB 1.0 levels. Logical '1'.  LOGIC_1_CURR */
	if (
		('+' == qfltVal.sgn) && (0 == qfltVal.power) &&
		 (  (2 == qfltVal.integer)&&(4 <= qfltVal.fraction)   ) ||  (    3 <= qfltVal.integer   )    
	) return 1;

	return 0;
}

int ProcessPoint( pTimepointType pTimepoint )
{

QuasiFloatType qfltRelTime;
int _left, _right;
QuasiFloatType qfltJiffy; 
qfltJiffy.fraction = 1;


	pTimepoint->qfltAbsTime.integer = (pTimepoint->qfltAbsTime.integer < 0)?
		(pTimepoint->qfltAbsTime.integer * 1000000) - (pTimepoint->qfltAbsTime.fraction / 10):
		(pTimepoint->qfltAbsTime.integer * 1000000) + (pTimepoint->qfltAbsTime.fraction / 10);

	/* Take current time */
	gettimeofday(&endtimePROC,0);

	/* Compute time elapsed since head of list processing till now */
	qfltRelTime.integer = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec - 6.0) 
		+ endtimePROC.tv_usec - starttimePROC.tv_usec;


#if defined(FAST_UCSIMM)
	printf("[%s] : <BEFORE TIME SHIFTING> real tm.: %d, shiftable tm.: %d \n", __FILE__,
		pTimepoint->qfltAbsTime.integer,	qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */

	/* TODO: resolve bug. On (-1.0 .. 1.0) comparing <qfltRelTime.integer> against 0 is not sufficient. */

	/* If relative time stays on the left from 0 */
	if (qfltRelTime.integer < 0)

		/* then '_right' is relative time, '_left' is real time */
		_right = qfltRelTime.integer, _left = pTimepoint->qfltAbsTime.integer;

	/* If relative time stays on the right from 0, or is exctly 0 */
	else
		/* then '_right' is real time, '_left' is relative time */
		_left = qfltRelTime.integer, _right = pTimepoint->qfltAbsTime.integer;

	if (0 == pTimepoint->qfltAbsTime.integer) return;

	/* TODO: make <do-while> instead of <while-do>, thus avoid a code duplication */
	while (_right < _left )
	{
		/* Wait for relative <fltRelTime> to catch up with absolute <fltRealTime>  */
		usleep (qfltJiffy.fraction);

		/* Take current time */
		gettimeofday(&endtimePROC,0);

		/* Compute how much time elapsed since head of list processing till now */
		qfltRelTime.integer = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec - 6.0) 

			+ endtimePROC.tv_usec - starttimePROC.tv_usec;

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

#if defined(FAST_UCSIMM)
	/* Now they're equal or least 'relative tm' is not less than 'real tm' */
	printf("[%s] : <AFTER TIME SHIFTING> will pretend like <%d>, is same as <%d> \n", __FILE__,
		pTimepoint->qfltAbsTime.integer,
		qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */

		/* Put marquee 'secPRC: xxx;' on the screen, so we are sure platform is still not hanged */
		if (0 == pTimepoint->qfltAbsTime.power)

			if (iOldSecPRC!= pTimepoint->qfltAbsTime.integer)

				{iOldSecPRC=pTimepoint->qfltAbsTime.integer; printf("secPRC: %d; ", iOldSecPRC); fflush(stdout); }


		/* USB 1.0 levels. Logical '1'.  LOGIC_1_CURR */
		if ( iChkUsb10Lg1(pTimepoint->qfltXval) )

			/* Pull terminal USB#0/USB#1 <dIN> line up. */
			Term_Up( PD0 );

		else
			/* USB 1.0 levels. Logical '0'. LOGIC_0_CURR */
			if ( iChkUsb10Lg0(pTimepoint->qfltXval) )

				/* Pull terminal USB#0/USB#1 <dIN> line down. */
				Term_Down( PD0 );
#if 0
			else
				/* TODO: REWORK! A lot of _logical_ zeroes (or sort of that ) will come with negative power of 10 (i.e. 'sgn' is '-'). */
				PortD_Down( PD0 );

			/* Attention: overvoltage, U = 3.6++ Volts will be processed as logical zero, too. */
#endif /* (0) */

#if defined(DIN_FEEDBACK)
	;
#endif /* (DIN_FEEDBACK) */

}
