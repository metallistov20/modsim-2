/* stdout, NULL */
#include <stdio.h>

/* sprintf() */
#include <string.h>

/* <_1_SCLK_HI_W> et al */
#include "beagle.h"

//static PortD_Down(){};// TODO: rem
//static PortD_Up(){};// TODO: rem

/* <AD5300_DATA_LEN>, <AD5300_DONTCARE_LEN> */
#include "port_d.h"

#define MEDIUM_SIZE 1024
char pcCmdBuffer[MEDIUM_SIZE];

// TODO: explore GPIOs "4"/"5", this pair appears to be non working.
char * GPIOs[] = {
	 /* P9, left side */
	 "30", "31", "48" , "5", "3", "49", "117", "115", 

	 /* P9, right side */
	 "60", "50", "51" , "4", "2", "15", "14",  "112",

	 /* P8, left side */ 
	 "66", "69", "45", "23", "47", "27", "22",

	 /* P8, right side */ 
	 "67", "68", "44", "26", "46", "65", "61" 
}; /* char * GPIOs[] */


FILE * GPIO_VALUE_FILES[30];

/* Make GPIO port <pcPortStr>: a) to appear in the system; b) to become output port; */
static void OpenGPIO(char * pcPortStr)
{
	if  (NULL != pcPortStr)
	{
		/* Command prepare */
		sprintf (pcCmdBuffer, "echo %s > /sys/class/gpio/export", pcPortStr);

		/* Run command in <sh> shell */
		system (pcCmdBuffer);

		/* Wait for driver create filesystem components </sys/class/gpio/gpio%s/> */
		usleep(1000);

		/* Command prepare */
		sprintf (pcCmdBuffer, "echo out > /sys/class/gpio/gpio%s/direction", pcPortStr);

		/* Run command in <sh> shell */
		system (pcCmdBuffer);

		/* Wait for driver stabilize tartget GPIO port */
		usleep(1000);
	}
	else
		printf("[%s] [%s] empty Port ID \n",__FILE__, __func__ );
}

/* Toggle GPIO port <pcPortStr> ON */
#if defined(SH_FOPS)
void OnGPIO(char * pcPortStr)
#else
void OnGPIO(FILE * fcPortFile)
#endif /* (0) */
{
#if defined(SH_FOPS)
	if  (NULL != pcPortStr)
#else
	if  (NULL != fcPortFile)
#endif /* (0) */
	{
#if defined(SH_FOPS)
		/* Command prepare */		
		sprintf (pcCmdBuffer, "echo 1 > /sys/class/gpio/gpio%s/value", pcPortStr);

		/* Run command in <sh> shell */
		system (pcCmdBuffer);
#else
		fwrite("1", 1,1, fcPortFile);
		fflush(fcPortFile);
#endif /* (0) */

		/* Time balast */
//.		usleep(1);
	}
	else
		printf("[%s] [%s] empty Port ID \n",__FILE__, __func__ );
}

/* Toggle GPIO port <pcPortStr> OFF */
#if defined(SH_FOPS)
void OffGPIO(char * pcPortStr)
#else
void OffGPIO(FILE * fcPortFile)
#endif /* (0) */
{
#if defined(SH_FOPS)
	if  (NULL != pcPortStr)
#else
	if  (NULL != fcPortFile)
#endif /* (0) */
	{
#if defined(SH_FOPS)
		/* Command prepare */
		sprintf (pcCmdBuffer, "echo 0 > /sys/class/gpio/gpio%s/value", pcPortStr);

		/* Run command in <sh> shell */
		system (pcCmdBuffer);
#else
		fwrite("0", 1,1, fcPortFile);
		fflush(fcPortFile);
#endif /* (0) */

		/* Time balast */
//.		usleep(2);
	}
	else
		printf("[%s] [%s] empty Port ID \n",__FILE__, __func__ );
}

// TODO: remove on nearest Monday
void _i_AD5300_Write_W(unsigned char data, int iIdx) 
{
unsigned short tmp;

unsigned char iCnt;

	tmp = data << AD5300_DONTCARE_LEN;

	_i_AD5300_ACT_W ( SYNC_i_W [iIdx] );

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		_i_SCLK_HI_W( SCLK_i_W[iIdx] );

		(tmp & (unsigned short)( 1U << (15 - iCnt) ) ) ? (_i_MOSI_HI_W( MOSI_i_W[iIdx] )) : (_i_MOSI_LO_W( MOSI_i_W[iIdx] ));

		_i_SCLK_LO_W( SCLK_i_W[iIdx] );
	}

	_i_AD5300_DEACT_W ( SYNC_i_W [iIdx] );

} /* void _i_AD5300_Write_W(unsigned short data, int iIdx) */

