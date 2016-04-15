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

/* stdout */
#include <stdio.h>

/* strlen() */
#include <string.h>

/* calloc() */
#include <stdlib.h>

/* struct timeval, gettimeofday() */
#include <sys/time.h>

/* Data structure type definition */
#include "datastruct.h"

/* Port D definitions, prototypes */
#include "port_d.h"

/* Errcode definitions */
#include "modsim.h"

#define max(x,y)	((x>y)?x:y)
#define CONV_BASE	256

/* Time measurement variable to define begin of time scale */
struct timeval starttimePROC;

/* First time, will be subtracted in HAL */
#if !defined(QUASIFLOAT) 
	float fFIRST;
#else
	int iFIRST;
#endif /* defined(QUASIFLOAT)  */

/* Attach 3 floats to tail of dynamic structure 'pTimepointType' */
int _EnrollPoint(const char * caller, pTimepointType * ppThisPointChain, 
#if !defined(QUASIFLOAT) 
	float * pfltTm, float * pfltX, float * pfltY, 
#else
	pQuasiFloatType pqfltTm, pQuasiFloatType pqfltX, pQuasiFloatType pqfltY, 
#endif /* !defined(QUASIFLOAT) */
	char * pcMrq)

{
pTimepointType pChild, pTempPointChain;

	if (NULL == *ppThisPointChain)
	{
		/* only one chain, for beginning */
		*ppThisPointChain = (pTimepointType) calloc ( 1, sizeof (TimepointType) );

		/* check if successful */
		if (NULL == *ppThisPointChain)
		{
#if !defined(QUASIFLOAT)
			printf("[%s] %s:%s : ERROR: can't allocate memory for first element. %f: [X(%f),Y(%f)]  \n",
			__FILE__, caller, __func__,
			*pfltTm, *pfltX, *pfltY);
#else
			printf("[%s] %s:%s : ERROR: can't allocate memory for first element. \n",
			__FILE__, caller, __func__);
#endif /* !defined(QUASIFLOAT) */

			return P_ERROR_MEM;
		}

#if !defined(QUASIFLOAT)
		(*ppThisPointChain)->fltXval = *pfltX;
		(*ppThisPointChain)->fltYval = *pfltY;
		(*ppThisPointChain)->fltAbsTime = *pfltTm;

		// TODO: introduce spline approximation here
		(*ppThisPointChain)->ushSplineXval = (*ppThisPointChain)->fltXval * 100;
		(*ppThisPointChain)->ushSplineYval = (*ppThisPointChain)->fltYval * 100;
#else
		memcpy(& ((*ppThisPointChain)->qfltXval), pqfltX, sizeof(QuasiFloatType) );
		memcpy(& ((*ppThisPointChain)->qfltYval), pqfltY, sizeof(QuasiFloatType) );
		memcpy(& ((*ppThisPointChain)->qfltAbsTime), pqfltTm, sizeof(QuasiFloatType) );

		// TODO: introduce spline approximation here
		(*ppThisPointChain)->ushSplineXval = ( (*ppThisPointChain)->qfltXval.integer * 100000 +
				(*ppThisPointChain)->qfltXval.fraction ) / 1000;

		(*ppThisPointChain)->ushSplineYval = ( (*ppThisPointChain)->qfltYval.integer * 100000 +
				(*ppThisPointChain)->qfltYval.fraction ) / 1000;
#endif /* !defined(QUASIFLOAT) */

		(*ppThisPointChain)->ushSplineXval = max(CONV_BASE, (*ppThisPointChain)->ushSplineXval);
		(*ppThisPointChain)->ushSplineYval = max(CONV_BASE, (*ppThisPointChain)->ushSplineYval);

		(*ppThisPointChain)->pcMarquee = calloc (1, strlen (pcMrq) +1 );
		strcpy( (*ppThisPointChain)->pcMarquee, pcMrq);
#if defined(QUASIFLOAT) 
		if ( 0 > (*ppThisPointChain)->qfltAbsTime.integer*1000000 )
			iFIRST = (*ppThisPointChain)->qfltAbsTime.integer*1000000 - (*ppThisPointChain)->qfltAbsTime.fraction;
		else
			iFIRST = (*ppThisPointChain)->qfltAbsTime.integer*1000000 + (*ppThisPointChain)->qfltAbsTime.fraction;
#else
		fFIRST = (*ppThisPointChain)->fltAbsTime;
#endif /* defined(QUASIFLOAT)  */

#if defined(DEBUG_DATA)
#if !defined(QUASIFLOAT)
		printf("[%s] %s:%s : FIRST <%f> <%f>[%d] <%f>[%d] <%s> \n", __FILE__, caller, __func__,
			(*ppThisPointChain)->fltAbsTime,
			(*ppThisPointChain)->fltXval, (*ppThisPointChain)->ushSplineXval,
			(*ppThisPointChain)->fltYval, (*ppThisPointChain)->ushSplineYval,
			(*ppThisPointChain)->pcMarquee
		);
#else
		printf("[%s] %s:%s : FIRST <%d.%d> <%d.%d>[%d] <%d.%d>[%d] <%s> \n", __FILE__, caller, __func__,

			(*ppThisPointChain)->qfltAbsTime.integer,(*ppThisPointChain)->qfltAbsTime.fraction,
			(*ppThisPointChain)->qfltXval.integer,(*ppThisPointChain)->qfltXval.fraction, (*ppThisPointChain)->ushSplineXval,
			(*ppThisPointChain)->qfltYval.integer,(*ppThisPointChain)->qfltYval.fraction, (*ppThisPointChain)->ushSplineYval,
			(*ppThisPointChain)->pcMarquee
		);
#endif /* !defined(QUASIFLOAT) */
#endif /* (DEBUG_DATA) */

	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *ppThisPointChain;

		pTempPointChain = (pTimepointType) calloc (1, sizeof (TimepointType) );

		if (NULL == pTempPointChain)
		{
#if !defined(QUASIFLOAT)
			printf("[%s] %s:%s : ERROR: can't allocate memory for next element. %f: [X(%f),Y(%f)]  \n", 
			__FILE__, caller, __func__,
			*pfltTm, *pfltX, *pfltY);
#else
			printf("[%s] %s:%s : ERROR: can't allocate memory for next element.\n", 
			__FILE__, caller, __func__);
#endif /* !defined(QUASIFLOAT) */

			return P_ERROR_MEM;
		}

#if !defined(QUASIFLOAT)
		pTempPointChain->fltXval = *pfltX;
		pTempPointChain->fltYval = *pfltY;
		pTempPointChain->fltAbsTime = *pfltTm;

		// TODO: introduce spline approximation here
		pTempPointChain->ushSplineXval = pTempPointChain->fltXval * 100;
		pTempPointChain->ushSplineYval = pTempPointChain->fltYval * 100;
#else
		memcpy(& ( pTempPointChain->qfltXval), 	pqfltX, sizeof(QuasiFloatType) );
		memcpy(& ( pTempPointChain->qfltYval), 	pqfltY, sizeof(QuasiFloatType) );
		memcpy(& ( pTempPointChain->qfltAbsTime), pqfltTm, sizeof(QuasiFloatType) );

		// TODO: introduce spline approximation here
		pTempPointChain->ushSplineXval = ( pTempPointChain->qfltXval.integer * 100000 +
				pTempPointChain->qfltXval.fraction ) / 1000;
		
		pTempPointChain->ushSplineYval = ( pTempPointChain->qfltYval.integer * 100000 +
				pTempPointChain->qfltYval.fraction ) / 1000;
#endif /* !defined(QUASIFLOAT) */

		// TODO: remove parasitic values (mostly they're same by modul, and different by sign)
		pTempPointChain->ushSplineXval = max(CONV_BASE, pTempPointChain->ushSplineXval);
		pTempPointChain->ushSplineYval = max(CONV_BASE, pTempPointChain->ushSplineYval);

		pTempPointChain->pcMarquee = calloc (1, strlen (pcMrq) +1 );
		strcpy( pTempPointChain->pcMarquee, pcMrq);


#if defined(DEBUG_DATA)
#if !defined(QUASIFLOAT)
		printf("[%s] %s:%s : NEXT <%f> <%f>[%d] <%f>[%d] <%s> \n", __FILE__, caller, __func__,
			pTempPointChain->fltAbsTime,
			pTempPointChain->fltXval, pTempPointChain->ushSplineXval,
			pTempPointChain->fltYval, pTempPointChain->ushSplineYval,
			pTempPointChain->pcMarquee
		);
#else
		printf("[%s] %s:%s : NEXT <%d.%d> <%d.%06d>[%d] <%d.%06d>[%d]   <%s> \n", __FILE__, caller, __func__,

			pTempPointChain->qfltAbsTime.integer,pTempPointChain->qfltAbsTime.fraction,
			pTempPointChain->qfltXval.integer,pTempPointChain->qfltXval.fraction, pTempPointChain->ushSplineXval,
			pTempPointChain->qfltYval.integer,pTempPointChain->qfltYval.fraction, pTempPointChain->ushSplineYval,		
			pTempPointChain->pcMarquee
		);
#endif /* !defined(QUASIFLOAT) */
#endif /* (DEBUG_DATA) */

		/* Skip everything, except last entry */
		while ( (NULL != pChild) && (NULL != pChild->pNext ) )
		{
			/* . . */
			pChild = pChild->pNext;
		}

		/* before while we've already ensured that next chunk was created allright */

		/* attach a new chain entry to the end of existing chain */
		pChild->pNext = pTempPointChain;

	}

	return P_SUCCESS;

} /* int _EnrollPoint( ... ) */


