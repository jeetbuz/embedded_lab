/*

* Author: Texas Instruments

* Editted by: Saurav Shandilya, Vishwanathan Iyer
	      ERTS Lab, CSE Department, IIT Bombay

* Description: This code will test file to check software and hardware setup. This code will blink three colors of RGB LED present of Launchpad board in sequence.

* Filename: lab-0.c

* Functions: setup(), led_pin_config(), main()

* Global Variables: none

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))
/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals

* Example Call: setup();

*/
void setup(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	        // ADD Tiva-C GPIO setup - enables port, sets pins 1-3 (RGB) pins for output
	        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	        LOCK_F=0x4C4F434BU;
	        CR_F=GPIO_PIN_0|GPIO_PIN_4;
	        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
	        GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	        // Turn on the LED
	        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
}

/*

* Function Name: led_pin_config()

* Input: none

* Output: none

* Description: Set PORTF Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.

* Example Call: led_pin_config();

*/
void delay(void);

void RGB(void);

void led_pin_config(void)
{
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}


int main(void)
{

	setup();
	led_pin_config();
    int SW2status=1;

	while(1)                                                                        // forever loop
	   {
        if(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))) //if SW_1 is pressed
        {RGB();                                                        // toggle LED
        delay();
            }
        if(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0 ))) //if SW_1 is pressed
                 {
        				SW2status = SW2status + 1;
                		while((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 )))
                			{RGB();                                                        // toggle LED
                		           delay();                                                                                            //if GREEN is turn on
                			}
        	           }
        }



}
void RGB(void)
{
        // LED values - 2=RED, 4=BLUE, 8=GREEN
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
        {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
        }
        else if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
        {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
        }
        else
        {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
        }
}



void delay(void)
{
         SysCtlDelay(6700000);                // creates ~500ms delay - TivaWare fxn
}
