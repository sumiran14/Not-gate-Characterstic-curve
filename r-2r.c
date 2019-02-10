#include <msp430.h>
#include <inttypes.h>
#include <stdbool.h>


// Port 2 is the input for R-2r DAC ladder. 1.0is the output of the ladder
//reading is from analog channel  A0
int y;
int inputval;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    //configure port 2.6 and 2.7 as GPIO
    P2SEL &=~( BIT6 + BIT7);
    P2SEL2&=~(BIT6 + BIT7);

    //ADC Settings
    ADC10CTL0  &=~ENC;
    ADC10CTL1|=INCH_3;      // select channel A0
    ADC10CTL1|=ADC10SSEL_2; //SMCLK clk source
    ADC10AE0=0x09; // enable A0 only on 1.0
    ADC10CTL0|=ADC10ON; //ADC is on
    inputval =0;

    P2DIR = 0xff; // switch on all ports
    P2OUT =inputval;
    while (1)
    {
        ADC10CTL0|= ENC ;
        ADC10CTL0|=ADC10SC;

        while(ADC10CTL1 & ADC10BUSY);
        y= ADC10MEM;
               __delay_cycles(1);
        inputval++;
       P2OUT =inputval;
        __delay_cycles(1000);

    }
	return 0;
}
