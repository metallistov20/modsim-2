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

#define PD0orPD1 (PD0 | PD1)


#if !defined(USB20)

/* RUNET: TTL levels "logical 0" 0.4V and "logical 1" 2.4V, more precise: less than 0.4 and more
than 2.4 correspondingly. Current on DIn, DOut shuld not exceet 3.6 V. */

/* WIKI: Low- and full-speed modes (USB 1.x) use a single data pair, labeled D+ and D−, in half-duplex.
Transmitted signal levels are 0.0–0.3 V for logical low, and 2.8–3.6 V for logical high level. The signal
lines are not terminated. */
#define LOGIC_0_CURR	0.4
#define LOGIC_1_CURR	2.4
#define OVERDOSE_CURR 	3.6
/* This is pretty much OK for USB 1.0, but the target buses are all USB 2.0 */

#else

/* WIKI: high-speed mode (USB 2.0) uses the same wire pair, but with different electrical conventions. Lower
signal voltages of −10 – 10 mV for low and 360–440 mV for logical high level, and termination of 45 Ω to 
ground, or 90 Ω differential to match the data cable impedance. */

#define LOGIC_0_CURR	(-0.01) /* Lower signal voltages of −10 mV .. +10 mV for low */
#define LOGIC_0up_CURR	0.01 
#define LOGIC_1_CURR	0.36 /* 360 mV .. 440 mV for logical high level */
#define LOGIC_1up_CURR	0.44
/* Crrently not used. Undefined. */
#define OVERDOSE_CURR 	0

#endif /* (#if !defined(USB20)) */

/* Switch on bits defined by bitmask 'uchBit' in Port D */
void PortD_Toggle(unsigned char uchBit);

/* Switch off bits defined by bitmask 'uchBit' in Port D */
void PortD_Down(unsigned char uchBit);



#endif /* _PORT_D_H_ */
