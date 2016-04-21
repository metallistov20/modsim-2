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


/* ~Synchronization.  */
#define SYNC_1_W	 	"66" //"30"
/* Clocking.  */
#define SCLK_1_W	 	"31"
/* Data output. */
#define MOSI_1_W	 	"48"
/* ~Synchronization. */
#define SYNC_1_G	 	"67" //"5"
/* Clocking. */
#define SCLK_1_G	 	"3"
/* Data output. */
#define MOSI_1_G	 	"49"

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
#define SYNC_2_W	 	"117"
/* Clocking.  */
#define SCLK_2_W	 	"115"
/* Data output. */
#define MOSI_2_W	 	"60"
/* ~Synchronization. */
#define SYNC_2_G	 	"50"
/* Clocking. */
#define SCLK_2_G	 	"51"
/* Data output. */
#define MOSI_2_G	 	"4"

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
#define SYNC_3_W	 	"2"
/* Clocking.  */
#define SCLK_3_W	 	"15"
/* Data output. */
#define MOSI_3_W	 	"14"
/* ~Synchronization. */
#define SYNC_3_G	 	"112"
/* Clocking. */
#define SCLK_3_G	 	"66"
/* Data output. */
#define MOSI_3_G	 	"69"

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
#define SYNC_4_W	 	"45"
/* Clocking.  */
#define SCLK_4_W	 	"23"
/* Data output. */
#define MOSI_4_W	 	"47"
/* ~Synchronization. */
#define SYNC_4_G	 	"27"
/* Clocking. */
#define SCLK_4_G	 	"22"
/* Data output. */
#define MOSI_4_G	 	"67"

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
#define SYNC_5_W	 	"68"
/* Clocking.  */
#define SCLK_5_W	 	"44"
/* Data output. */
#define MOSI_5_W	 	"26"
/* ~Synchronization. */
#define SYNC_5_G	 	"46"
/* Clocking. */
#define SCLK_5_G	 	"65"
/* Data output. */
#define MOSI_5_G	 	"61"

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



#endif /* _BEAGLE_H_ */
