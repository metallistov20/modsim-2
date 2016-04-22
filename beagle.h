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

#ifndef _BEAGLE_H_
#define _BEAGLE_H_

#define NUM_PORTS	6

#if defined(SH_FOPS)

	/* Definitions for shell file operatoins */

	/* CPE#1 */
	#define _30 "30"
	#define _31 "31"
	#define _48 "48"
	#define _5 "5"
	#define _3 "3"
	#define _49 "49"

	/* CPE#2 */
	#define _117 "117"
	#define _115 "115"
	#define _60 "60"
	#define _50 "50"
	#define _51 "51"
	#define _4 "4"

	/* CPE#3 */
	#define _2 "2"
	#define _15 "15"
	#define _14 "14"
	#define _112 "112"
	#define _66 "66"
	#define _69 "69"

	/* CPE#4 */
	#define _45 "45"
	#define _23 "23"
	#define _47 "47"
	#define _27 "27"
	#define _22 "22"
	#define _67 "67"

	/* CPE#5*/
	#define _68 "68"
	#define _44 "44"
	#define _26 "26"
	#define _46 "46"
	#define _65 "65"
	#define _61 "61"
#else
	/* Definitions for GLIBC API file operatoins */

	/* CPE#1 */
	#define _30 GPIO_VALUE_FILES[0]
	#define _31 GPIO_VALUE_FILES[1]
	#define _48 GPIO_VALUE_FILES[2]
	#define _5 GPIO_VALUE_FILES[3]
	#define _3 GPIO_VALUE_FILES[4]
	#define _49 GPIO_VALUE_FILES[5]

	/* CPE#2 */
	#define _117 GPIO_VALUE_FILES[6]
	#define _115 GPIO_VALUE_FILES[7]
	#define _60 GPIO_VALUE_FILES[8]
	#define _50 GPIO_VALUE_FILES[9]
	#define _51 GPIO_VALUE_FILES[10]
	#define _4 GPIO_VALUE_FILES[11]

	/* CPE#3 */
	#define _2 GPIO_VALUE_FILES[12]
	#define _15 GPIO_VALUE_FILES[13]
	#define _14 GPIO_VALUE_FILES[14]
	#define _112 GPIO_VALUE_FILES[15]
	#define _66 GPIO_VALUE_FILES[16]
	#define _69 GPIO_VALUE_FILES[17]

	/* CPE#4 */
	#define _45 GPIO_VALUE_FILES[18]
	#define _23 GPIO_VALUE_FILES[19]
	#define _47 GPIO_VALUE_FILES[20]
	#define _27 GPIO_VALUE_FILES[21]
	#define _22 GPIO_VALUE_FILES[22]
	#define _67 GPIO_VALUE_FILES[23]

	/* CPE#5 */
	#define _68 GPIO_VALUE_FILES[24]
	#define _44 GPIO_VALUE_FILES[25]
	#define _26 GPIO_VALUE_FILES[26]
	#define _46 GPIO_VALUE_FILES[27]
	#define _65 GPIO_VALUE_FILES[28]
	#define _61 GPIO_VALUE_FILES[29]

#endif /* (SH_FOPS) */


/* ~Synchronization.  */
#define SYNC_1_W	 	_30
/* Clocking.  */
#define SCLK_1_W	 	_31
/* Data output. */
#define MOSI_1_W	 	_66
/* ~Synchronization. */
#define SYNC_1_G	 	_5
/* Clocking. */
#define SCLK_1_G	 	_3
/* Data output. */
#define MOSI_1_G	 	_67

/* Toggle down SCKL on 'white' converter */
#define _1_SCLK_LO_W		OffGPIO(SCLK_1_W)
/* Toggle up SCKL on 'white' converter */
#define _1_SCLK_HI_W		OnGPIO(SCLK_1_W)
/* Toggle down MOSI on 'white' converter */
#define _1_MOSI_LO_W		OffGPIO(MOSI_1_W)
/* Toggle up MOSI on 'white' converter */
#define _1_MOSI_HI_W		OnGPIO(MOSI_1_W)
/* Activate 'white' converter */
#define _1_AD5300_ACT_W 	OffGPIO(SYNC_1_W)
/* Deactivate 'white' converter */
#define _1_AD5300_DEACT_W 	OnGPIO(SYNC_1_W)

/* Toggle down SCKL on 'green' converter */
#define _1_SCLK_LO_G		PortD_Down(SCLK_1_G)
/* Toggle up SCKL on 'green' converter */
#define _1_SCLK_HI_G		PortD_Up(SCLK_1_G)
/* Toggle down MOSI on 'green' converter */
#define _1_MOSI_LO_G		PortD_Down(MOSI_1_G)
/* Toggle up MOSI on 'green' converter */
#define _1_MOSI_HI_G		PortD_Up(MOSI_1_G)
/* Activate 'green' converter */
#define _1_AD5300_ACT_G 	PortD_Down(SYNC_1_G)
/* Deactivate 'green' converter */
#define _1_AD5300_DEACT_G 	PortD_Up(SYNC_1_G)


