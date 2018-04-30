/* int mode = 0;
int modeButton = 3;
numMode = X; //number of modes you have

void setup() {
pinMode (buttonMode, INPUT);
}

void checkModeButton() { //would need to make sure button is called: modeButton etc
  int raw = digitalRead(modeButton);
  if (raw == 1) {
    display.fillScreen(BLACK);
    mode = (((mode + 1) % numMode)); // makes mode loop 0->1->0 etc---called mod, remainder function
    delay(250);
     //for (i = 3; i < numMode; i++) this could also work
  }
}

void loop() {
  
checkModeButton();
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



*/
