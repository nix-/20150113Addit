/*
 * console.h
 *
 *  Created on: Nov 26, 2014
 *      Author: nikica
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#define VS_BUFFER_SIZE		256

#define MaxCharNo 100
#define MAX_ATTRIBUTES_NUMBER 4

#define undefineState	-1
#define error 			0
#define sucess 			1

#define SPACE		0x20
#define ENTER		0x0D
#define BACKSPACE	'\177'

void printc(const char *format, ...);
volatile void (*sendData)(const char *buff);
int consoleInit(volatile void (*pFunc)(const char *buff));
int charArrived(char data);
int commandParser(const char *pBuff);

#endif /* CONSOLE_H_ */
