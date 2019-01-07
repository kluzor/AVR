#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "HD44780_I2C.h"
#include "mkuart.h"

//pwm definicje

#define PWM_A    (1<<PB1)
#define PWM_B    (1<<PB2)
#define A1       (1<<PB0)
#define A2       (1<<PB4)
#define B1       (1<<PB3)
#define B2       (1<<PB5)

#define przod 1
#define tyl 2
#define stop 0

#define pwmx OCR1A
#define pwmy OCR1B

#define przod_1 PORTB|=A1;PORTB&=~A2
#define przod_2 PORTB|=B1;PORTB&=~B2
#define tyl_1 PORTB|=A2;PORTB&=~A1
#define tyl_2 PORTB|=B2;PORTB&=~B1

//czujniki definicje

char bufor_P[5];
char bufor_S[5];
char bufor_L[6];
double L=0;
double S=0;
double P=0;
int x=0;

#define TRIG_S	(1<<PC0)  //wyjœcie TRIG_S
#define ECHO_S	(1<<PD4)  //wejœcie ECHO_S
#define TRIG_L	(1<<PC1)  //wyjœcie TRIG_L
#define ECHO_L	(1<<PD3)  //wejœcie ECHO_L
#define TRIG_P	(1<<PC2)  //wyjœcie TRIG_P
#define ECHO_P	(1<<PD2)  //wejœcie ECHO_P

void silniki(uint8_t a, uint8_t b) //lewy, prawy
{
	switch(a)	//lewy
		{
		case 0: //stoj
			pwmx=0;
			break;
		case 1: //do przodu
			przod_2;
			break;
		case 2:  //do tylu
			tyl_2;
			break;
		}

	switch(b) // prawy
		{
		case 0: //stoj
				pwmy=0;
				break;
		case 1: //do przodu
				przod_1;
				break;
		case 2:  //do tylu
				tyl_1;
				break;
		}
}

int pomiar_S()
{
	unsigned int dist_S = 0;
	PORTC|=TRIG_S;				//wyœlij impuls TRIG
	_delay_us(10);
	PORTC&=~TRIG_S;
	while((PIND & ECHO_S)== 0);	//pêtla zliczaj¹ca czas odpowiedzi ECHO
	do
	{
		dist_S++;
		_delay_us(0.6);
	} while (PIND & ECHO_S);
	S=dist_S/58;
	if(S<=2)
		{
			return 2;
		}
	else
		{
			return S;
		}
}

int pomiar_L()
{
	unsigned int dist_L = 0;
	PORTC|=TRIG_L;				//wyœlij impuls TRIG
	_delay_us(10);
	PORTC&=~TRIG_L;
	while((PIND & ECHO_L)== 0);	//pêtla zliczaj¹ca czas odpowiedzi ECHO
	do
	{
		dist_L++;
		_delay_us(0.6);
	} while (PIND & ECHO_L);
	L=dist_L/58;
	if(L<=2)
		{
		return 2;
		}
	else
		{
		return L;
		}
}

int pomiar_P()
{
	unsigned int dist_P = 0;
	PORTC|=TRIG_P;				//wyœlij impuls TRIG
	_delay_us(10);
	PORTC&=~TRIG_P;
	while((PIND & ECHO_P)== 0);	//pêtla zliczaj¹ca czas odpowiedzi ECHO
	do
	{
		dist_P++;
		_delay_us(0.6);
	} while (PIND & ECHO_P);
	P=dist_P/58;
	if(P<=2)
		{
			return 2;
		}
	else
		{
			return P;
		}
}

//pomiary L,S oraz P dzielone s¹ przez 58 - sta³a korekcyjna do uzyskania "cm"

