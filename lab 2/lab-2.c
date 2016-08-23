       #include <stdint.h>
       #include <stdbool.h>
       #include "inc/tm4c123gh6pm.h"
       #include "inc/hw_memmap.h"
       #include "inc/hw_types.h"
       #include "driverlib/sysctl.h"
       #include "driverlib/interrupt.h"
       #include "driverlib/gpio.h"
       #include "driverlib/timer.h"
      uint32_t ui32Period;


void pin_setup(void)
{
		SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	    // Pin F4 setup
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        // Enable port F
	    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  // Init PF4 as input
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  // Enable weak pullup resistor for PF4

	    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

	    ui32Period = (SysCtlClockGet() / 10) / 1;
	    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	    IntEnable(INT_TIMER0A);
	    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	    IntMasterEnable();
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

	     TimerEnable(TIMER0_BASE, TIMER_A);
}

void interrupt_setup(void)
{

    // Interrupt setuü
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4);        // Disable interrupt for PF4 (in case it was enabled)
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);      // Clear pending interrupts for PF4
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,
        GPIO_FALLING_EDGE);             // Configure PF4 for falling edge trigger
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);     // Enable interrupt for PF4

}


extern int flag=0;
int main(void)
{

      pin_setup();
      while(1) {

      }
}
unsigned char DetectKeyPress(void)
{
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0)
            {
                flag=1;
            }
            else if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0)
            {
                flag=2;
            }
                return flag;
}
int f;
void Timer0Handler(void)
       {
        // Clear the timer interrupt
        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        // Read the current state of the GPIO pin and
        // write back the opposite state
        f=DetectKeyPress();
                if(f==2)
                 {
                     GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
                 }
                 else if(f==1)
                 {
                     GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);

                 }
                 else if(f==0)
                          {
                              GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);

                          }
        }
