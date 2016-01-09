// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _esp8266_hydro_H_
#define _esp8266_hydro_H_
#include "Arduino.h"
//add your includes for the project esp8266_hydro here


//end of add your includes here


//add your function definitions for the project esp8266_hydro here
uint8_t crc8(const uint8_t *addr, uint8_t len);
void setup_mqtt();



//Do not add code below this line
#endif /* _esp8266_hydro_H_ */
