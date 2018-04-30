
/*
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(3, OUTPUT); //led is here
}

void loop() {
  int lightsens = analogRead(2);

  Serial.println(lightsens);
  delay(100);
  int bright =map(lightsens, 620, 280, 0, 255); //maps lightsensor values 280(needs light) & 620(no light) to led values 0-255
   bright = constrain(bright, 0, 255); //doesn't allow bright to go below 0 or above 255
   analogWrite(3, bright);
  /*if (flex > 600) {
    digitalWrite(3, 0);
  } else if (flex < 599 && flex > 400) {
    digitalWrite(3, 135);
  } else {
    digitalWrite(3, 255);
  }*/
 

}

*/
