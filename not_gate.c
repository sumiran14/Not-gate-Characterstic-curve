#include <msp430.h>
#include <inttypes.h>
#include <stdbool.h>

//PWM wave duty cycles's can be changed by any analog device and voltage will be shown on LCD.
int y,z,flag,s;
int i,k=0;
uint8_t val =0;

char ch[4];
//----------------------------------------------------------------------------------------------------------------
/*void convert()
{
   float x = 0.25;
   float z = x*y;   //8 bit ADC
   int val = z;
}*/
//----------------------------------------------------------------------------------------------------------------

/*void pwm()
{
    TA1CCR0 = 256;          //8 bit PWM mode // 62.5 kHz PWM
    TA1CCR1 = 127;          //initial width

    TA1CTL |= TASSEL_2;     //select SMCLK as source
    TA1CTL |= TACLR;        //clear
    TA1CTL |= MC_1;         //up mode

    TA1CCTL1 |= OUTMOD_7;   //reset set mode

    // P2.1 is set as PWM
    P2DIR |= BIT1;
    P2SEL |= BIT1;
    P2SEL2&=~BIT1;
}*/
//----------------------------------------------------------------------------------------------------------------

void clock_init()
{
    BCSCTL1 = CALBC1_16MHZ;                              // Set DCO to 16MHz
    DCOCTL  = CALDCO_16MHZ;
    __delay_cycles(100);
}
//----------------------------------------------------------------------------------------------------------------
void adc_init()
{
    ADC10CTL0  &=~ENC;
    ADC10CTL1|=INCH_0;      // select channel A0
    ADC10CTL1|=ADC10SSEL_2; //SMCLK clk source
    ADC10AE0=0x09; // enable A0 only on 1.0
    ADC10CTL0|=ADC10ON; //ADC is on
    flag=0;
}
//--------------------------------------------------------------------------------------------------------------
void uart_init()
{
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 82;                             // 16MHz 9600
    UCA0BR1 = 06;                              // 16MHz 9600
    UCA0MCTL = 0x0C;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}
//------------------------------------------------------------------------------------------------------------
/*unsigned char recieve()
{
    unsigned char rx;
    while (!(IFG2 & UCA0RXIFG));              // USCI_A0 RX buffer ready?
    rx = UCA0RXBUF;
    return rx;
}*/
//-------------------------------------------------------------------------------------------------------------
void transmit(unsigned char a)
{
    while(!flag);
    while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
    UCA0TXBUF = a;
    flag=0;
}
//--------------------------------------------------------------------------------------------------------------
void transmitstr(const char* str)
{
    while(*str)
        transmit(*str++);
}

//--------------------------------------------------------------------------------------------------------------
void convertIntToStr()
{
    int digi[4]; int z =y;
    digi[0] = z/1000;
    z=z%1000;
    digi[1]=z/100;
    z=z%100;
    digi[2]=z/10;
    z=z%10;
    digi[3]=z;
    //sending higher digit first
    ch[0] = 0x30 + digi[0];
    ch[1] = 0x30 + digi[1];
    ch[2] = 0x30 + digi[2];
    ch[3] = 0x30 + digi[3];
}
//---------------------------------------------------------------------------------------------------------------
int main(void)

{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P2SEL &=~( BIT6 + BIT7);
    P2SEL2&=~(BIT6 + BIT7);

    P2DIR = 0xff; // switch on all ports
    int aaky = 0;
    P2OUT = aaky;

    int over =0;

    clock_init();
 //   pwm();
    adc_init();
    uart_init();

    __bis_SR_register(GIE);       //  interrupts enabled
    while(1)
    {
        __delay_cycles(1000);
        ADC10CTL1 =INCH_0;      // select channel A0 // vin
        ADC10CTL0|= ENC ;
        ADC10CTL0|=ADC10SC;
        y=0;
        for(k=0;k<5;k++)
        {
            while(ADC10CTL1 & ADC10BUSY);
            y+= ADC10MEM;
        }
        ADC10CTL0  &=~(ENC+ADC10SC);
        y=y/5;
//       convert();
        convertIntToStr();
        transmitstr(ch);
        while(!flag);
      //  __delay_cycles(3200000);
         __delay_cycles(1);
        ADC10CTL1 =INCH_3;      // select channel A3 // vout
        ADC10CTL0|= ENC ;
        ADC10CTL0|=ADC10SC;
        y=0;
        for(k=0;k<5;k++)
        {
            while(ADC10CTL1 & ADC10BUSY);
            y+= ADC10MEM;
        }
        ADC10CTL0  &=~(ENC+ADC10SC);
        y=y/5;
//       convert();
        convertIntToStr();
        transmitstr(ch);
        while(!flag);
  //     __delay_cycles(3200000);
        __delay_cycles(1);
        over ++;
        aaky++;
        P2OUT = aaky;
        //val++;
        //TA1CCR1 = val;          //initial width
        if (over == 256)
            break;
    }
    while(1)
    {
        char end = 0x4F;
        transmit(end);
    }
    return 0;
}
//---------------------------------------------------------------------------------------------

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    if(i==0)
    {
        while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
        UCA0TXBUF = 0x30;       // TX -> 0
        flag =1;
        i++;
    }
    if(i)
    {
        s=UCA0RXBUF;
        if(s==0x31)
            flag=1;
    }
}

