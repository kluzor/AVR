#ifndef HD44780_I2C_H	
#define HD44780_I2C_H
 
void wyslij_do_lcd( int8_t bajt, uint8_t czy_dana );	//wyslij bajt do lcd
void lcd_wypisz( char tekst[] );						//wyslij tekst do lcd
void lcd_xy( char wiersz, char kolumna);				//ustawia kursor lcd w pozycji (wiersz,kolumna)
void lcd_init();										//inicjalizuje lcd
void lcd_wyczysc();										//czysci ekran lcd

#endif
