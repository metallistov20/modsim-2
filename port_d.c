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

#include <stdio.h>

#if defined(UCSIMM)

/* Platform definitions */
#include <asm/MC68EZ328.h>

#include <sys/bitypes.h>

#endif /* (UCSIMM) */

/* Own interface, definitions */
#include "port_d.h"

/* iOperatoin */
#include "modsim.h"


/* Not connected to AD5300. Leg 1 of CPE#0. (D-, White) */
#define NIX		PD0
/* Not connected to AD5300. Leg 2 of CPE#0. (D+, Green) */
#define NIX_IN		PD1

#if 0

/* ~Synchronization. Leg 5 of AD5300. (Orange) */
#define SYNC_PIN	 PD2

/* Clocking. Leg 6 of AD5300. (Yellow) */
#define SCLK_PIN	 PD3

/* Data output. Leg 7 of AD5300. (Green) */
#define MOSI_PIN	 PD4

#else

#if 0
// TODO: REMOVE!
/* ~Synchronization. Leg 5 of AD5300. (Orange) */
#define SYNC_PIN	 PD2

/* Clocking. Leg 6 of AD5300. (Yellow) */
#define SCLK_PIN	 PD3

/* Data output. Leg 7 of AD5300. (Green) */
#define MOSI_PIN	 PD4
#endif 

/* ~Synchronization. Leg 5 of 'white' AD5300. (Orange) */
#define SYNC_PIN_W	 PD2
/* Clocking. Leg 6 of 'white' AD5300. (Yellow) */
#define SCLK_PIN_W	 PD3
/* Data output. Leg 7 of 'white' AD5300. (Green) */
#define MOSI_PIN_W	 PD4

/* ~Synchronization. Leg 5 of 'green' AD5300. (Orange) */
#define SYNC_PIN_G	 PD5
/* Clocking. Leg 6 of 'green' AD5300. (Yellow) */
#define SCLK_PIN_G	 PD6
/* Data output. Leg 7 of 'freen' AD5300. (Green) */
#define MOSI_PIN_G	 PD7

#endif /* (0) */

/* Port to write Data to*/
#define SPI_PORT	PDDATA

/*
#define SCLK_LO		PortD_Down(SCLK_PIN)
#define SCLK_HI		PortD_Up(SCLK_PIN)
#define MOSI_LO		PortD_Down(MOSI_PIN)
#define MOSI_HI		PortD_Up(MOSI_PIN)
#define AD5300_ACT 	PortD_Down(SYNC_PIN)
#define AD5300_DEACT 	PortD_Up(SYNC_PIN)
*/

#define SCLK_LO_W		PortD_Down(SCLK_PIN_W)
#define SCLK_HI_W		PortD_Up(SCLK_PIN_W)
#define MOSI_LO_W		PortD_Down(MOSI_PIN_W)
#define MOSI_HI_W		PortD_Up(MOSI_PIN_W)
#define AD5300_ACT_W 		PortD_Down(SYNC_PIN_W)
#define AD5300_DEACT_W 		PortD_Up(SYNC_PIN_W)

#define SCLK_LO_G		PortD_Down(SCLK_PIN_G)
#define SCLK_HI_G		PortD_Up(SCLK_PIN_G)
#define MOSI_LO_G		PortD_Down(MOSI_PIN_G)
#define MOSI_HI_G		PortD_Up(MOSI_PIN_G)
#define AD5300_ACT_G 		PortD_Down(SYNC_PIN_G)
#define AD5300_DEACT_G 		PortD_Up(SYNC_PIN_G)

#define AD5300_DATA_LEN	16
#define AD5300_DONTCARE_LEN 4

/* Prepare Port's D IOs defined by bitmask 'uchBit' as outputs */
void PortD_Prepare(unsigned char uchBitMask)
{
	PDSEL = uchBitMask;

	PDDIR = uchBitMask;

	printf ("Bits <%08bb> of Port D initialized as OUT\n", uchBitMask );
}

/* Toggle bits defined by bitmask 'uchBit' in Port D */
void PortD_Toggle(unsigned char uchBit)
{
	PDDATA ^= uchBit;
}

/* Switch off bits defined by bitmask 'uchBit' in Port D */
void PortD_Down(unsigned char uchBit)
{
	PDDATA &= ~uchBit;
}

/* Switch on bits defined by bitmask 'uchBit' in Port D */
void PortD_Up(unsigned char uchBit)
{
	PDDATA |= uchBit;
}

/* Terminal line DOWN - switch off <dIN> wire on CPE#0, bzw CPE#1. Exposed to <ProcessPoint()> */
void Term_Down()
{
	switch (iOperation)
	{
		case DO_GATE0_OP:
			PortD_Down(PD0);
			break;

		case DO_GATE1_OP:
#if 0
			AD5300_Write( USB20_LOGIC_0_UP_CURR_FRACT/1000 )  ;
#else
#endif /* (0) */
			break;

		default:
			printf("__s: bad kind of operatoin (while DOWN), restart the program.\n");
			exit (-1);
	}
} /* void Term_Down() */

