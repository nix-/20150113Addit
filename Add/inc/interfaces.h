/*
 * interfaces.h
 *
 *  Created on: Nov 29, 2014
 *      Author: nix
 */

#ifndef INTERFACES_H_
#define INTERFACES_H_

#define HIGH	1
#define LOW		0


// ToDo: ova treba da se brishe nadole go ostaviv samo za primer
// DRV8840 module 1 //
/********************
P1.28	- Reset
P1.29	- Sleep
P2.0	- Fault
P2.1	- Decay
P2.2	- Phase
P2.3	- Enable
P2.4	- I0|
P2.5	- I1|
P2.6	- I2|>> Current
P2.7	- I3|   in [%]
P2.8	- I4|
********************/
int initInterface(void);
int setResetModule1(int state); 	// module 1
int setSleepModule1(int state);		// module 1
int readFaultModule1(void);			// module 1
int setDecayModule1(int state);		// module 1
int setPhaseModule1(int state);		// module 1
int setEnableModule1(int state);	// module 1
int setCurrentModule1(int value);	// module 1

#endif /* INTERFACES_H_ */
