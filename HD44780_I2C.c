#include <avr/io.h>
#include <util/delay.h>
#include "I2C_biblioteka.h"
#include "HD44780_I2C.h"

void wyslij_do_lcd( int8_t bajt, uint8_t czy_dana )	//wyslij bajt do lcd
{	
	wyslij_bajt( 0x27, ( bajt & 0xf0 ) | ( 0x08 | czy_dana ) ); 			//ustwiamy starsza czesc bajtu
	wyslij_bajt( 0x27, ( bajt & 0xf0 ) | ( 0x0c | czy_dana ) );				//ustwiamy linie E na stan wysoki
	wyslij_bajt( 0x27, ( bajt & 0xf0 ) | ( 0x08 | czy_dana ) );				//ustwiamy linie E na stan niski
	wyslij_bajt( 0x27, (( bajt << 4 ) & 0xf0 ) | ( 0x08 | czy_dana ) ); 	//ustwiamy mlodsza czesc bajtu
	wyslij_bajt( 0x27, (( bajt << 4 ) & 0xf0 ) | ( 0x0c | czy_dana ) ); 	//ustwiamy linie E na stan wysoki
	wyslij_bajt( 0x27, (( bajt << 4 ) & 0xf0 ) | ( 0x08 | czy_dana ) ); 	//ustwiamy linie E na stan niski
}

void lcd_wypisz( char tekst[] )						//wyslij tekst do lcd
{
	int dl_tekstu=strlen(tekst);
	for ( int i=0; i<dl_tekstu; i++)
	{
		wyslij_do_lcd( tekst[i], 1 );
	}
}

void lcd_xy( char wiersz, char kolumna)								//ustawia kursor lcd w pozycji (wiersz,kolumna)
{
	uint8_t komenda;
	if ( kolumna>=0 && kolumna<=15 && wiersz>=0 && wiersz <=1) 		//formowanie komendy
	komenda = 0x80 | ( wiersz << 6 ) | kolumna;
	wyslij_do_lcd( komenda , 0 );									//wysylanie komendy
}

void lcd_init()			//inicjalizuje lcd
{
	TWBR = 0x06;                                    //Ustawienie czêstotliwoœci LCD na 250KHz
	TWSR = (0 << TWPS0) | (0 << TWPS1);				//ustawienie preskalera dla TWI=1, zmniejszenie szybkoœci I2C
	_delay_ms(100);
	wyslij_bajt( 0x27, 0x28);
	wyslij_bajt( 0x27, 0x2c);
	wyslij_bajt( 0x27, 0x28);						//ustwwia tryb 4-bitowy
	wyslij_do_lcd( 0x08, 0 );						//wylacz lcd
	wyslij_do_lcd( 0x01, 0 ); 						//wyczysc lcd
	wyslij_do_lcd( 0x06, 0 );
	wyslij_do_lcd( 0x0c, 0 );						// wlacz lcd, wlacz kursor, wylacz mruganie
}

void lcd_wyczysc()		//czysci ekran lcd
{
						wyslij_do_lcd( 0x01, 0 );
}
