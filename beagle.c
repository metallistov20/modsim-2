/* stdout, NULL */
#include <stdio.h>

/* sprintf() */
#include <string.h>

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


FILE * GPIO_VALUE_FILES[] = {

	 NULL, NULL, NULL , NULL, NULL, NULL, NULL, NULL,
	 NULL, NULL, NULL , NULL, NULL, NULL, NULL, NULL,
	 NULL, NULL, NULL , NULL, NULL, NULL, NULL,
	 NULL, NULL, NULL , NULL, NULL, NULL, NULL   
};


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
		sprintf (pcCmdBuffer, "out > /sys/class/gpio/gpio%s/direction", pcPortStr);

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
static void OnGPIO(char * pcPortStr)
#else
static void OnGPIO(FILE * fcPortFile)
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
static void OffGPIO(char * pcPortStr)
#else
static void OffGPIO(FILE * fcPortFile)
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

void test () 
{
int iIdx;

	/* Initialize GPIO ports */
	for (iIdx = 0; iIdx < sizeof(GPIOs)/sizeof(GPIOs[0]);iIdx++ )
	{
		printf("[%s] [%s] opening GPIO %s \n",__FILE__, __func__ , GPIOs[iIdx] );

		OpenGPIO( GPIOs[iIdx] );	
	}

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

	while (1) 
	{	/* Cyclically toggle GPIO ports ON */
		for (iIdx = 0; iIdx < sizeof(GPIOs)/sizeof(GPIOs[0]);iIdx++ )
		{
//.			printf("[%s] [%s] toggling GPIO %s ON\n",__FILE__, __func__ , GPIOs[iIdx] );
#if defined(SH_FOPS)
			OnGPIO( GPIOs[iIdx] );
#else
			OnGPIO( GPIO_VALUE_FILES[iIdx] );
#endif /* (0) */
		}

		/* Cyclically toggle GPIO ports OFF */
		for (iIdx = 0; iIdx < sizeof(GPIOs)/sizeof(GPIOs[0]);iIdx++ )
		{
//.			printf("[%s] [%s] toggling GPIO %s OFF\n",__FILE__, __func__ , GPIOs[iIdx] );
#if defined(SH_FOPS)
			OffGPIO( GPIOs[iIdx] );
#else
			OffGPIO( GPIO_VALUE_FILES[iIdx] );
#endif /* (0) */
		}
	} /* while (1) */

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

}/* void main () */


void main () 
{
	test () ;
}
