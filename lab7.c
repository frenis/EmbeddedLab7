// This code has not been tested and is incomplete.

#include <msp430.h>

volatile float voltage1, voltage2;
int data = 0;

void uart_init(void);
void ConfigureAdc_light();

unsigned int adc[4];
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    uart_init();
    ConfigureAdc_light();

    _enable_interrupts();

    while(1){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc;
        voltage1 = adc[0]*3.3/1023; // Convert ADC output to voltage
        voltage2 = adc[3]*3.3/1023;
    }
}
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIA0TX_ISR(void){
    if (data > 0) {
        UCA0TXBUF =  voltage1;
        data = 0;
    }
    else {
        UCA0TXBUF =  voltage2;
        data = 1;
    }

}
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void){
    P1OUT = UCA0RXBUF;
}
void uart_init(void){
    //clock//baudrate//modulation//sw rest//TXRX interrupt
    UCA0CTL1 |= UCSSEL_2;                    //Select SMCLK as the baud rate generator source
    UCA0BR1 = 0;                            //high byte
    UCA0BR0 = 104;                          //low byte      //Produce a 9,600 Baud UART rate
    UCA0MCTL = UCBRS_1;                     //Second modulation select stage is 1
    UCA0STAT |= UCLISTEN;
    UCA0CTL1 &= ~UCSWRST;                    //clear software reset, resume operaion
    IE2 |= UCA0RXIE | UCA0TXIE;              //Enable the UART Transmit receiver Interrupt

}
void ConfigureAdc_light(){
    ADC10CTL1 = INCH_3 + ADC10DIV_0 + CONSEQ_3 + SHS_0; // read inputs from A0 to A3
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON;    // using 5V reference
    ADC10AE0 = BIT3 + BIT0;
    ADC10DTC1 = 4;
}
