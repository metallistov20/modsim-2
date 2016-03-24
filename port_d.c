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


/* Not connected to AD5300. Leg (-1) of AD5300. (White) */
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

#define ConverterActivate 	PortD_Down(SYNC_PIN)
#define ConverterDeactivate	PortD_Up(SYNC_PIN)


#define AD5300_DATA_LEN		16
#define AD5300_DONTCARE_LEN	4

/* Prepare Port 'D' */
void PortD_Prepare()
{
#if defined(UCSIMM)

#if !defined(DIN_FEEDBACK)
	PDSEL = PD0 | PD1 | PD2 | PD3;

	PDDIR = PD0 | PD1 | PD2 | PD3;

	printf ("Bits <%08bb> <%08bb> of Port D initialized as OUT\n", (unsigned char)PD0, (unsigned char)PD1);
#else

    	SPIMCONT = 0x402f;/* TODO: put OR'ed macros of <asm/MC68EZ328.h> of constant here */
    	SPIMCONT |= SPIMCONT_ENABLE;

	printf ("Port D : bit <%08bb> OUT, bit <%08bb> IN \n", (unsigned char)PD0, (unsigned char)PD1);


#endif /* (!defined(DIN_FEEDBACK)) */

#endif /* (UCSIMM) */
}

/* Toggle bits defined by bitmask 'uchBit' in Port D*/
void PortD_Toggle(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA ^= uchBit;
#endif /* (UCSIMM) */
}

/* Switch off bits defined by bitmask 'uchBit' in Port D*/
void PortD_Down(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA &= ~uchBit;
#endif /* (UCSIMM) */
}

/* Switch on bits defined by bitmask 'uchBit' in Port D*/
void PortD_Up(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA |= uchBit;
#endif /* (UCSIMM) */
}

/* Port D probe routine, two bits are enough for current task */
void PortD_Probe( )
{
#if defined(UCSIMM)
	PortD_Prepare( );

	PortD_Toggle(  PD0 );

	while (1)
	{
		PortD_Toggle(  PD1 | PD0 );

		/* Dubious */
		usleep (10);
	}
#endif /* (UCSIMM) */
}

#if defined(DIN_FEEDBACK)

/* Return contents of Port D (as integer) as its data ready */
unsigned char PortD_Read(unsigned char uchBit)
{
	while (!(SPIMCONT & SPIMCONT_IRQ))

		usleep(1);

	return (PDDATA & uchBit) ;

}

/* Check if bit(s) defined by 'uchBit' of Port E is zero */
int PortD_CheckL0( unsigned char uchBit )
{
	return (0 == PortD_Read (~uchBit) );
}

/* Check if bit(s) defined by 'uchBit' of Port E is one */
int PortD_CheckL1( unsigned char uchBit )
{
	return (0 < PortD_Read (uchBit) );
}

#endif /* (defined(DIN_FEEDBACK)) */ 


void ConverterWrite(unsigned char data)
{
unsigned short tmp;

unsigned char iCnt;

	tmp = data << AD5300_DONTCARE_LEN;

	ConverterActivate;

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		SCLK_HI;

		(tmp & (unsigned short)( 1U<< (15 - iCnt) ) ) ? (MOSI_HI) : (MOSI_LO);

		SCLK_LO;
	}

	ConverterDeactivate;
} /* void ConverterWrite(unsigned short data) */

void ConverterInit(void)
{
	/* Prepare Port D */
	PDSEL = PD1 | PD2 | PD3;
	PDDIR = PD1 | PD2 | PD3;

	/* Prepare controler AD53xx */
	ConverterDeactivate;
	SCLK_LO;// TODO: check if necessary 
	MOSI_LO;// TODO: check if necessary 

} /* void ConverterInit(void) */
