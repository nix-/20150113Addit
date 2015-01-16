/*
 * drv8840.h
 *
 *  Created on: Nov 29, 2014
 *      Author: nix
 */

#ifndef DRV8840_H_
#define DRV8840_H_

#define SET			1
#define RESET		0

#define WAKEUP		1
#define SLEEP		0

#define ERROR 		1
#define NONE		0

#define SLOW		0
#define FAST		1

#define FORWARD 	1
#define BACKWARD 	0

#define ENABLE 		1
#define DISABLE 	0

// motor driver status structure
typedef struct drv8840StateTag{
	int ResetPin;
	int SleepPin;
	int FaultPin;
	int DecayPin;	// treba da e na 0 (taka poubavo ravboti - neznam zosto !!!proveri)
	int PhasePin;
	int EnablePin;
	int CurrentValue;
}drv8840State;

// motor driver structure
typedef struct{
	int (*setReset)(int state);		// pointer to the function that handles Reset pin state
	int (*setSleep)(int state);		// pointer to the function that handles Sleep pin state
	int (*readFault)(void);			// pointer to the function that reads Fault pin status
	int (*setDecay)(int state);		// pointer to the function that handles Decay pin state
	int (*setPhase)(int state);		// pointer to the function that handles Phase pin state
	int (*setEnable)(int state);	// pointer to the function that handles Enable pin state
	int (*setCurrent)(int value);	// pointer to the function that handles Current interface state

}drv8840;

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
		);

/*****************************************************************************
** Function name:		setMotorMode
**
** Descriptions:		function for setting the state to a motor driver
** 						(first should be called initDriver function)
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
void setMotorMode(
		drv8840 *driver,
		int reset,
		int sleep,
		int decay,
		int phase,
		int enable,
		int current
		);

#endif /* DRV8840_H_ */
