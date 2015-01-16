/*
 * console.c
 *
 *  Created on: Nov 26, 2014
 *      Author: nikica
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "console.h"
//#include "tank.h"

#include "drv8840.h"


struct commandLineStruct{
	char *command;
	char *attribute1;
	char *attribute2;
	char *attribute3;
	// adding extra attributes if is required
}CommandLine;

typedef enum {
	command,
	attribute1,
	attribute2,
	attribute3
}attributeOrderInCommands;

//private variables
char Buff[100] = {0};

extern drv8840 Motor1;
extern drv8840State Motor1_state;

int consoleInit(volatile void (*pFunc)(const char *buff)){

	sendData = pFunc;

}

void printc(const char *format, ...)
{
	char VsBuffer[VS_BUFFER_SIZE] = {0};
	va_list args;

	va_start(args, format);

	vsprintf(VsBuffer, format, args);

	sendData((char*) VsBuffer);

	va_end(args);
}

void showStatus(void){

	printc( "\r\n");

}

// collecting arrived chars
int charArrived(char data){

	static int flag = 1;
	static int counter = 0;

	if(data == ENTER){	// Enter
		if(flag == 1){
			Buff[counter - 1] = '\0';
			commandParser((char*) Buff);

		}else if(flag == 0){
			Buff[counter] = '\0';
			commandParser((char*) Buff);
		}
		flag = 1;
		counter = 0;
		return sucess;

	}else if((data == BACKSPACE) && (counter > 0)){  //Backspace and counter
		counter--;
		if(Buff[counter] == SPACE){
			flag = 1;
		}else{
			flag = 0;
		}
		return sucess;

	}else if(data == SPACE){ //space
		if(flag == 0){
			Buff[counter] = data; // polnenje na baferot
			counter ++;
			flag = 1;
		}
		return sucess;

	}else if((counter < MaxCharNo) && (data != BACKSPACE)){ //
		Buff[counter] = data; // polnenje na baferot
		counter ++;
		flag = 0;
		return sucess;

	}else if(counter >= MaxCharNo){
		counter = 0;
		return error;
	}

	counter = 0;
	return undefineState;
}

void removeExstraSpaces(const char *pBuff){

	int i = 0;
	int j = 0;

	for(; i < MaxCharNo; i++){

		if((Buff[j] == ' ') && (Buff[i] != ' ')){
			Buff[j+1] = Buff[i];

		}else if((Buff[j] != ' ') && (Buff[i] != ' ')){
			j++;
		}

 	}
}

int commandParser(const char *pBuff){

	// |command|_|attribute1|_|attribute2|_|attribute3| <- command Buffer structure

	char *tmpPointer;
	char *tmpPointer2;
	attributeOrderInCommands status = command;
	int len;

	tmpPointer = pBuff;

	CommandLine.command 	= NULL;
	CommandLine.attribute1 	= NULL;
	CommandLine.attribute2 	= NULL;
	CommandLine.attribute3 	= NULL;

	// ToDo: sreduvanje da bafferot da ne se

	do{
		// ToDo: trganje na multipl od prazni mesta od komandnio string
		tmpPointer2 = strchr(tmpPointer, 0x20);	// search for space symbol

		if(tmpPointer2 != NULL){
			tmpPointer2[0] = '\0'; 					// cut the sting to the first space
		}

		switch(status){
		case command:
			CommandLine.command 	= tmpPointer;
			break;

		case attribute1:
			CommandLine.attribute1 	= tmpPointer;
			break;

		case attribute2:
			CommandLine.attribute2 	= tmpPointer;
			break;

		case attribute3:
			CommandLine.attribute3 	= tmpPointer;
			break;

		default:
			// error (command buffer is too long)
			break;

		}

		status++;

		if(status >= MAX_ATTRIBUTES_NUMBER){	// Check validity of the attribute number
			break;
		}

		tmpPointer = &tmpPointer2[1];	// Positioning to the next attribute

	}while(tmpPointer2 != NULL);


	// -- Implementation of the commands --

	// ToDo: implementirnje na switch za granenje na komandite
	if(strcmp(CommandLine.command, "status") == 0){ // command status
		showStatus();

	}else if(strcmp(CommandLine.command, "set") == 0){

		if(strcmp(CommandLine.attribute1, "fuel") == 0){

			if(CommandLine.attribute2 != 0){
				showStatus();

			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}


		}else if(strcmp(CommandLine.attribute1, "additive") == 0){

			if(CommandLine.attribute2 != 0){
				//sys_info.additive = atoi(CommandLine.attribute2);
				showStatus();

			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}

		}else if(strcmp(CommandLine.attribute1, "miles") == 0){

			if(CommandLine.attribute2 != 0){
				//sys_info.miles = atoi(CommandLine.attribute2);
				showStatus();

			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}

		}else if(strcmp(CommandLine.attribute1, "speed") == 0){

			if(CommandLine.attribute2 != 0){
				//sys_info.speed = atoi(CommandLine.attribute2);
				Motor1_state.CurrentValue = atoi(CommandLine.attribute2);
				setDriverMode(&Motor1, &Motor1_state);
				printc("\r\n");
				//showStatus();

			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}

		}else if(strcmp(CommandLine.attribute1, "phase") == 0){

			if(CommandLine.attribute2 != 0){
				//sys_info.speed = atoi(CommandLine.attribute2);
				Motor1_state.PhasePin = atoi(CommandLine.attribute2);
				setDriverMode(&Motor1, &Motor1_state);
				printc("\r\n");
				//showStatus();

			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}

		}else if(strcmp(CommandLine.attribute1, "dcy") == 0){

			if(CommandLine.attribute2 != 0){
				//sys_info.speed = atoi(CommandLine.attribute2);
				Motor1_state.DecayPin = atoi(CommandLine.attribute2);
				setDriverMode(&Motor1, &Motor1_state);
				printc("\r\n");
			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}

		}else if(strcmp(CommandLine.attribute1, "sleep") == 0){

			if(CommandLine.attribute2 != 0){
				//sys_info.speed = atoi(CommandLine.attribute2);
				Motor1_state.SleepPin = atoi(CommandLine.attribute2);
				setDriverMode(&Motor1, &Motor1_state);
				printc("\r\n");
			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}

		}else if(strcmp(CommandLine.attribute1, "reset") == 0){

			if(CommandLine.attribute2 != 0){
				//sys_info.speed = atoi(CommandLine.attribute2);
				Motor1_state.ResetPin = atoi(CommandLine.attribute2);
				setDriverMode(&Motor1, &Motor1_state);
				printc("\r\n");
			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}

		}else if(strcmp(CommandLine.attribute1, "enable") == 0){

			if(CommandLine.attribute2 != 0){
				//sys_info.speed = atoi(CommandLine.attribute2);
				Motor1_state.EnablePin = atoi(CommandLine.attribute2);
				setDriverMode(&Motor1, &Motor1_state);
				printc("\r\n");
			}else{
				// missed attribute 2
				printc("\r\n>>undefine command\r\n>>");
			}

		}else{
			printc("\r\n>>undefine command\r\n>>");
		}

	}else if(strcmp(CommandLine.command, "fuel.read") == 0){

	}else if(strcmp(CommandLine.command, "reset") == 0){
		asm("nop");
	}else{
		printc("\r\n>>undefine command\r\n>>");
	}

}
