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
#define SPI_PORT	PDDATA /* PORTB */

#if defined (AVRCODE)
	#define SPI_DD		DDRB//TODO: remove
#endif /* defined (AVRCODE) */

#if defined (AVRCODE)
	#define AD5300_ACTIVATE		SPI_PORT &= (u_int8_t) ~_BV(SS_PIN)//TODO: remove
	#define AD5300_DEACTIVATE	SPI_PORT |= _BV(SS_PIN)
#else
	#define ConverterActivate 	PortD_Down(SYNC_PIN)
	#define ConverterDeactivate	PortD_Up(SYNC_PIN)
#endif /* defined (AVRCODE) */

#if defined (AVRCODE)
	#define SCLK_LO		SPI_PORT &= (u_int8_t) ~_BV(SCK_PIN) //TODO: remove
	#define SCLK_HI		SPI_PORT |= _BV(SCK_PIN)

	#define MOSI_LO		SPI_PORT &= (u_int8_t) ~_BV(MOSI_PIN)
	#define MOSI_HI		SPI_PORT |= _BV(MOSI_PIN)
#else
	#define SCLK_LO		PortD_Down(SCLK_PIN)
	#define SCLK_HI		PortD_Up(SCLK_PIN)

	#define MOSI_LO		PortD_Down(MOSI_PIN)
	#define MOSI_HI		PortD_Up(MOSI_PIN)
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

#if 0 //TODO: remove
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


#define _SWAP16(x) 	((unsigned short)	((x&0xFF00)>>8)	|	((x&0x00FF)<<8)			)
void ConverterWrite0AA0(unsigned char data)
{
ConverterActivate;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 1
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 2
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; //MOSI_HI; //MOSI_LO; //MOSI_HI; // 3
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; //MOSI_LO; //MOSI_HI; //MOSI_HI; // 4
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }



	SCLK_HI;
	 MOSI_HI; // 5
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 6
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	 MOSI_HI; // 7
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 8
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	 MOSI_HI; // 9
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 10
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	 MOSI_HI; // 11
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 12
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }




	SCLK_HI;
	MOSI_LO; // 13
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 14
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 15
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 16
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }


ConverterDeactivate;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
}


void ConverterWrite0000(unsigned char data)
{
ConverterActivate;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 1
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 2
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; //MOSI_HI; //MOSI_LO; //MOSI_HI; // 3
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; //MOSI_LO; //MOSI_HI; //MOSI_HI; // 4
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }



	SCLK_HI;
	MOSI_LO; // 5
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 6
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 7
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 8
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 9
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 10
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 11
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 12
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }




	SCLK_HI;
	MOSI_LO; // 13
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 14
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 15
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 16
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }


ConverterDeactivate;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
}

void ConverterWrite0FF0(unsigned char data)
{
ConverterActivate;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 1
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 2
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; //MOSI_HI; //MOSI_LO; //MOSI_HI; // 3
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; //MOSI_LO; //MOSI_HI; //MOSI_HI; // 4
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }



	SCLK_HI;
	MOSI_HI; // 5
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_HI; // 6
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_HI; // 7
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_HI; // 8
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_HI; // 9
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_HI; // 10
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_HI; // 11
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_HI; // 12
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }




	SCLK_HI;
	MOSI_LO; // 13
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 14
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 15
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 16
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }


ConverterDeactivate;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
}



void ConverterWrite0110(unsigned char data)
{
ConverterActivate;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 1
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 2
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; //MOSI_HI; //MOSI_LO; //MOSI_HI; // 3
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; //MOSI_LO; //MOSI_HI; //MOSI_HI; // 4
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }



	SCLK_HI;
	MOSI_LO; // 5
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 6
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 7
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	 MOSI_HI; // 8
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 9
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 10
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 11
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	 MOSI_HI; // 12
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }




	SCLK_HI;
	MOSI_LO; // 13
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 14
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 15
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }

	SCLK_HI;
	MOSI_LO; // 16
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
	SCLK_LO;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }


ConverterDeactivate;
{volatile int iHundred = 5; while (iHundred--) {iHundred += 0;}   }
}

void ConverterWrite00F0(unsigned char data)
{
ConverterActivate;
	SCLK_HI;
	MOSI_LO; // 1
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 2
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 3
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 4
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 5
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 6
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 7
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 8
	SCLK_LO;

	SCLK_HI;
	MOSI_HI; // 9
	SCLK_LO;

	SCLK_HI;
	MOSI_HI; // 10
	SCLK_LO;

	SCLK_HI;
	MOSI_HI; // 11
	SCLK_LO;

	SCLK_HI;
	MOSI_HI; // 12
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 13
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 14
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 15
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 16
	SCLK_LO;

	MOSI_LO;

ConverterDeactivate;
}


void ConverterWrite0F00(unsigned char data)
{
ConverterActivate;
	SCLK_HI;
	MOSI_LO; // 1
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 2
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 3
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 4
	SCLK_LO;

	SCLK_HI;
	MOSI_HI; // 5
	SCLK_LO;

	SCLK_HI;
	MOSI_HI; // 6
	SCLK_LO;

	SCLK_HI;
	MOSI_HI; // 7
	SCLK_LO;

	SCLK_HI;
	MOSI_HI; // 8
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 9
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 10
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 11
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 12
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 13
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 14
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 15
	SCLK_LO;

	SCLK_HI;
	MOSI_LO; // 16
	SCLK_LO;

	MOSI_LO;

ConverterDeactivate;
}



//void ConverterWrite(unsigned short data)
void ConverterWrite(unsigned char data)
{
//u_int16_t tmp = data << AD5300_DONTCARE_LEN;
//unsigned short tmp = (unsigned short) ( data << AD5300_DONTCARE_LEN ) ;
unsigned short tmp;

//u_int8_t i, _i;
unsigned char i, _i;

	tmp = _SWAP16(data);
	tmp <<= 4;

	ConverterActivate;

	for (i = 0; i < AD5300_DATA_LEN; i++)
	{
		_i = 15 - i;

		SCLK_HI;
#if defined (AVRCODE)
		(tmp & _BV(_i)) ? (MOSI_HI) : (MOSI_LO);
#else
		//(tmp & (u_int16_t)(1U<<_i)) ? (MOSI_HI) : (MOSI_LO);
		//(tmp & (unsigned short)( _SWAP16 (1U<<i) ) ) ? (MOSI_HI) : (MOSI_LO);
		//if (255 == data) MOSI_HI ; else MOSI_LO;
		MOSI_LO;
#endif /* defined (AVRCODE) */
		

		SCLK_LO;
	}

	MOSI_LO;

	ConverterDeactivate;
} /* void ConverterWrite(unsigned short data) */

void ConverterInit(void)
{
#if defined (AVRCODE)
	SPI_DD |= _BV(SS_PIN) | _BV(SCK_PIN) | _BV(MOSI_PIN);
#else
	PDSEL = PD1 | PD2 | PD3;

	PDDIR = PD1 | PD2 | PD3;
#endif /* defined (AVRCODE) */

	ConverterDeactivate;	

	SCLK_LO;
	MOSI_LO;

	ConverterWrite(0);

} /* void ConverterInit(void) */
