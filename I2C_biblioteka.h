#ifndef I2C_BIBLIOTEKA_H
#define I2C_BIBLIOTEKA_H
 
void wyslij_bajt( uint8_t adres, uint8_t bajt );		//wyslij bajt jako jako master
uint8_t odbierz_bajt( uint8_t adres );					//odbierz bajt jako master

#endif
