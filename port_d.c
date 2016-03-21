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

#define SS_PIN		PD1 /* PB4 */
#define SCK_PIN		PD2 /* PB5 */
#define MOSI_PIN	PD3 /* PB3 */

#define SPI_PORT	PDDATA /* PORTB */

#if defined (AVRCODE)
	#define SPI_DD		DDRB//TODO: remove
#endif /* defined (AVRCODE) */

#if defined (AVRCODE)
	#define AD5300_ACTIVATE		SPI_PORT &= (u_int8_t) ~_BV(SS_PIN)//TODO: remove
	#define AD5300_DEACTIVATE	SPI_PORT |= _BV(SS_PIN)
#else
	#define ConverterActivate 	PortD_Down(SS_PIN)
	#define ConverterDeactivate	PortD_Toggle(SS_PIN)
#endif /* defined (AVRCODE) */

#if defined (AVRCODE)
	#define SCK_LO		SPI_PORT &= (u_int8_t) ~_BV(SCK_PIN) //TODO: remove
	#define SCK_HI		SPI_PORT |= _BV(SCK_PIN)

	#define MOSI_LO		SPI_PORT &= (u_int8_t) ~_BV(MOSI_PIN)
	#define MOSI_HI		SPI_PORT |= _BV(MOSI_PIN)
#else
	#define SCK_LO		PortD_Down(SCK_PIN)
	#define SCK_HI		PortD_Toggle(SCK_PIN)

	#define MOSI_LO		PortD_Down(MOSI_PIN)
	#define MOSI_HI		PortD_Toggle(MOSI_PIN)
#endif /* defined (AVRCODE) */

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

#if 0
    	SPIMCONT = 0x0277;  // SPIMCONT = PUT_FIELD(SPIMCONT_DATA_RATE, 2) | SPIMCONT_PHA | PUTFIELD(SPIMCONT_BIT_COUNT, 15);
    	SPIMCONT = PUT_FIELD(SPIMCONT_DATA_RATE, 2) | SPIMCONT_PHA | PUTFIELD(SPIMCONT_BIT_COUNT, 15);
#else
    	SPIMCONT = 0x402f;
    	SPIMCONT |= SPIMCONT_ENABLE;

	printf ("Port D : bit <%08bb> OUT, bit <%08bb> IN \n", (unsigned char)PD0, (unsigned char)PD1);
#endif /* (0) */ 

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


void ConverterWrite(const u_int8_t data)
{
u_int16_t tmp = data << AD5300_DONTCARE_LEN;

u_int8_t i, _i;

	ConverterActivate;

	for (i = 0; i < AD5300_DATA_LEN; i++)
	{
		_i = 15 - i;

		SCK_HI;
#if defined (AVRCODE)
		(tmp & _BV(_i)) ? (MOSI_HI) : (MOSI_LO);
#else
		(tmp & (u_int16_t)(1U<<_i)) ? (MOSI_HI) : (MOSI_LO);
#endif /* defined (AVRCODE) */

		SCK_LO;
	}

	MOSI_LO;

	ConverterDeactivate;
}

void ConverterInit(void)
{
#if defined (AVRCODE)
	SPI_DD |= _BV(SS_PIN) | _BV(SCK_PIN) | _BV(MOSI_PIN);
#else
	PDSEL = PD1 | PD2 | PD3;
	PDDIR = PD1 | PD2 | PD3;
#endif /* defined (AVRCODE) */

	ConverterDeactivate;

	SCK_LO;

	MOSI_LO;

	ConverterWrite(0);
}