/* Process data stored in dynamic structure pointed by 'pPointChainPar' */
int _ProcessPoints(const char * caller, pTimepointType pPointChainPar)
{
pTimepointType pPointChain = pPointChainPar;
float fltAbsTime;

double timeusePROC;

	/* Take initial time. Current time values will be taken in 'ProcRealAndRel()' */
	gettimeofday(&starttimePROC, 0);

	/* Process each entry of chain */
	while (NULL != pPointChain)
	{
#if DEBUG_DATA_
#if !defined(QUASIFLOAT)
		printf("[%s] %s:%s : <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			pPointChain->fltAbsTime,
			pPointChain->fltXval,
			pPointChain->fltYval,
			pPointChain->pcMarquee
		);
#else
		printf("[%s] %s:%s : <%d.%d> <%d.%d> <%d.%d> <%s> \n", __FILE__, caller, __func__,
			pPointChain->qfltAbsTime.integer,pPointChain->qfltAbsTime.fraction,
			pPointChain->qfltXval.integer,pPointChain->qfltXval.fraction,
			pPointChain->qfltYval.integer,pPointChain->qfltYval.fraction,
			pPointChain->pcMarquee
		);
#endif /* !defined(QUASIFLOAT) */
#endif /* (DEBUG_DATA) */

		/* Realtime and relative-time values */
		ProcessPoint(pPointChain);

		/* Go to next record of chain */
		pPointChain = pPointChain->pNext;
	}

	return P_SUCCESS;

} /* int _ProcessPoints(const char * caller, pTimepointType pPointChainPar) */

/* Free memory occupied by '*ppThisPointChain' */
void _DeletePoints(const char * caller, pTimepointType * ppThisPointChain)
{
pTimepointType pChild, pThisPointChain = *ppThisPointChain;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisPointChain)
	{
		/* if space to keep item's name is allocated */
		if (pThisPointChain->pcMarquee)
		
		    /* then release this space */
		    free(pThisPointChain->pcMarquee);

		/* preserve a pointer to next record */		    
		pChild = pThisPointChain->pNext;
		
		/* free space occupied by current record */
		free (pThisPointChain);
		
		/* Go to next record */
		pThisPointChain = pChild;
	}

	/* Dispose first element of chain */
	*ppThisPointChain = NULL;

} /* void _DeletePoints(const char * caller, pTimepointType * ppThisPointChain) */
