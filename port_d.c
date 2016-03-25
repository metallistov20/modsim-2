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


/* Not connected to AD5300. Leg 1 of CPE#0. (White) */
#define NIX		PD0

/* ~Synchronization. Leg 5 of AD5300. (Orange) */
#define SYNC_PIN	PD1

/* Clocking. Leg 6 of AD5300. (Yellow) */
#define SCLK_PIN	PD2

/* Data output. Leg 7 of AD5300. (Green) */
#define MOSI_PIN	PD3

/* Port to write Data to*/
#define SPI_PORT	PDDATA


#define SCLK_LO		PortD_Down(SCLK_PIN)
#define SCLK_HI		PortD_Up(SCLK_PIN)

#define MOSI_LO		PortD_Down(MOSI_PIN)
#define MOSI_HI		PortD_Up(MOSI_PIN)

#define AD5300_Activate 	PortD_Down(SYNC_PIN)
#define AD5300_Deactivate PortD_Up(SYNC_PIN)

#define AD5300_DATA_LEN	16
#define AD5300_DONTCARE_LEN 4

/* Prepare Port 'D' */
void PortD_Prepare()
{
	PDSEL = PD0 | PD1 | PD2 | PD3;

	PDDIR = PD0 | PD1 | PD2 | PD3;

	printf ("Bits <%08bb> <%08bb> of Port D initialized as OUT\n", (unsigned char)PD0, (unsigned char)PD1);
}

/* Toggle bits defined by bitmask 'uchBit' in Port D*/
void PortD_Toggle(unsigned char uchBit)
{
	PDDATA ^= uchBit;
}

/* Switch off bits defined by bitmask 'uchBit' in Port D*/
void PortD_Down(unsigned char uchBit)
{
	PDDATA &= ~uchBit;
}

/* Switch on bits defined by bitmask 'uchBit' in Port D*/
void PortD_Up(unsigned char uchBit)
{
	PDDATA |= uchBit;
}

/* Switch off <Din> wire on either CPE#0, or CPE#1 */
void Term_Down()
{
	switch (iOperation)
	{
		case DO_GATE0_OP:
			PortD_Down(PD0);
			break;

		case DO_GATE1_OP:
			AD5300_Write( USB20_LOGIC_0_UP_CURR_FRACT/1000 )  ;
			break;

		default:
			printf("__s: bad kind of operatoin (while DOWN), restart the program.\n");
			exit (-1);
	}
} /* void Term_Down() */

/* Switch on <Din> wite on either CPE#0, or CPE#1 */
void Term_Up()
{
	switch (iOperation)
	{
		case DO_GATE0_OP:
			PortD_Up(PD0);
			break;

		case DO_GATE1_OP:
			AD5300_Write( USB20_LOGIC_1_UP_CURR_FRACT/1000 )  ;
			break;

		default:
			printf("__s: bad kind of operatoin (while UP), restart the program\n");
			exit (-1);
	}
} /* void Term_Up() */

void AD5300_Write(unsigned char data)
{
unsigned short tmp;

unsigned char iCnt;

	tmp = data << AD5300_DONTCARE_LEN;

	AD5300_Activate;

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		SCLK_HI;

		(tmp & (unsigned short)( 1U << (15 - iCnt) ) ) ? (MOSI_HI) : (MOSI_LO);

		SCLK_LO;
	}

	AD5300_Deactivate;
} /* void AD5300_rWrite(unsigned short data) */

void AD5300_Init(void)
{
	/* Prepare Port D */
	PDSEL = PD1 | PD2 | PD3;
	PDDIR = PD1 | PD2 | PD3;

	/* Prepare controler AD53xx */
	AD5300_Deactivate;
	SCLK_LO;// TODO: check if necessary 
	MOSI_LO;// TODO: check if necessary 

} /* void AD5300_Init(void) */


/* Initialize Port 'D' and, once needed, converter arrached to it */
void PeriphInit(void)
{
	switch (iOperation)
	{
		case DO_GATE0_OP:
			/* Set digital PIOs 1-4 as outputs */
			PortD_Prepare( );
			break;

		case DO_GATE1_OP:
			/* Besides all, it prepares PIOs, so theres no need to do <PortD_Prepare()> */
			AD5300_Init();
			break;

		default:
			printf("__s: bad kind of operatoin (while PERIPH. INIT), restart the program\n");
			exit (-1);
	}
} /* void PeriphInit(void) */

