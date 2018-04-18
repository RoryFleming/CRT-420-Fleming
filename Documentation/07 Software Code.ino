
#include <SPI.h>
#include <Wire.h>
#include <SFE_BMP180.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

SFE_BMP180 pressure;
double baseline;
//#define ALTITUDE 210.15 //Mount Berry, GA

int mode = 0;
int timeMode = 0;
int modeButton = 3;
int timerButton = 4;
float timeStart = 0;
float timeEnd = 0;
boolean stopWatch = false; //if the stopwatch is counting
float tim = 0.0; //time start/started from watch to determine height
float timing = 0.0;


int led = 13;

const unsigned char mountain [] PROGMEM = {
  // 'il_340x270, 128x64px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xf8, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0xff, 0xf1, 0xf1, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x03, 0xff, 0xe1, 0xf1, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x0f, 0x87, 0xc1, 0xf0, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xc0, 0x1e, 0x0f, 0x81, 0xf0, 0x7f, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x7c, 0x1f, 0x00, 0xfc, 0x1f, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x7f, 0xf0, 0xf8, 0x3e, 0x00, 0x3f, 0x07, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x07, 0xfb, 0xe0, 0x7c, 0x00, 0x1f, 0x03, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x03, 0xff, 0xc1, 0xf0, 0x00, 0xff, 0x80, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x01, 0xff, 0x83, 0xc0, 0x00, 0x7f, 0xe0, 0x7e, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3f, 0x9f, 0x00, 0x01, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x70, 0x1f, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7e, 0x3e, 0x00, 0x00, 0xfc, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00,
  0x00, 0x00, 0x01, 0xf8, 0xfc, 0x00, 0x01, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00,
  0x00, 0x00, 0x03, 0xe1, 0xf8, 0x00, 0x03, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00,
  0x00, 0x00, 0x0f, 0xc7, 0xf0, 0x00, 0x0f, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00,
  0x00, 0x00, 0x3f, 0x0f, 0xc0, 0x00, 0x1f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
  0x00, 0x00, 0x7c, 0x3f, 0x80, 0x00, 0x7c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0,
  0x00, 0x01, 0xf0, 0xf9, 0x00, 0x01, 0xf8, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0,
  0x00, 0x07, 0xc3, 0xe2, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70,
  0x00, 0x0f, 0x87, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
  0x00, 0x3e, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xf8, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xe0, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0x80, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x38, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char easter [] PROGMEM = {
   // 'no smoking', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x0e, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x0f, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0xc3, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x03, 0xc1, 0xc0, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x07, 0x00, 0xc0, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x0e, 0x00, 0xc0, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x1c, 0x00, 0xc0, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x18, 0x00, 0xc0, 0x0f, 0xf7, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x18, 0x00, 0xc0, 0x1f, 0xe3, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x18, 0x01, 0xc0, 0x3f, 0xc3, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x18, 0x07, 0x80, 0x7f, 0x81, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x1c, 0x3f, 0x00, 0xff, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0x01, 0xfc, 0x3c, 0x01, 0xfe, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0x07, 0xfc, 0x18, 0x03, 0xfc, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7e, 0x0f, 0x00, 0x38, 0x07, 0xf8, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7e, 0x1c, 0x00, 0x30, 0x0f, 0xf0, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7e, 0x38, 0x00, 0x70, 0x1f, 0xe0, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7c, 0x30, 0xff, 0xe0, 0x3f, 0xc0, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7c, 0x33, 0xff, 0xc0, 0x7f, 0x80, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x77, 0x00, 0x00, 0xff, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7c, 0x66, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7c, 0x66, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7e, 0x66, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0xff, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0x81, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0xc3, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xe7, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0x7f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const static char aboutScreen[] PROGMEM  = {"This device provides your alt. above sea  level, temperature, &your relative height directly above  some-thing else.\n"};

const static char helpScreen[] PROGMEM  = {"On height screen,    -time- is time of    fall. -Last time- is for your own info,   it's  the time of thelast fall.\n"};


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  pressure.begin();
  pinMode(modeButton, INPUT);
  pinMode(timerButton, INPUT);
  pinMode (led, OUTPUT);
  Serial.begin(9600);

 /* if (pressure.begin()) //just to say yes or no
    Serial.println("BMP init y");
  else
  {
    Serial.println("BMP init n");
    while (1); // Pause forever.
  }*/

  // Get the baseline pressure:

  baseline = getPressure();

 /* Serial.print("base pres: ");
  Serial.print(baseline);
  Serial.println(" mb"); //can take all this out once it works */
}

void checkModeButton() {
  int raw = digitalRead(modeButton);
  if (raw == 1) {
    display.fillScreen(BLACK);
    mode = max(((mode + 1) % 7), 1); // makes mode loop 0->1->0 etc---called mod, remainder function
    delay(250);
  }
}

void loop() {
  //checkModeButton to see if button has been pushed, go to next mode, then % funt to go back to mode 1
  //Serial.print (timeStart);
  //Serial.print (abs(timeEnd - timeStart) / 1000);


  checkModeButton();
  if (digitalRead (timerButton) == HIGH && digitalRead (modeButton) == HIGH)
  { display.drawBitmap(0, 3, easter, 128, 64, WHITE);
    display.display();
  } else {

    switch (mode) {
      case 0: splash(); break;
      case 1: mainMenu(); break;
      case 2: elevation(); break;
      case 3: temp(); break;
      case 4: height(); break;
      case 5: about(); break;
      case 6: help(); break;
    }
  }
}

void splash() {
  display.setCursor(24, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print("HOW HIGH AM I?");
  display.drawBitmap(0, 3, mountain, 128, 64, WHITE);
  display.display();
  delay(3000);
  mode = 1;
  display.clearDisplay();
}

void mainMenu() {
  display.setCursor(35, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print("Main Menu");
  display.setCursor(0, 8);
  display.print("Options:");
  display.setCursor(0, 16);
  display.print("Elevation");
  display.setCursor(0, 26);
  display.print("Temperature");
  display.setCursor(0, 36);
  display.print("Relative Height");
  display.setCursor(0, 46);
  display.print("About");
  display.setCursor(0, 56);
  display.print("Help");
  display.display();
}


void elevation() {
  int alt = checkAltitude();

  display.setCursor(40, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print("Elevation");

  display.setTextSize(3);
  display.setCursor(40, 19);
  display.print(alt);

  display.setCursor(5, 54);
  display.setTextSize(1);
  display.print("feet above sea level");
  display.display();
}
void temp () {
  int temper = checkTemp();
  display.setCursor(33, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print("Temperature");

  display.setTextSize(4);
  display.setCursor(44, 17);
  display.print(temper);

  display.setCursor(14, 54);
  display.setTextSize(1);
  display.print("degrees farenheit");
  display.display();
}

void checkTimerButton() {
  int raw = digitalRead(timerButton);
  if (raw == 1) {
    display.fillScreen(BLACK);
    timeMode = (timeMode + 1) % 3; // makes mode loop 0->1->0 etc---called mod, remainder function
    Serial.println(timeMode);
    delay(500);
  }
}

void height() {
  int rHeight = checkHeight();
  display.setCursor(30, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
  display.print("Height");


 display.setTextSize(1);
  display.setCursor(0, 28);
  display.print("time"); // (millis() - timeStart)
  
  display.setCursor(0, 36);
  //display.print(tim);  //need to create new variable that is (millis - timeStart)/1000;
  display.print ((millis() - timeStart) / 1000);

  
  display.setCursor(102, 20);
  display.print("last");  //stationary number of last timed section
  display.setCursor(102, 28);
  display.print("time"); 
  display.setCursor(102, 36);
  display.print(tim);
  //that is the current time as it increases & shows it for the user;




  //float rtim = checkHeight();

  display.setTextSize(4);
  display.setCursor(30, 17);
  display.print(rHeight); //actual big height number 

  display.setCursor(38, 54);
  display.setTextSize(1);
  display.print("feet high");
  display.display();


}
/*
void countTime() {
  Serial.println("CountTime");
  display.setCursor(0, 0);
  display.print("time");
  display.setCursor(0, 8);
  display.print ((millis() - timeStart) / 1000);
}

void lastTime() {
  Serial.println("LastTime");
  display.setCursor(95, 0);
  display.print("ltime");
  display.setCursor(95, 8);
  display.print(tim);
}
void noTime() {
  Serial.println("NoTime");
}
*/
void about() { //bring this page and help page to EEPROM
  display.setCursor(37, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print("ABOUT");
  display.setCursor(0, 16);

  for (int k = 0; k < strlen_P(aboutScreen); k++)
  {
    char myChar =  pgm_read_byte_near(aboutScreen + k);
    display.print(myChar);
  }

  //display.print(aboutScreen);
  //display.print("This is what we are about, man");//How High am I? is a device that provides the user with his altititude above sea level, the temperature, and his relative height directly above something else.

  //display.drawBitmap(0, 3, mountain, 128, 64, WHITE);

  display.display();
  tim = 0; //needed to reset height when switching through all modes
}

void help() {
  display.setCursor(37, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print("HELP");
  display.setCursor(0, 16);
  //display.print(helpScreen);
  for (int k = 0; k < strlen_P(helpScreen); k++)
  {
    char myChar =  pgm_read_byte_near(helpScreen + k);
    display.print(myChar);
  }


  //display.print("We are here to help"); //If a problem occurs, restart the device and make sure you are using the correct button for your intended purpose.  The relative height function uses a physics equation based on the time it takes something to fall.  Start the timer right as you drop a solid object, stop the timer right when the object hits the ground.  Repeat and average heights for more accuracy.
  display.display();
}

double getPressure()
{
  char status;
  double T, P, p0, a;


  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
      }
    }
  }
}


float checkAltitude () { //

  double a, P;

  P = getPressure(); //P is in mb, "baseline" is aprox 990 mb, needs to be translated to feet
  a = pressure.altitude(P, baseline); //a is relative alt, compared to baseline

  /*
    Serial.print("rel alt: ");
    if (a >= 0.0) Serial.print(" ");
    Serial.print(a, 1);
    Serial.print(" met, ");
    if (a >= 0.0) Serial.print(" "); // add a space for positive numbers
    Serial.print(a * 3.28084, 0);
    Serial.println(" ft");*/
  //make sure here, we return absolute altitude
  display.fillScreen(BLACK);
  float eq = pow(P / 1013.25, 0.190284);

  return (1 - eq) * 145366.45; //need to be 640 ft

  delay(200);


}




int checkTemp() {
  char status;
  double T, P, p0, a;

  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    { /*
        Serial.print("temperature: ");
        Serial.print(T, 2);
        Serial.println(" deg C, ");
        Serial.print((9.0 / 5.0)*T + 32.0, 2);
        Serial.println(" deg F"); */
    }
  }
  return (1.8) * T + 32.0;
}

float checkHeight () {


  int raw = digitalRead(timerButton);
  if (raw == 1 && !stopWatch) {
    timeStart  = millis();
    stopWatch = true;
    //Serial.println ("timestart");
    //Serial.println(timeStart);
    //display.print ((millis() - timeStart)/1000);
    delay(200); //other debounce method??
  }
  else if (raw == 1 && stopWatch) {
    timeEnd = millis();
    stopWatch = false;
    tim = (abs((millis() - timeStart) / 1000));
    //Serial.println ("time:");
    //Serial.println (tim);

    delay(200);


  }
  // Serial.println("Time equals:");
  //Serial.println (abs(timeEnd - timeStart) / 1000);
  display.fillScreen(BLACK);
  float hite;
  hite = 4.9 * (tim * tim);
  //take hite1, (hite[1] +hite[2] +hite[i])/i
  //Serial.print ("this is height:");
  //Serial.println(hite);
  return hite;
}