void LCD()
{
	lcd_xy(0,0);
	lcd_wypisz(" L    S    P    ");
	lcd_xy(1,0);
	sprintf(bufor_L,"%3.0d  ",pomiar_L());
	lcd_wypisz(bufor_L);
	lcd_xy(1,5);
	sprintf(bufor_S,"%3.0d  ",pomiar_S());
	lcd_wypisz(bufor_S);
	lcd_xy(1,10);
	sprintf(bufor_P,"%3.0d cm",pomiar_P());
	lcd_wypisz(bufor_P);
	_delay_us(600);
}

int main(void)
{
	lcd_init();
	USART_Init(_UBRR);

		// USTAWIANIE WYJŒÆ
		DDRB |= (PWM_A|PWM_B);    //wyjœcia pwm
		DDRB |= (A1|A2|B1|B2);    //silniki A i B

	 	// INICJALIZACJA PWM - TIMER1
	    TCCR1A |= (1<<WGM10);                      // Fast PWM 8bit
	    TCCR1B |= (1<<WGM12);
	    TCCR1A |= (1<<COM1A1)|(1<<COM1B1) ;        //Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM
	    TCCR1B |= (1<<CS10)|(1<<CS11);             // Preksaler = 64  fpwm = 976,5Hz
		sei();									   // odblokowanie globalne przerwañ

	// LCD + czujniki
	DDRC|=TRIG_S;
	DDRD&=~ECHO_S;
	DDRC|=TRIG_L;
	DDRD&=~ECHO_L;
	DDRC|=TRIG_P;
	DDRD&=~ECHO_P;

			//_delay_ms(15000);
			//uart_puts("Start");
			//_delay_ms(1000);
while(1)
	{
	_delay_ms(2000);
	uart_puts("Imie");
	_delay_ms(2000);
	uart_puts(":");
	_delay_ms(2000);
	uart_puts("Nazwisko");
	_delay_ms(2000);
	uart_puts("!");
	_delay_ms(2000);
	uart_puts("Adres");
	_delay_ms(2000);
	uart_puts("@");

/*
	 LCD();

//1
	 if((pomiar_L()>25) && (pomiar_S()>25) && (pomiar_P()>25))
	 	 	 	 { 				//jazda do przodu
	 		 		pwmx=220;
	 		 		pwmy=220;
	 		 		silniki(przod,przod);
	 		 	 }

//2
	 if((pomiar_L()>25) && (pomiar_S()>25) && (pomiar_P()<=25))
				{				//zwrot w lewo
					pwmx=220;
					pwmy=0;
					silniki(przod,przod);
				}
//3
	 if((pomiar_L()>25) && (pomiar_S()<=25) && (pomiar_P()>25))
				{				//zwrot w prawo
					pwmx=0;
					pwmy=220;
					silniki(przod,przod);
				}
//4
	 if((pomiar_L()>25) && (pomiar_S()<=25) && (pomiar_P()<=25))
				{				//zwrot w lewo
					pwmx=220;
					pwmy=0;
					silniki(przod,przod);
				}
//5
	 if((pomiar_L()<=25) && (pomiar_S()>25) && (pomiar_P()>25))
				{				//zwrot w prawo
					pwmx=0;
					pwmy=220;
					silniki(przod,przod);
				}
//6
	 if((pomiar_L()<=25) && (pomiar_S()>25) && (pomiar_P()<=25))
				{				//jazda do przodu
		 	 	 	 pwmx=220;
		 	 	 	 pwmy=220;
		 	 	 	 silniki(przod,tyl);
				}
//7
	 if((pomiar_L()<=25) && (pomiar_S()<=25) && (pomiar_P()>25))
				{				//zwrot w prawo
					pwmx=0;
					pwmy=220;
					silniki(przod,przod);
				}
//8
	 if((pomiar_L()<=25) && (pomiar_S()<=25) && (pomiar_P()<=25))
				{				//nawrót, w prawo
		 	 	 	pwmx=220;
		 	 	 	pwmy=220;
		 	 	 	silniki(tyl,przod);
				}
	*/}
}