// TODO: remove on nearest Monday
void _i_AD5300_Write_G(unsigned char data, int iIdx) 
{
unsigned short tmp;

unsigned char iCnt;

	tmp = data << AD5300_DONTCARE_LEN;

	_i_AD5300_ACT_G ( SYNC_i_G [iIdx] );

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		_i_SCLK_HI_G( SCLK_i_G[iIdx] );

		(tmp & (unsigned short)( 1U << (15 - iCnt) ) ) ? (_i_MOSI_HI_G( MOSI_i_G[iIdx] )) : (_i_MOSI_LO_G( MOSI_i_G[iIdx] ));

		_i_SCLK_LO_G( SCLK_i_G[iIdx] );
	}

	_i_AD5300_DEACT_G ( SYNC_i_G [iIdx] );

} /* void _i_AD5300_Write_G(unsigned short data, int iIdx) */

// TODO: rem.
void _1_AD5300_Write_W(unsigned char data) 
{
unsigned short tmp;

unsigned char iCnt;

#if !defined(SH_FOPS)

	tmp = data << AD5300_DONTCARE_LEN;

	_1_AD5300_ACT_W;

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		_1_SCLK_HI_W;

		(tmp & (unsigned short)( 1U << (15 - iCnt) ) ) ? (_1_MOSI_HI_W) : (_1_MOSI_LO_W);

		_1_SCLK_LO_W;
	}

	_1_AD5300_DEACT_W;
#endif
} /* void _1_AD5300_Write_W(unsigned short data) */

// TODO: remove as nasty
void _1_AD5300_Write_G(unsigned char data) 
{
unsigned short tmp;

unsigned char iCnt;

#if !defined(SH_FOPS)
	tmp = data << AD5300_DONTCARE_LEN;

	_1_AD5300_ACT_G;

	for (iCnt = 0; iCnt < AD5300_DATA_LEN; iCnt++)
	{
		_1_SCLK_HI_G;

		(tmp & (unsigned short)( 1U << (15 - iCnt) ) ) ? (_1_MOSI_HI_G) : (_1_MOSI_LO_G);

		_1_SCLK_LO_G;
	}

	_1_AD5300_DEACT_G;
#endif
} /* void _1_AD5300_Write_W(unsigned short data) */

