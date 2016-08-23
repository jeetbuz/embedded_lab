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
#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))

void pin_setup(void)
{
        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
        // Pin F4 setup
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        LOCK_F=0x4C4F434BU;
        CR_F=GPIO_PIN_0|GPIO_PIN_4;
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
        GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
        // Turn on the LED
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

        ui32Period = (SysCtlClockGet() / 10) / 2;
        TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

        IntEnable(INT_TIMER0A);
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        IntMasterEnable();

         TimerEnable(TIMER0_BASE, TIMER_A);
}



int flag = 0;
int set=0;

int main(void)
{
      pin_setup();
      while(1) {
                  }
}

unsigned char DetectKeyPress(void)
       {
        // Clear the timer interrupt
        // Read the current state of the GPIO pin and
        // write back the opposite state
        if((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0)|(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0))
        {
            if(set==1)
            {
                set=0;
                flag=1;
            }
            set=1;
        }
        else if((flag==0)&&(set==1)&&((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)!=0)|(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)!=0)))
        {
            set=0;
        }
        else if((flag==1)&&((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)!=0)|(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)!=0)))
        {
            set=0;
            flag=0;
        }

        return flag;
        }
int f;
int c=1;
void Timer0Handler(void)
       {
        // Clear the timer interrupt
        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        // Read the current state of the GPIO pin and
        // write back the opposite state
      f=DetectKeyPress();
      if (f==0)
      {c=1;
      }
      if((f==1)&&(c==1))
                {c=0;
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

       }