/* ~Synchronization.  */
#define SYNC_2_W	 	_117
/* Clocking.  */
#define SCLK_2_W	 	_115
/* Data output. */
#define MOSI_2_W	 	_60
/* ~Synchronization. */
#define SYNC_2_G	 	_50
/* Clocking. */
#define SCLK_2_G	 	_51
/* Data output. */
#define MOSI_2_G	 	_4

/* Toggle down SCKL on 'white' converter */
#define _2_SCLK_LO_W		OffGPIO(SCLK_2_W)
/* Toggle up SCKL on 'white' converter */
#define _2_SCLK_HI_W		OnGPIO(SCLK_2_W)
/* Toggle down MOSI on 'white' converter */
#define _2_MOSI_LO_W		OffGPIO(MOSI_2_W)
/* Toggle up MOSI on 'white' converter */
#define _2_MOSI_HI_W		OnGPIO(MOSI_2_W)
/* Activate 'white' converter */
#define _2_AD5300_ACT_W 	OffGPIO(SYNC_2_W)
/* Deactivate 'white' converter */
#define _2_AD5300_DEACT_W 	OnGPIO(SYNC_2_W)

/* Toggle down SCKL on 'green' converter */
#define _2_SCLK_LO_G		PortD_Down(SCLK_2_G)
/* Toggle up SCKL on 'green' converter */
#define _2_SCLK_HI_G		PortD_Up(SCLK_2_G)
/* Toggle down MOSI on 'green' converter */
#define _2_MOSI_LO_G		PortD_Down(MOSI_2_G)
/* Toggle up MOSI on 'green' converter */
#define _2_MOSI_HI_G		PortD_Up(MOSI_2_G)
/* Activate 'green' converter */
#define _2_AD5300_ACT_G 	PortD_Down(SYNC_2_G)
/* Deactivate 'green' converter */
#define _2_AD5300_DEACT_G	PortD_Up(SYNC_2_G)

/* ~Synchronization.  */
#define SYNC_3_W	 	_2
/* Clocking.  */
#define SCLK_3_W	 	_15
/* Data output. */
#define MOSI_3_W	 	_14
/* ~Synchronization. */
#define SYNC_3_G	 	_112
/* Clocking. */
#define SCLK_3_G	 	_66
/* Data output. */
#define MOSI_3_G	 	_69

/* Toggle down SCKL on 'white' converter */
#define _3_SCLK_LO_W		OffGPIO(SCLK_3_W)
/* Toggle up SCKL on 'white' converter */
#define _3_SCLK_HI_W		OnGPIO(SCLK_3_W)
/* Toggle down MOSI on 'white' converter */
#define _3_MOSI_LO_W		OffGPIO(MOSI_3_W)
/* Toggle up MOSI on 'white' converter */
#define _3_MOSI_HI_W		OnGPIO(MOSI_3_W)
/* Activate 'white' converter */
#define _3_AD5300_ACT_W 	OffGPIO(SYNC_3_W)
/* Deactivate 'white' converter */
#define _3_AD5300_DEACT_W 	OnGPIO(SYNC_3_W)

/* Toggle down SCKL on 'green' converter */
#define _3_SCLK_LO_G		PortD_Down(SCLK_3_G)
/* Toggle up SCKL on 'green' converter */
#define _3_SCLK_HI_G		PortD_Up(SCLK_3_G)
/* Toggle down MOSI on 'green' converter */
#define _3_MOSI_LO_G		PortD_Down(MOSI_3_G)
/* Toggle up MOSI on 'green' converter */
#define _3_MOSI_HI_G		PortD_Up(MOSI_3_G)
/* Activate 'green' converter */
#define _3_AD5300_ACT_G 	PortD_Down(SYNC_3_G)
/* Deactivate 'green' converter */
#define _3_AD5300_DEACT_G 	PortD_Up(SYNC_3_G)

/* ~Synchronization.  */
#define SYNC_4_W	 	_45
/* Clocking.  */
#define SCLK_4_W	 	_23
/* Data output. */
#define MOSI_4_W	 	_47
/* ~Synchronization. */
#define SYNC_4_G	 	_27
/* Clocking. */
#define SCLK_4_G	 	_22
/* Data output. */
#define MOSI_4_G	 	_67

