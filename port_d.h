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

#ifndef _PORT_D_H_
#define _PORT_D_H_

/*
BIT		Pin
~~~~~~~~~~~~~~~~~~
LD0		30
LD1		29
LD2		28
LD3		27
*/
#define PD0 0x80
#define PD1 0x40
#define PD2 0x20
#define PD3 0x10

#define PD4 0x08
#define PD5 0x04
#define PD6 0x02
#define PD7 0x01


/* RUNET: TTL levels "logical 0" 0.4V and "logical 1" 2.4V, more precise: less than 0.4 and more
than 2.4 correspondingly. Current on DIn, DOut shuld not exceet 3.6 V. */

/* WIKI: Low- and full-speed modes (USB 1.x) use a single data pair, labeled D+ and D−, in half-duplex.
Transmitted signal levels are 0.0–0.3 V for logical low, and 2.8–3.6 V for logical high level. The signal
lines are not terminated. */

// TODO: give a preference either to WIKI wither to RUNET. (see above.)

/* Anything under this voltage is regarded to be <logical 0> for USB 1.1 */
#define USB11_LOGIC_0_CURR		0.4	// TODO: not used, clean out
/* Integer part of <LOGIC_0_CURR> */
#define USB11_LOGIC_0_CURR_INTGR	0
/* Fraction  of <LOGIC_0_CURR> */
#define USB11_LOGIC_0_CURR_FRACT	40000

/* Anything above this voltage is regarded to be <logical 1> for USB 1.1 */
#define USB11_LOGIC_1_CURR		2.4	// TODO: not used, clean out
/* Integer part of <LOGIC_1_CURR> */
#define USB11_LOGIC_1_CURR_INTGR	2
/* Fraction  of <LOGIC_1_CURR> */
#define USB11_LOGIC_1_CURR_FRACT	40000


/* WIKI: high-speed mode (USB 2.0) uses the same wire pair, but with different electrical conventions. Lower
signal voltages of −10 – 10 mV for low and 360–440 mV for logical high level, and termination of 45 Ω to 
ground, or 90 Ω differential to match the data cable impedance. */


/* Anything from range −10 mV .. +10 mV is regarded to be <logical 0> for USB 2.0 */
#define USB20_LOGIC_0_LO_CURR	(-0.01)		// TODO: not used, clean out
#define USB20_LOGIC_0_UP_CURR	0.01 		// TODO: not used, clean out

/* Anything from range 360 mV .. 440 mV is regarded to be <logical 1> for USB 2.0 */
#define USB20_LOGIC_1_LO_CURR	0.36		// TODO: not used, clean out
#define USB20_LOGIC_1_UP_CURR	0.44		// TODO: not used, clean out

#define USB20_LOGIC_0_LO_CURR_INTGR	(-0)
#define USB20_LOGIC_0_LO_CURR_FRACT	1000
#define USB20_LOGIC_0_UP_CURR_INTGR	0
#define USB20_LOGIC_0_UP_CURR_FRACT	1000

#define USB20_LOGIC_1_LO_CURR_INTGR	0
#define USB20_LOGIC_1_LO_CURR_FRACT	36000
#define USB20_LOGIC_1_UP_CURR_INTGR	0
#define USB20_LOGIC_1_UP_CURR_FRACT	44000

#define UNPROC				"Not processed"

/* Prepared Port's D IOs defined by bitmask 'uchBit' as outputs. Exposed to main() [HW_PORTD_TEST] */
void PortD_Prepare(unsigned char uchBitMask);

/* Switch on bits defined by bitmask 'uchBit' in Port D. Exposed to main() [HW_PORTD_TEST] */
void PortD_Toggle(unsigned char uchBit);

/* Terminal line DOWN - switch off <dIN> wire on CPE#0, bzw CPE#1. Exposed to <ProcessPoint()> */
void Term_Up();

/* Terminal line DOWN - switch off <dIN> wire on CPE#0, bzw CPE#1. Exposed to <ProcessPoint()> */
void Term_Down();

/* Write 'data'::{0..255} to converter. Parameter '0' for <0> Volts, <255> - VDD Volts. VDD is 3.3(5.0). Exposed to main() [HW_AD53_TEST] */
void AD5300_Write(unsigned char data);

/* Initialize converter. Exposed to main() [HW_AD53_TEST] */
void AD5300_Init(void);

/* Initialize Port 'D' and, once needed, converter arrached to it. Exposed to main() */
void PeriphInit(void);

#endif /* _PORT_D_H_ */
