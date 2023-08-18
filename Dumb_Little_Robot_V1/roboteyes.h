#ifndef ROBOTEYES_H
#define ROBOTEYES_H

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;
extern const uint8_t image_data_DISTRESSED_EYESarray[] PROGMEM;
extern const uint8_t eye_lookleft [] PROGMEM;
extern const uint8_t eye_lookright [] PROGMEM; 
extern const uint8_t eye_nuetral [] PROGMEM;
extern const uint8_t eye_squint [] PROGMEM;
void look_nuetral(int t);
void blink_eyes(int t,int numberOfblinks);
void look_left(int t);
void look_right(int t);
void look_squint(int t);

#endif

 