/* Terminal line DOWN - switch off <dIN> wire on CPE#0, bzwCPE#1. Exposed to <ProcessPoint()> */
void Term_Up()
{
	switch (iOperation)
	{
		case DO_GATE0_OP:
			PortD_Up(PD0);
			break;

		case DO_GATE1_OP:
#if 0
			AD5300_Write( USB20_LOGIC_1_UP_CURR_FRACT/1000 )  ;
#else
#endif /* (0) */
			break;

		default:
			printf("__s: bad kind of operatoin (while UP), restart the program\n");
			exit (-1);
	}
} /* void Term_Up() */

/* Write 'data'::{0..255} to converter. Parameter '0' for <0> Volts, <255> - VDD Volts. VDD is 3.3 (bzw 5.0) */
#if 0
// TODO: REMOVE!
void AD5300_Write(unsigned char data /* Value '256' is relevant for converter, but not relevant for this FN. TODO: resolve */ ) 
{
unsigned short tmp;

unsigned char iCnt;

	tmp = data << AD5300_DONTCARE_LEN;

	AD5300_ACT;

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		SCLK_HI;

		(tmp & (unsigned short)( 1U << (15 - iCnt) ) ) ? (MOSI_HI) : (MOSI_LO);

		SCLK_LO;
	}

	AD5300_DEACT;
} /* void AD5300_Write(unsigned short data) */
#endif 

void AD5300_Write_W(unsigned char data) 
{
unsigned short tmp;

unsigned char iCnt;

	tmp = data << AD5300_DONTCARE_LEN;

	AD5300_ACT_W;

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		SCLK_HI_W;

		(tmp & (unsigned short)( 1U << (15 - iCnt) ) ) ? (MOSI_HI_W) : (MOSI_LO_W);

		SCLK_LO_W;
	}

	AD5300_DEACT_W;

} /* void AD5300_Write_W(unsigned short data) */

void AD5300_Write_G(unsigned char data) 
{
unsigned short tmp;

unsigned char iCnt;

	tmp = data << AD5300_DONTCARE_LEN;

	AD5300_ACT_G;

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		SCLK_HI_G;

		(tmp & (unsigned short)( 1U << (15 - iCnt) ) ) ? (MOSI_HI_G) : (MOSI_LO_G);

		SCLK_LO_G;
	}

	AD5300_DEACT_G;

} /* void AD5300_Write_W(unsigned short data) */

#if 0
// TODO: REMOVE!
/* Initialize converter */
void AD5300_Init(void)
{
	/* Prepare Port D */
	PDSEL = SYNC_PIN | SCLK_PIN | MOSI_PIN;
	PDDIR = SYNC_PIN | SCLK_PIN | MOSI_PIN;  

	/* Prepare controler AD53xx */
	AD5300_DEACT;

	SCLK_LO;// TODO: check if necessary 
	MOSI_LO;// TODO: check if necessary 

} /* void AD5300_Init(void) */
#endif

/* Initialize 'white' converter */
void AD5300_Init_W(void)
{
#if 0
	/* Prepare 'white'  bits on Port D */
	PDSEL = SYNC_PIN_W | SCLK_PIN_W | MOSI_PIN_W;
	PDDIR = SYNC_PIN_W | SCLK_PIN_W | MOSI_PIN_W;  
#else
	PDSEL = SYNC_PIN_G | SCLK_PIN_G | MOSI_PIN_G | SYNC_PIN_W | SCLK_PIN_W | MOSI_PIN_W;
	PDDIR = SYNC_PIN_G | SCLK_PIN_G | MOSI_PIN_G | SYNC_PIN_W | SCLK_PIN_W | MOSI_PIN_W;  
#endif
	/* Prepare 'white' controler AD53xx */
	AD5300_DEACT_W;

	SCLK_LO_W;
	MOSI_LO_W;

} /* void AD5300_Init_W(void) */

/* Initialize 'green' converter */
void AD5300_Init_G(void)
{
#if 0
	/* Prepare 'green'  bits on Port D */
	PDSEL = SYNC_PIN_G | SCLK_PIN_G | MOSI_PIN_G;
	PDDIR = SYNC_PIN_G | SCLK_PIN_G | MOSI_PIN_G;  
#else
	PDSEL = SYNC_PIN_G | SCLK_PIN_G | MOSI_PIN_G | SYNC_PIN_W | SCLK_PIN_W | MOSI_PIN_W;
	PDDIR = SYNC_PIN_G | SCLK_PIN_G | MOSI_PIN_G | SYNC_PIN_W | SCLK_PIN_W | MOSI_PIN_W;  
#endif

	/* Prepare 'white' controler AD53xx */
	AD5300_DEACT_G;

	SCLK_LO_G;
	MOSI_LO_G;

} /* void AD5300_Init_G(void) */


/* Initialize Port 'D' and, once needed, converter arrached to it */
void PeriphInit(void)
{
	switch (iOperation)
	{
		case DO_GATE0_OP:
			/* Set digital PIOs 1-4 as outputs */
//TODO: rem after test:			PortD_Prepare( PD0 | PD1 | PD2 | PD3 );
			PortD_Prepare( PD0 | PD1 /* | PD2 | PD3 */ );
			break;

		case DO_GATE1_OP:
			/* Besides all, it prepares PIOs, so theres no need to do <PortD_Prepare()> */
			AD5300_Init_W();
			AD5300_Init_G();
			break;

		default:
			printf("__s: bad kind of operatoin (while PERIPH. INIT), restart the program\n");
			exit (-1);
	}
} /* void PeriphInit(void) */