void test() 
{
int iIdx, iPdx;

	memset( (void*) GPIO_VALUE_FILES, 0, sizeof (GPIO_VALUE_FILES) );

	/* Initialize GPIO ports */
	for (iIdx = 0; iIdx < sizeof(GPIOs)/sizeof(GPIOs[0]);iIdx++ )
	{
//.		printf("[%s] [%s] opening GPIO %s \n",__FILE__, __func__ , GPIOs[iIdx] );

		OpenGPIO( GPIOs[iIdx] );	
	}
printf("[%s] [%s] opened all GPIO ports \n",__FILE__, __func__ );

#if !defined(SH_FOPS)

	/* Open GPIO value files and store file poniters in array <GPIO_VALUE_FILES> */
	for (iIdx = 0; iIdx < sizeof(GPIO_VALUE_FILES)/sizeof(GPIO_VALUE_FILES[0]);iIdx++ )
	{
		printf("[%s] [%s] opening GPIO value file for GPIO %s \n",__FILE__, __func__ , GPIOs[iIdx] );

		/* Filename prepare */
		sprintf (pcCmdBuffer, "/sys/class/gpio/gpio%s/value", GPIOs[iIdx]);

		/* Try to open value file <pcCmdBuffer> */
		if ( NULL == (GPIO_VALUE_FILES[iIdx] = fopen (pcCmdBuffer, "wb+") ) )
		{
			printf("[%s] [%s] can't open GPIO value file <%s> \n", __FILE__, __func__ , pcCmdBuffer);

			continue;
		}

	} /* for (iIdx = 0; ... */

printf("\n[%s] [%s]  SCLK_i_W : ", __FILE__, __func__ );
	/* GPIOs connected to SCKL with 'white' wire */
	for (iPdx = 0; iPdx < (NUM_PORTS-1); iPdx++)
{		SCLK_i_W[iPdx] = GPIO_VALUE_FILES[NUM_PORTS*iPdx];
printf("<%p> ", SCLK_i_W[iPdx]);
}

printf("\n[%s] [%s]  MOSI_i_W : ", __FILE__, __func__ );
	/* GPIOs connected to MOSI with 'white' wire */
	for (iPdx = 0; iPdx < (NUM_PORTS-1); iPdx++)
{		MOSI_i_W[iPdx] = GPIO_VALUE_FILES[NUM_PORTS*iPdx + 1];
printf("<%p> ", MOSI_i_W[iPdx]);
}

printf("\n[%s] [%s]  SYNC_i_W : ", __FILE__, __func__ );
	/* GPIOs connected to SYNC with 'white' wire */
	for (iPdx = 0; iPdx < (NUM_PORTS-1); iPdx++)
{		SYNC_i_W[iPdx] = GPIO_VALUE_FILES[NUM_PORTS*iPdx + 2];
printf("<%p> ", SYNC_i_W[iPdx]);
}

printf("\n[%s] [%s]  SCLK_i_G : ", __FILE__, __func__ );
	/* GPIOs connected to SCKL with 'green' wire */
	for (iPdx = 0; iPdx < (NUM_PORTS-1); iPdx++)
{		SCLK_i_G[iPdx] = GPIO_VALUE_FILES[NUM_PORTS*iPdx + 3];
printf("<%p> ", SCLK_i_G[iPdx]);
}

printf("\n[%s] [%s]  MOSI_i_G : ", __FILE__, __func__ );
	/* GPIOs connected to MOSI with 'green' wire */
	for (iPdx = 0; iPdx < (NUM_PORTS-1); iPdx++)
{		MOSI_i_G[iPdx] = GPIO_VALUE_FILES[NUM_PORTS*iPdx + 4];
printf("<%p> ", MOSI_i_G[iPdx]);
}

printf("\n[%s] [%s]  SYNC_i_G : ", __FILE__, __func__ );
	/* GPIOs connected to SYNC with 'green' wire */
	for (iPdx = 0; iPdx < (NUM_PORTS-1); iPdx++)
		SYNC_i_G[iPdx] = GPIO_VALUE_FILES[NUM_PORTS*iPdx + 5];



// TODO: remove block+

printf("\n[%s] [%s]  Workaround to check data in Osc\n", __FILE__, __func__ );	
	for (iPdx = 0; iPdx < (NUM_PORTS-1); iPdx++)
	{
		__tmp[iPdx] = MOSI_i_W[iPdx];
		MOSI_i_W[iPdx] = SYNC_i_G[iPdx];
		SYNC_i_G[iPdx] = __tmp[iPdx];
	}
// TODO: remove block-


fflush(stdout);// TODO: rem.

#endif /* !defined(SH_FOPS) */

	while (1) 
	{
#if defined(SH_FOPS)
		/* Cyclically toggle GPIO ports ON */
		for (iIdx = 0; iIdx < sizeof(GPIOs)/sizeof(GPIOs[0]);iIdx++ )
		{
//.			printf("[%s] [%s] toggling GPIO %s ON\n",__FILE__, __func__ , GPIOs[iIdx] );
			OnGPIO( GPIOs[iIdx] );
		}

		/* Cyclically toggle GPIO ports OFF */
		for (iIdx = 0; iIdx < sizeof(GPIOs)/sizeof(GPIOs[0]);iIdx++ )
		{
//.			printf("[%s] [%s] toggling GPIO %s OFF\n",__FILE__, __func__ , GPIOs[iIdx] );
			OffGPIO( GPIOs[iIdx] );
		}
#else		
		/* Cyclically toggle GPIO ports OFF */
		//.for (iIdx = 0; iIdx < sizeof(GPIOs)/sizeof(GPIOs[0]);iIdx++ )

		/* For each CPE port except last one (which is going to be USB 3.0, and conseq. requires special handling) */
		for (iIdx = 0; iIdx < NUM_PORTS - 1 /* skip USB 3.0 Port */;iIdx++ )

		{
			/* Test */
			//._1_AD5300_Write_W( 0xAA );
			_i_AD5300_Write_W(0xAA, iIdx); /* CH1: blue oscilloscope beam */
			//._1_AD5300_Write_G( 0x55 );
			_i_AD5300_Write_G(0x88, iIdx); /* CH2: yellow oscilloscope beam*/
		}
#endif /* defined(SH_FOPS) */

	} /* while (1) */

#if !defined(SH_FOPS)
	/* Open GPIO value files and store file poniters in array <GPIO_VALUE_FILES> */
	for (iIdx = 0; iIdx < sizeof(GPIO_VALUE_FILES)/sizeof(GPIO_VALUE_FILES[0]);iIdx++ )
	{
		printf("[%s] [%s] closing GPIO value file for GPIO %s \n",__FILE__, __func__ , GPIOs[iIdx] );

		/* Try to close value file <pcCmdBuffer> */
		if ( 0 != fclose ( GPIO_VALUE_FILES[iIdx] ) )
		{
			printf("[%s] [%s] can't close GPIO value file <%s> \n", __FILE__, __func__ , pcCmdBuffer);

			continue;
		}
	}/* for (iIdx = 0; ... */
#endif
}/* void main () */


void main () 
{
	// TODO: don't forget to get plainly initialized members of such arrays as SCLK_i_W[] et al. 
	
	test () ;
}
