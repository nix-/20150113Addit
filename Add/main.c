/*
 * main.c
 *
 *  Created on: Jan 16, 2015
 *      Author: nikica
 */

#include "VirtualSerial.h"
#include <string.h>

#include "console.h"

#include "drv8840.h"
#include "interfaces.h"

drv8840 Motor1;
drv8840State Motor1_state = {1, 1, 1, 0, 0, 1, 0x1f};

static void sendBuff(const char *buff);

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

#define ECHO_CHARACTER_TASK     (0)
#define CDC_BRIDGE_TASK         (ECHO_CHARACTER_TASK + 1)

int test;
char buff[20] = {0};

/** LPCUSBlib CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface = {
	.Config = {
		.ControlInterfaceNumber         = 0,

		.DataINEndpointNumber           = CDC_TX_EPNUM,
		.DataINEndpointSize             = CDC_TXRX_EPSIZE,
		.DataINEndpointDoubleBank       = false,

		.DataOUTEndpointNumber          = CDC_RX_EPNUM,
		.DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
		.DataOUTEndpointDoubleBank      = false,

		.NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
		.NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
		.NotificationEndpointDoubleBank = false,
		.PortNumber             = 0,
	},
};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs
 */
// static FILE USBSerialStream;

/** Select example task, currently lpc11Uxx and lpc17xx don't support for bridging task
 * Only LPC18xx has this feature */
#define CDC_TASK_SELECT ECHO_CHARACTER_TASK

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/** Configures the board hardware and chip peripherals for the demo's functionality. */
static void SetupHardware(void)
{
	Board_Init();

	USB_Init(VirtualSerial_CDC_Interface.Config.PortNumber, USB_MODE_Device);

#if defined(USB_DEVICE_ROM_DRIVER)
	UsbdCdc_Init();
#endif
}

static void sendBuff(const char *buff){

	CDC_Device_SendData(&VirtualSerial_CDC_Interface, (char *) buff, strlen(buff));

}

#if (CDC_TASK_SELECT == ECHO_CHARACTER_TASK)
/** Checks for data input, reply back to the host. */
static void EchoCharacter(void)
{
	/* Echo back character */
	uint8_t recv_byte[CDC_TXRX_EPSIZE];
#if !defined(USB_DEVICE_ROM_DRIVER)
	if (CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface)) {
		// Tuka vraka echo

/*		MC24LC64_Init(); // ToDo: treba da se iskoristat funkciite koi se povikuvaat vo ovaa rutina

		//Chip_I2C_Init(I2C1);
		char buff[]="n";
		uint8_t RdBuffer[10];

		//MC24LC64_Wr(0x000F, "Mr Nikica.S", sizeof("Mr Nikica.S"));

		MC24LC64_Rd(0x000F, RdBuffer, sizeof(RdBuffer));

		//PCF8574_Wr(PCF8574_ADDR, 0xFF);

		PCF8574_Rd(PCF8574_ADDR, RdBuffer);

		HD44780_IO_Init();

		display(1,2, "Nikica Srezoski");

		while(1){
			test = takeDataADC_ADS1231();
			sprintf(buff, "%d\r\n", test);
			CDC_Device_SendData(&VirtualSerial_CDC_Interface, buff, strlen(buff));
		}
*/

		recv_byte[0] = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
		CDC_Device_SendData(&VirtualSerial_CDC_Interface, (char *) recv_byte, 1);

		charArrived(recv_byte[0]);

	}
#else
	uint32_t recv_count;
	recv_count = UsbdCdc_RecvData(recv_byte, CDC_TXRX_EPSIZE);
	if (recv_count) {
		UsbdCdc_SendData(recv_byte, recv_count);
	}
#endif

}

#else
/** USB-UART Bridge Task */
static void CDC_Bridge_Task(void)
{
	/* Echo back character */
	uint8_t out_buff[CDC_TXRX_EPSIZE], in_buff[CDC_TXRX_EPSIZE];
	uint32_t recv_count;
#if !defined(USB_DEVICE_ROM_DRIVER)
	recv_count = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);
	while (recv_count--) {
		out_buff[0] = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
		Serial_Send((uint8_t *) out_buff, 1, BLOCKING);
	}

	recv_count = Serial_Revc(in_buff, CDC_TXRX_EPSIZE, NONE_BLOCKING);
	if (recv_count) {
		CDC_Device_SendData(&VirtualSerial_CDC_Interface, (char *) in_buff, recv_count);
	}
#else
	recv_count = UsbdCdc_RecvData(out_buff, CDC_TXRX_EPSIZE);
	if (recv_count) {
		Serial_Send((uint8_t *) out_buff, recv_count, BLOCKING);
	}

	recv_count = Serial_Revc(in_buff, CDC_TXRX_EPSIZE, NONE_BLOCKING);
	if (recv_count) {
		UsbdCdc_SendData(in_buff, recv_count);
	}
#endif
}

#endif

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	int tog = 0;

	SetupHardware();

	consoleInit(sendBuff);

	initInterface();

	initDriver(	&Motor1,
				setResetModule1,
				setSleepModule1,
				readFaultModule1,
				setDecayModule1,
				setPhaseModule1,
				setEnableModule1,
				setCurrentModule1
				);

	setDriverMode(&Motor1, &Motor1_state);

	for (;; ) {
#if defined(USB_DEVICE_ROM_DRIVER)
		UsbdCdc_IO_Buffer_Sync_Task();
#endif

#if (CDC_TASK_SELECT == ECHO_CHARACTER_TASK)
		EchoCharacter();
#else
		CDC_Bridge_Task();
#endif
#if !defined(USB_DEVICE_ROM_DRIVER)
		// CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask(VirtualSerial_CDC_Interface.Config.PortNumber, USB_MODE_Device);
#endif

		Delay_MS(1000/Motor1_state.CurrentValue);

		switch(tog){
		case 0:
			Motor1_state.CurrentValue++;
			if(Motor1_state.CurrentValue == 22)
				tog = 1;
			Motor1_state.CurrentValue &= 0x1f;
			break;
		case 1:
			Motor1_state.CurrentValue--;
			if(Motor1_state.CurrentValue == 6)
				tog = 0;
			Motor1_state.CurrentValue &= 0x1f;
			break;
		}

		//Motor1_state.PhasePin = tog;
		setDriverMode(&Motor1, &Motor1_state);
	}
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

	//	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	VirtualSerial_CDC_Interface.State.LineEncoding.BaudRateBPS = 921600;  // set baudRate
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

#if !defined(USB_DEVICE_ROM_DRIVER)
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t *const CDCInterfaceInfo)
{
	/*TODO: add LineEncoding processing here
	 * this is just a simple statement, only Baud rate is set */
	// Serial_Init(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS, false);
}

#else
void EVENT_UsbdCdc_SetLineCode(CDC_LINE_CODING *line_coding)
{
	// Serial_Init(VirtualSerial_CDC_Interface.State.LineEncoding.BaudRateBPS, false);
}

#endif
