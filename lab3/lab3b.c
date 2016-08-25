#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
 #include "inc/tm4c123gh6pm.h"


#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))
uint32_t ui32Period;

extern int set=0;
extern int flag=0;

extern int set2=0;
extern int flag2=0;
float p=1;
volatile uint32_t ui32Load;


#define PWM_FREQUENCY 55

int main(void)
{

    volatile uint32_t ui32PWMClock;
        volatile uint8_t ui8Red,ui8Blue,ui8Green;
        ui8Blue = ui8Red= ui8Green=1;
        volatile uint8_t ui8Adjust;
        ui8Adjust = 55;

        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
       SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
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
        GPIOPinConfigure(GPIO_PF1_M1PWM5);
        GPIOPinConfigure(GPIO_PF2_M1PWM6);
        GPIOPinConfigure(GPIO_PF3_M1PWM7);
        GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    //    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

        HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
        HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

        ui32PWMClock = SysCtlClockGet() / 64;
        ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
        PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN );
        PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN );
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);

        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Red * ui32Load / 1000);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Blue * ui32Load / 1000);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Green * ui32Load / 1000);

        PWMGenEnable(PWM1_BASE, PWM_GEN_3);
        PWMGenEnable(PWM1_BASE, PWM_GEN_2);

        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);




        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

        GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);

        // Turn on the LED


        GPIOPinConfigure(GPIO_PD0_M0PWM6);
        GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);


        PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN );


        PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, ui32Load);

        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 110 * ui32Load / 1000);

        PWMGenEnable(PWM0_BASE, PWM_GEN_3);


        PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
    while(1)
    {

// PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 55 * ui32Load / 1000);
if(p>1)

        if(flag==1)
        {
            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,  3*27* ui32Load / 1000);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 5 * ui32Load / 1000);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 254 * ui32Load / 1000);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 5 * ui32Load / 1000);
        }

        else if(flag2==1)
        {

            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, p*27 * ui32Load / 1000);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 5 * ui32Load / 1000);
             PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 4 * ui32Load / 1000);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 254 * ui32Load / 1000);

        }
    }

}





unsigned char DetectKeyPress(void)
       {

    //         for switch 2
       if((flag==0)&&(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))
              {
                  if(set==1)
                  {
                      set=0;
                      flag=1;
                  }
                  set=1;
              }
       else if((set==1)&&(flag==0)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))
               {
           set=0;
               }

       else if((flag==1)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))
               {
           set=0;
           flag=0;
               }

// for switch 1
       if((flag2==0)&&(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
                     {
                         if(set2==1)
                         {
                             set2=0;
                             flag2=1;
                         }
                         set2=1;
                     }
              else if((set2==1)&&(flag2==0)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
                      {
                  set2=0;
                      }

              else if((flag2==1)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
                      {
                  set2=0;
                  flag2=0;
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

       }