/* Toggle down SCKL on 'white' converter */
#define _4_SCLK_LO_W		OffGPIO(SCLK_4_W)
/* Toggle up SCKL on 'white' converter */
#define _4_SCLK_HI_W		OnGPIO(SCLK_4_W)
/* Toggle down MOSI on 'white' converter */
#define _4_MOSI_LO_W		OffGPIO(MOSI_4_W)
/* Toggle up MOSI on 'white' converter */
#define _4_MOSI_HI_W		OnGPIO(MOSI_4_W)
/* Activate 'white' converter */
#define _4_AD5300_ACT_W 	OffGPIO(SYNC_4_W)
/* Deactivate 'white' converter */
#define _4_AD5300_DEACT_W 	OnGPIO(SYNC_4_W)

/* Toggle down SCKL on 'green' converter */
#define _4_SCLK_LO_G		PortD_Down(SCLK_4_G)
/* Toggle up SCKL on 'green' converter */
#define _4_SCLK_HI_G		PortD_Up(SCLK_4_G)
/* Toggle down MOSI on 'green' converter */
#define _4_MOSI_LO_G		PortD_Down(MOSI_4_G)
/* Toggle up MOSI on 'green' converter */
#define _4_MOSI_HI_G		PortD_Up(MOSI_4_G)
/* Activate 'green' converter */
#define _4_AD5300_ACT_G 	PortD_Down(SYNC_4_G)
/* Deactivate 'green' converter */
#define _4_AD5300_DEACT_G 	PortD_Up(SYNC_4_G)

/* ~Synchronization.  */
#define SYNC_5_W	 	_68
/* Clocking.  */
#define SCLK_5_W	 	_44
/* Data output. */
#define MOSI_5_W	 	_26
/* ~Synchronization. */
#define SYNC_5_G	 	_46
/* Clocking. */
#define SCLK_5_G	 	_65
/* Data output. */
#define MOSI_5_G	 	_61

/* Toggle down SCKL on 'white' converter */
#define _5_SCLK_LO_W		OffGPIO(SCLK_5_W)
/* Toggle up SCKL on 'white' converter */
#define _5_SCLK_HI_W		OnGPIO(SCLK_5_W)
/* Toggle down MOSI on 'white' converter */
#define _5_MOSI_LO_W		OffGPIO(MOSI_5_W)
/* Toggle up MOSI on 'white' converter */
#define _5_MOSI_HI_W		OnGPIO(MOSI_5_W)
/* Activate 'white' converter */
#define _5_AD5300_ACT_W 	OffGPIO(SYNC_5_W)
/* Deactivate 'white' converter */
#define _5_AD5300_DEACT_W 	OnGPIO(SYNC_5_W)

/* Toggle down SCKL on 'green' converter */
#define _5_SCLK_LO_G		PortD_Down(SCLK_5_G)
/* Toggle up SCKL on 'green' converter */
#define _5_SCLK_HI_G		PortD_Up(SCLK_5_G)
/* Toggle down MOSI on 'green' converter */
#define _5_MOSI_LO_G		PortD_Down(MOSI_5_G)
/* Toggle up MOSI on 'green' converter */
#define _5_MOSI_HI_G		PortD_Up(MOSI_5_G)
/* Activate 'green' converter */
#define _5_AD5300_ACT_G 	PortD_Down(SYNC_5_G)
/* Deactivate 'green' converter */
#define _5_AD5300_DEACT_G 	PortD_Up(SYNC_5_G)



#if defined(SH_FOPS)
typedef void (*GPIOHandler)(char *);
typedef char * GPIOTarget;
#else
typedef void (*GPIOHandler)(FILE *);
typedef FILE * GPIOTarget;
#endif

FILE * GPIO_VALUE_FILES[30];

/* Toggle down SCKL on 'white' converter */
GPIOTarget SCLK_i_W[NUM_PORTS] = {_30, _117, _2, _45, _68, NULL};
/* Toggle up SCKL on 'white' converter */
GPIOTarget MOSI_i_W[NUM_PORTS] = {_31, _115, _15, _23, _44, NULL};
/* Toggle down MOSI on 'white' converter */
GPIOTarget SYNC_i_W[NUM_PORTS] = {_66, _60, _14, _47, _26, NULL};
/* Toggle up MOSI on 'white' converter */
GPIOTarget SCLK_i_G[NUM_PORTS] = {_5, _50, _112, _27, _46, NULL};
/* Activate 'white' converter */
GPIOTarget MOSI_i_G[NUM_PORTS] = {_3, _51, _66/*!*/, _22, _65, NULL};
/* Deactivate 'white' converter */
GPIOTarget SYNC_i_G[NUM_PORTS] = {_67, _4, _69, _67/*!*/, _61, NULL};


#endif /* _BEAGLE_H_ */
