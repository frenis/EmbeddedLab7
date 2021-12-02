#include <msp430.h>

volatile float voltage1, voltage2;
char result[100];
void uart_init(void);
void ConfigureAdc_light();
void port_init();
void itoa(int value, char* str, int base);
void strreverse(char* begin, char* end);

unsigned int adc[4];
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    port_init();
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

        int acount =0;
        result[acount] = 'A';
        while((IFG2 & UCA0TXIFG)==0);

        UCA0TXBUF = result[acount];
        _delay_cycles(1500000*15);


        itoa((adc[0]),result,10);
        while(result[acount]!='\0')
        {

            while((IFG2 & UCA0TXIFG)==0);

            UCA0TXBUF = result[acount++] ;                   //Transmit the received data.
        }

        _delay_cycles(1500000*15);

        itoa((adc[3]),result,10);

        acount =0;

        while(result[acount]!='\0')
        {
            while((IFG2 & UCA0TXIFG)==0);                  //Wait Unitl the UART transmitter is ready
            UCA0TXBUF = result[acount++] ;                   //Transmit the received data.
        }
        _delay_cycles(1500000*15);
    }
}

void uart_init(void){
    //clock//baudrate//modulation//sw rest//TXRX interrupt
    UCA0CTL1 |= UCSWRST;                     //Disable the UART state machine
    UCA0CTL1 |= UCSSEL_3;                    //Select SMCLK as the baud rate generator source
    UCA0BR1 =0;
    UCA0BR0 = 104;                           //Produce a 9,600 Baud UART rate
    UCA0MCTL = 0x02;                         //Chooa propriately from Table 15-4 in User Guide
    UCA0CTL1 &= ~UCSWRST;                    //Enable the UART state naching
    IE2 |= UCA0RXIE;                         //Enable the UART receiver Interrupt
}
void ConfigureAdc_light(){
    ADC10CTL1 = INCH_3 + ADC10DIV_0 + CONSEQ_3 + SHS_0; // read inputs from A0 to A3
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON;    // using 5V reference
    ADC10AE0 = BIT3 + BIT0;
    ADC10DTC1 = 4;
}
void port_init(){
    P1SEL |= BIT1 + BIT2;            // select non-GPIO  usage for Pins 1 and 2
    P1SEL2 |= BIT1 + BIT2;           // Select UART usage of Pins 1 and 2
}

void itoa(int value, char* str, int base) {  //Function to convert the signed int to an ASCII char array

    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* wstr=str;
    int sign;

    // Validate that base is between 2 and 35 (inlcusive)
    if (base<2 || base>35){
        *wstr='\0';
        return;
    }

    // Get magnitude and th value
    sign=value;
    if (sign < 0)
        value = -value;

    do // Perform interger-to-string conversion.
        *wstr++ = num[value%base]; //create the next number in converse by taking the modolus
    while(value/=base);  // stop when you get  a 0 for the quotient


    if(sign<0) //attch sign character, if needed
          *wstr++='-';
      *wstr='\0'; //Attach a null character at end of char array. The string is in reverse order at this point
      strreverse(str,wstr-1); // Reverse string

  }

void strreverse(char* begin, char* end)      // Function to reverse the order of the ASCII char array elements
{
    char aux;
    while(end>begin)
        aux=*end, *end--=*begin, *begin++=aux;
}
