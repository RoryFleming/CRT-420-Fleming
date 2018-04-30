#include <Servo.h> //need to use servo

#include <Adafruit_NeoPixel.h>
int bright = 75; //brightness of neopixel (0-255)
int numPixels = 3; //number of neopix used
int neoPixelPin = 7; //pin number that neopixel is plugged into
// Instatiate the NeoPixel from the ibrary
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);//include thee 6 ^ lines for OLED


#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// int sirv = 10; ->in setup:** myservo.attach(10); ** // attaches the servo on digital pin X on arduino

//digital pin 13 built in LED

int led = 3;
int piez = 6; //digital pin 6
int pot = 0; //but on pin A0
int button = 8;


const unsigned char umbrella [] PROGMEM = {
  // '*need width / 8*, 32x32px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x0e, 0x00, 0x00, 0xf8, 0x1f, 0x80, 0x01, 0xfc, 0x3f, 0xc0, 0x07, 0xfc, 0x3f, 0xe0,
  0x0f, 0xfe, 0x7f, 0xf8, 0x00, 0x0e, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x40, 0x00,
  0x00, 0x1e, 0x78, 0x00, 0x00, 0x7c, 0x3e, 0x00, 0x01, 0xfc, 0x3f, 0x80, 0x03, 0xfc, 0x3c, 0xc0,
  0x0f, 0xf8, 0x1b, 0xe0, 0x1f, 0xf8, 0x1d, 0xf8, 0x3f, 0xf0, 0x0f, 0xfc, 0x7f, 0xf0, 0x0f, 0xfe,
  0x7f, 0xf0, 0x0f, 0xfe, 0x1f, 0xe0, 0x07, 0xf8, 0x0f, 0xe0, 0x07, 0xf0, 0x07, 0xc0, 0x07, 0xe0,
  0x03, 0xc0, 0x03, 0xc0, 0x01, 0xc0, 0x03, 0x80, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  myservo.attach(10);  // attaches the servo on digital pin X on arduino

  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  pinMode(piez, OUTPUT);


  strip.begin();  // initialize the strip
  strip.show();   // make sure it is visible
  strip.clear();  // Initialize all pixels to 'off'
  strip.setBrightness(30);

}


void loop() {

  //for button & piezo
  int raw = digitalRead(button);
  if (raw == 1 ) {
   
    digitalWrite(piez, HIGH);
    delay (50);
    //Serial.println("sound on");
  } else {

    digitalWrite(piez, LOW);
    //Serial.println("sound off");
  }

//for potentiometer.servo
  pot = analogRead(0); //1023 on left, 0 on right
  Serial.println(pot);
  pot = map(pot, 0, 1023, 0, 180); // // scales pot to use it with the servo **(value between 0 and 180)**
  myservo.write(pot);
  // put your main code here, to run repeatedly:
  /*digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(100);
  */

  //for screen
  display.setCursor(5, 20);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
  display.print("HELLO DAVE");
  //display.drawBitmap(40,20, umbrella, 32, 32, WHITE); //format to draw bitmap
  //display.drawCircle(64, 38, 20, WHITE); //format to draw object: x, y, r, color
  display.display();

  //for neopix (w/ potentiometer rn)
  for ( int i = 0; i < numPixels; i++ ) {
    strip.setPixelColor(i, map(pot, 0, 1023, 0, 255), 0, 0 ); //what do I put for the first variable
    strip.show();
    delay(5);
  }
}
