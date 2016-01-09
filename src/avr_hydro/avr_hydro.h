// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _avr_hydro_H_
#define _avr_hydro_H_
#include "Arduino.h"
//add your includes for the project avr_hydro here


//end of add your includes here


//add your function definitions for the project avr_hydro here
String get_item_name_by_index(int index);
void requestEvent();
void receiveEvent(int howMany);
uint8_t crc8(const uint8_t *addr, uint8_t len);
bool check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc);
uint16_t crc16(const uint8_t* input, uint16_t len, uint16_t crc);



//Do not add code below this line
#endif /* _avr_hydro_H_ */
