	

    #include <avr/io.h>
    #include <stdint.h>
    #include <avr/interrupt.h>
     
    void adc_inicjalizacja();
     
    #define LED1 PB0
    #define LED2 PB1
    #define LED3 PB2
    #define STALA 512
     
    int licznik;
    int czujnik;
     
    void PWM_inicjalizacja();
     
    int main(void)
    {
            //adc_inicjalizacja();
            //sei();
            PWM_inicjalizacja();
            DDRB = 0xff;
            DDRD = 0xff;
     
            //ADMUX = 0b00100001;
     
            /*
             * Pobieranie sygnału z potencjometru.
             * --------------------------------------
             * ADMUX ustawione na pobieranie napięcia referencyjnego z pinu AREF
             * oraz odczyt sygnału z portu PC0 (pin 23).
             *
             */
            ADMUX=0x00;
            ADCSRA=0xe7;
     
     
            while(1)
            {
                    czujnik = ADC;
                    PORTB |= (1<<LED3);
                    OCR2B = 255;
                    if( czujnik < STALA )
                    {
                            PORTB |= 1<<LED1;
                            PORTB &=~ (1<<LED2);
                    }
                    if( czujnik > STALA )
                    {
                            PORTB |= 1<<LED2;
                            PORTB &=~ (1<<LED1);
                    }
                    PORTD=(1<<PD2);
     
                    PORTB &=~ (1<<LED3);
            }
     
            return 0;
    }
     
    /*
    void adc_inicjalizacja()
    {
            ADCSRA|=(1<<ADEN | 1<<ADSC | 1<<ADATE | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0 ) ;
    }
     
     
    ISR(ADC_vect)
    {
            licznik++;
            if(licznik > 50)
            {
                    czujnik = ADC;
                    licznik = 0;
            }
     
    }
    */
     
    void PWM_inicjalizacja()
    {
            TCCR2A|=(1<<WGM00 | 1<<WGM01 | 1<<COM2B1 );
            TCCR2B|=(1<<CS20);
    }

