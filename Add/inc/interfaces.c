/*
 * interfaces.c
 *
 *  Created on: Nov 29, 2014
 *      Author: nix
 */
#include "interfaces.h"
#include "LPC17xx.h"


/*****************************************************************************
** Function name:		initInterface
**
** Descriptions:		initialization of the used peripherals of the uC
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
int initInterface(void)
{
	// *** Peripheral for driving motor1 *** //
	LPC_PINCON->PINSEL3 &= ~(0x0f000000); 	//set P1.28 and P1.29 as GPIO
	LPC_GPIO1->FIODIR 	|= 0x30000000; 		//set P1.28 and P1.29 as output

	LPC_PINCON->PINSEL4 &= ~(0x0003ffff);	//set P2.0 - P2.8 set as GPIO
	LPC_GPIO2->FIODIR	|= 0x000001ff;		//set P2.0 - P2.8 set as output
	LPC_GPIO2->FIODIR	&= ~(0x00000001);	//set P2.0 as input

	return 1;

}

// END OF FILE
