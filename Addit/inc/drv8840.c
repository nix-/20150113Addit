/*
 * drv8840.c
 *
 *  Created on: Nov 29, 2014
 *      Author: nix
 */
#include "drv8840.h"

/*****************************************************************************
** Function name:		initDriver
**
** Descriptions:		initialization of the control interface
**
** parameters:			function for handling state of the Reset pin
** 						function for handling state of the Sleep pin
** 						function for reading the state of the Fault pin
** 						function for handling state of the Decay pin
** 						function for handling state of the Phase pin
** 						function for handling state of the Enable pin
** 						function for handling value of the Current
**
** Returned value:		None
**
*****************************************************************************/
void initDriver(
		drv8840 *driver,
		int (*setReset)(int state),
		int (*setSleep)(int state),
		int (*readFault)(void),
		int (*setDecay)(int state),
		int (*setPhase)(int state),
		int (*setEnable)(int state),
		int (*setCurrent)(int value)
		){

	driver->setReset 	= setReset;
	driver->setSleep 	= setSleep;
	driver->readFault 	= readFault;
	driver->setDecay	= setDecay;
	driver->setPhase	= setPhase;
	driver->setEnable	= setEnable;
	driver->setCurrent	= setCurrent;

}

/*****************************************************************************
** Function name:		setDriverMode
**
** Descriptions:		function for setting the state to a motor driver
**
** parameters:			motor driver structure pointer
** 						state of the Reset pin
** 						state of the Sleep pin
** 						state of the Decay pin
** 						state of the Phase pin
** 						state of the Enable pin
** 						value of the output Current (motor current)
**
** Returned value:		None
**
*****************************************************************************/
void setDriverMode(drv8840 *driver, drv8840State *driverState){

	driver->setReset(driverState->ResetPin);
	driver->setSleep(driverState->SleepPin);
	driver->setDecay(driverState->DecayPin);
	driver->setPhase(driverState->PhasePin);
	driver->setEnable(driverState->EnablePin);
	driver->setCurrent(driverState->CurrentValue);

}
