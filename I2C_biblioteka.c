#include <avr/io.h>
#include <util/delay.h>
#include "I2C_biblioteka.h"

void wyslij_bajt( uint8_t adres, uint8_t bajt)		//wyslij bajt jako jako master
{
	TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);		//w³¹cz TWI, wyzeruj TWINT, wyslij START
	while (!(TWCR & (1<<TWINT)));					// czeka na flage TWINT
	TWDR = ( adres << 1 ) & ~1;						//ustawiamy TWDR na SLAVE_ADRES+WRITE
	TWCR = (1<<TWEN) | (1<<TWINT);					//wyzeruj TWINT
	while (!(TWCR & (1<<TWINT)));
	TWDR = bajt;									//ustawiamy TWDR na BAJT który chcemy wys³aæ
	TWCR = (1<<TWEN) | (1<<TWINT);					//wyzeruj TWINT
	while (!(TWCR & (1<<TWINT)));
	TWCR = (1<<TWEN) | (1<<TWSTO);					//zakoncz komunikacje
	_delay_us(150);
} 
	
uint8_t odbierz_bajt( uint8_t adres )				//odbierz bajt jako master
{
	uint8_t bajt;
	TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);		//w³¹cz TWI, wyzeruj TWINT, wyslij START
	while (!(TWCR & (1<<TWINT)));					// czeka na flage TWINT
	TWDR = ( adres << 1 ) | 1;						//ustawiamy TWDR na SLAVE_ADRES+READ
	TWCR = (1<<TWEN) | (1<<TWINT);					//wyzeruj TWINT
	while (!(TWCR & (1<<TWINT)))
	TWCR = (1<<TWEN) | (1<<TWINT);					//wyzeruj TWINT
	while (!(TWCR & (1<<TWINT)));
	bajt = TWDR;
	TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT);		//zakoncz komunikacje
	return bajt;
}
