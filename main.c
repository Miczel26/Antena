/*
 * 	Authors: Krzysztof Kucharczyk, Michał Stroka
 *	Date: 17.12.2014 r.
 *	Description: Program obsługujący antenę kierunkową
 */

/*
 * Biblioteki:
 * - avr/io.h - obsługuje piny i podstawowe funkcje
 * - avr/interrupt.h - obsługuje przerwania, na których miała działać bazowa wersja, aktualnie zbędne
 * - stdint.h - Standard Integer Types, czyli liczby całkowite o określonych szerokościach
 * 				W sumie nie wiem co to tutaj robi...
 */

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
     
void adc_inicjalizacja();
     
#define LED1 PB0		// Led testowy
#define LED2 PB1		// Led testowy
#define LED3 PB2		// Led testowy
#define STALA 512		// Testowa wartość progrowa przy testach ADC
     
int licznik;
int czujnik;

void ADC_inicjalizacja();
void PWM_inicjalizacja();
     
int main(void)
{
    ADC_inicjalizacja();
    PWM_inicjalizacja();
    
    DDRB = 0xff;		// Wyjścia
    DDRD = 0xff;		// Wyjścia
     
    while(1)
    {
        czujnik = ADC;					// Odczyt
        PORTB |= (1<<LED3);				
        OCR2B = 255;
        if( czujnik < STALA )			// Testowanie, czy odczyt działa poprawnie
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
 * ADC_inicjalizacja()
 * --------------------
 * Input: None
 * Output: None
 * --------------------
 *
 * Funkcja inicjalizująca konwerter analogowo-cyfrowy.
 * Znaczenie ustawionych bitów:
 * - ADEN - włączenie komparatora
 * - ADSC - włączenie konwersji (w naszym przypadku zapoczątkowanie ciągłej konwersji)
 * - ADATE - rozpoczynanie kowersji przy zboczu
 * - ADPS0, ADPS1, ADPS2 - preskaler ustawiony na wartość 128
 *
 * Konwerter analogowo-cyfrowy działa na zasadzie ciągłego pomiaru (free running).
 * Zostaje on pobierany w nieskończonej pętli w main(), następnie w zależności 
 * od wyniku zostaje podjęta stosowna akcja. Ze względu na niekorzystanie z przerwań
 * zostały one odłączone.
 */

void ADC_inicjalizacja()
{
    ADMUX=0x00;
    ADCSRA=0xe7;
}

/*
 * PWM_inicjalizacja()
 * --------------------
 * Input: None
 * Output: None
 * --------------------
 *
 * Funkcja inicjalizująca timery, by pracowały w trybie PWM.
 * Znaczenie ustawionych bitów:
 * - WGM00 oraz WGM01 - Fast PWM mode
 * - COM2B1 - non-inverting mode, czyszczenie OC2B przy porównaniu
 * - CS20 - brak preskalera
 *
 * Niestety funkcja nie została przetestowana, w zamyśle powinna spowodować ciągły ruch silnika.
 */

void PWM_inicjalizacja()
{
    TCCR2A|=(1<<WGM00 | 1<<WGM01 | 1<<COM2B1 );
    TCCR2B|=(1<<CS20);
}

/*
 * adc_inicjalizacja()
 * --------------------
 * Input: None
 * Output: None
 * --------------------
 *
 * Funkcja z początkowej fazy projektu, mająca na celu zainicjalizowanie
 * przetwornika ADC w taki sposób, by odczyt wyzwalał przerwanie.
 * Aktualnie nie używane/być może jest lepsze ?
 */

/*
void adc_inicjalizacja()
{
    ADCSRA|=(1<<ADEN | 1<<ADSC | 1<<ADATE | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0 ) ;
}
 */  
    
/*
 * ISR(ADC_vect)
 * --------------------
 * Warunek: wyzwalane przy pomiarze
 * --------------------
 *
 * Funkcja jest obsługą przerwania w wypadku wykonania pomiaru, również nie wykorzystana.
 * Zostawiam ze względu na możliwość późniejszego przejścia na przerwania.
 */

/*
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