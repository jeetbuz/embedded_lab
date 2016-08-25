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

extern uint8_t count=1;
extern int flag=0;
extern int auto_mode=1;
extern int sw1count=0;
extern int set=0;
extern int set1=0;
int bright=254;
extern int manual_mode=0;
extern int flagm=0;
extern int flaga=0;
extern int flagb=0;
extern int swcount=0;
extern int flagj=0;
extern int seth1=0;
extern int setm1=0;
extern int flagj1=0;



#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))

uint32_t ui32Period;

#define PWM_FREQUENCY 55

volatile uint32_t ui32Load;

void manual_mode1(void)
{   auto_mode=0;
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, bright * ui32Load / 1000);

}

void manual_mode2(void)
{auto_mode=0;
PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, bright * ui32Load / 1000);
}

void manual_mode3(void)
{auto_mode=0;
PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, bright * ui32Load / 1000);
}

void BlueInc(volatile uint8_t blue)
{ while(blue<254)
{
    if(auto_mode==0)
        return;
    if(flag!=2)

    blue++;
                if (blue > 254)
                {
                    blue = 254;
                }
    SysCtlDelay(count * 10000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blue * ui32Load / 1000);


}
}

void RedInc(volatile uint8_t red)
{ while(red<254)
{if(auto_mode==0)
    return;
    red++;
                if (red > 254)
                {
                    red = 254;
                }
    SysCtlDelay(count * 10000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red * ui32Load / 1000);


}
}


void GreenInc(volatile uint8_t green)
{ while(green<254)
{if(auto_mode==0)
    return;
    green++;
                if (green > 254)
                {
                    green = 254;
                }
    SysCtlDelay(count * 10000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, green * ui32Load / 1000);


}
}

void BlueDec(volatile uint8_t blue)
{ while(blue>10)
{
    if(auto_mode==0)
            return;
blue--;
                if (blue < 10)
                {
                    blue = 10;
                }
    SysCtlDelay(count * 10000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blue * ui32Load / 1000);


}
}

void RedDec(volatile uint8_t red)
{ while(red>10)
{
    if(auto_mode==0)
            return;
    red--;
                if (red < 10)
                {
                    red = 10;
                }
    SysCtlDelay(count * 10000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red * ui32Load / 1000);


}
}


void GreenDec(volatile uint8_t green)
{ while(green>10)
{
    if(auto_mode==0)
            return;
    green--;
                if (green <10)
                {
                    green = 10;
                }
    SysCtlDelay(count * 10000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, green * ui32Load / 1000);


}
}




int main(void)
{
    volatile uint32_t ui32PWMClock;
    volatile uint8_t ui8Red,ui8Blue,ui8Green;
    ui8Blue = ui8Red= ui8Green=1;
    volatile uint8_t ui8Adjust;
    ui8Adjust = 1;

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

    /// INTERRUPT SETUP

 //   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);




    while(1)
    {
       SysCtlDelay(100000);
        if(auto_mode==1)
        {
           BlueInc(10);
           RedInc(10);
           BlueDec(255);
           GreenInc(10);
           RedDec(255);
           BlueInc(10);
           GreenDec(255);
           BlueDec(255);
        }

        if(auto_mode==0)
        {   sw1count=0;
            if(manual_mode==1)
                manual_mode1();
            if(manual_mode==2)
                manual_mode2();
            if(manual_mode==3)
                manual_mode3();
        }



    }

}


unsigned char DetectKeyPress(void)
{

 //         Auto Mode delay control
    if((flaga==0)&&((auto_mode==1)&&(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))))
           {
               if(set1==1)
               {

                   count=count+1;
                       if(count>100)
                           count=100;

                   set1=0;
                   flaga=3;
               }
               set1=1;
           }
           else if((auto_mode==1)&&(flaga==0)&&(set1==1)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))
           {
               set1=0;
           }
           else if((auto_mode==1)&&(flaga==3)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))
           {
               set1=0;
               flaga=0;
           }

    if((flagb==0)&&((auto_mode==1)&&(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))))
               {
                   if(set==1)
                   {
                       count=count-1;
                               if(count<=1)
                                   count=1;

                       set=0;
                       flagb=4;
                   }
                   set=1;
               }
               else if((auto_mode==1)&&(flagb==0)&&(set==1)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
               {
                   set=0;
               }
               else if((auto_mode==1)&&(flagb==4)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
               {
                   set=0;
                   flagb=0;
               }

    //      Manual mode   case 1 and case 2

    if((!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))
                   {
                    if(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))

                   {    set=0;
                //   sw1count=0;
                //   swcount=0;


                        while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
                   {

                   if(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
                   {
               swcount++;
                   }

                    if((flagm==0)&&(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))))
                   {
                        if(set==1)
                        {set =0;
                        flagm=1;
                       sw1count++;
                        }
                       set=1;
                   }
                   else if((set==1)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
                   {
                       set=0;
                   }
                   else if((flagm==1)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
                   {   set=0;
                       flagm=0;
                   }
                   }
                   }
                   }




    if(sw1count>=3)
            {   swcount=0;
                set=0;
                auto_mode=0;
                manual_mode=3;
            }

        if(sw1count==2)
        {   sw1count=0;
            set=0;
            auto_mode=0;
            manual_mode=2;
        }


         if(sw1count==1)
         {  sw1count=0;
             set=0;
             auto_mode=0;
             manual_mode=1;

         }

 ////   manual mode brightness control


         if((flagj==0)&&((auto_mode==0)&&(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))))
                    {
                        if(setm1==1)
                        {

                            bright=bright+20;
                                if(bright>254)
                                    bright=254;

                            setm1=0;
                            flagj=3;
                        }
                        setm1=1;
                    }
                    else if((auto_mode==0)&&(flagj==0)&&(setm1==1)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))
                    {
                        setm1=0;
                    }
                    else if((auto_mode==0)&&(flagj==3)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)))
                    {
                        setm1=0;
                        flagj=0;
                    }

             if((flagj1==0)&&((auto_mode==0)&&(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))))
                        {
                            if(seth1==1)
                            {
                                bright=bright-20;
                                        if(bright<=1)
                                            bright=1;

                                seth1=0;
                                flagj1=4;
                            }
                            seth1=1;
                        }
                        else if((auto_mode==0)&&(flagj1==0)&&(seth1==1)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
                        {
                            seth1=0;
                        }
                        else if((auto_mode==0)&&(flagj1==4)&&(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
                        {
                            seth1=0;
                            flagj1=0;
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

        }
