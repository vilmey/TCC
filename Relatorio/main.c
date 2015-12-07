

#include <msp430.h>
#include <stdint.h>
#include <stddef.h>

/* LED blinking frequency */
#define LED_BLINK_FREQ_HZ   400
/* Number of cycles to delay based on 1MHz MCLK */
#define LED_DELAY_CYCLES    (1000000 / (2*LED_BLINK_FREQ_HZ))

static int  _verify_cal_data(void);

int main(int argc, char *argv[])
{
    /* Hold the watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    if (_verify_cal_data() != 0) {
        /*  Calibration data is corrupted...hang */
        while(1);
    }
    
    /* Configure the clock module - MCLK = 1MHz */
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    /* Configure P1.0 as digital output */
    P1SEL &= ~0x01;
    P1DIR |= 0x01;

    /* Set P1.0 output high */
    P1OUT |= 0x01;

    /* Configure P1.3 to digital input */
    P1SEL &= ~0x08;
    P1DIR &= ~0x08;

    /* Pull-up required for rev 1.5 Launchpad */
    P1REN |= 0x08;
    P1OUT |= 0x08;

    /* Wait forever until the button is pressed */

    int b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,start,stop,count = 0;
    unsigned int tx = 0b0110011001; /* Message */

    /* Passing the bits */
    b1 = (P1OUT & (~BIT0)) + (tx >> 9) & BIT0;
    b2 = (P1OUT & (~BIT0)) + (tx >> 8) & BIT0;
    b3 = (P1OUT & (~BIT0)) + (tx >> 7) & BIT0;
    b4 = (P1OUT & (~BIT0)) + (tx >> 6) & BIT0;
    b5 = (P1OUT & (~BIT0)) + (tx >> 5) & BIT0;
    b6 = (P1OUT & (~BIT0)) + (tx >> 4) & BIT0;
    b7 = (P1OUT & (~BIT0)) + (tx >> 3) & BIT0;
    b8 = (P1OUT & (~BIT0)) + (tx >> 2) & BIT0;
    b9 = (P1OUT & (~BIT0)) + (tx >> 1) & BIT0;
    b10 = (P1OUT & (~BIT0)) + (tx >> 0) & BIT0;
    /* Start and stop bits */
    start = P1OUT&(~BIT0);
    stop = P1OUT|BIT0;
    

    while(1)
    {   
        /*Sending data */
        if((P1IN & 0x08) == 0)
        {
            P1OUT = start; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b1; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b2; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b3; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b4; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b5; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b6; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b7; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b8; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b9; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = b10; __delay_cycles(LED_DELAY_CYCLES);
            P1OUT = stop;
        }

        __delay_cycles(LED_DELAY_CYCLES);
    }
    
}

static int _verify_cal_data(void)
{
    size_t len = 62 / 2;
    uint16_t *data = (uint16_t *) 0x10c2;
    uint16_t crc = 0;

    while (len-- > 0) {
        crc ^= *(data++);
    }

    return (TLV_CHECKSUM + crc);
}
