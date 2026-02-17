#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RoboEyes<Adafruit_SSD1306> roboEyes(display);

#define btn1 8
#define btn2 7

// ===== STATE MACHINE =====
enum State {
  IDLE_STATE,
  TEASE_PHASE1,
  TEASE_PHASE2,
  TEASE_PHASE3,
  LOOKAROUND_1,
  LOOKAROUND_2,
  LOOKAROUND_3,
  LOOKAROUND_4,
  LOOKAROUND_5
};

State currentState = IDLE_STATE;

unsigned long stateStartTime = 0;

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  enterIdleState();
}

void loop() {

  // ===== BUTTON DETECTION =====
  if (digitalRead(btn1) == LOW && currentState == IDLE_STATE) {
    changeState(TEASE_PHASE1);
  }
  if (digitalRead(btn2) == LOW && currentState == IDLE_STATE) {
    changeState(LOOKAROUND_1);
  }

  // ===== STATE HANDLER =====
  switch (currentState) {

    case IDLE_STATE:
      // idle hanya update saja
      roboEyes.update();
      break;

    case TEASE_PHASE1:
      if (millis() - stateStartTime < 1000) {
        roboEyes.update();
      } else {
        changeState(TEASE_PHASE2);
      }
      break;

    case TEASE_PHASE2:
      if (millis() - stateStartTime < 1000) {
        roboEyes.update();
      } else {
        changeState(TEASE_PHASE3);
      }
      break;

    case TEASE_PHASE3:
      if (millis() - stateStartTime < 2000) {
        roboEyes.update();
      } else {
        changeState(IDLE_STATE);
      }
      break;

    case LOOKAROUND_1:
      if (millis() - stateStartTime < 50) {
        roboEyes.update();
      } else {
        changeState(LOOKAROUND_2);
      }
      break;

    case LOOKAROUND_2:
      if (millis() - stateStartTime < 1000) {
        roboEyes.update();
      } else {
        changeState(LOOKAROUND_3);
      }
      break;

    case LOOKAROUND_3:
      if (millis() - stateStartTime < 1000) {
        roboEyes.update();
      } else {
        changeState(LOOKAROUND_5);
      }
      break;

    // case LOOKAROUND_4:
    //   if (millis() - stateStartTime < 800) {
    //     roboEyes.update();
    //   } else {
    //     changeState(LOOKAROUND_5);
    //   }
    //   break;

    case LOOKAROUND_5:
      if (millis() - stateStartTime < 1200) {
        roboEyes.update();
      } else {
        changeState(IDLE_STATE);
      }
      break;
  }
}

//
// ===== STATE TRANSITION FUNCTION =====
//

void changeState(State newState) {
  currentState = newState;
  stateStartTime = millis();

  switch (newState) {

    case IDLE_STATE:
      enterIdleState();
      break;

    case TEASE_PHASE1:
      roboEyes.setIdleMode(OFF, 2, 2);
      roboEyes.setAutoblinker(OFF, 3, 2);
      roboEyes.setVFlicker(ON, 10);
      // roboEyes.setMood(DEFAULT);
      roboEyes.setCuriosity(OFF);
      break;

    case TEASE_PHASE2:
      roboEyes.setVFlicker(OFF, 2);
      roboEyes.setPosition(DEFAULT);
      // roboEyes.setCuriosity(ON);
      // roboEyes.setMood(ANGRY);
      roboEyes.anim_confused();
      break;

    case TEASE_PHASE3:
      roboEyes.setPosition(E);
      roboEyes.setCuriosity(ON);
      roboEyes.setMood(ANGRY);
      break;
    
    case LOOKAROUND_1:
      roboEyes.setIdleMode(OFF, 2, 2);
      roboEyes.setAutoblinker(OFF, 3, 2);
      roboEyes.setWidth(25, 25); 
      roboEyes.setHeight(25, 25);
      // roboEyes.setCyclops(ON);
      roboEyes.setCuriosity(ON);
      // roboEyes.setPosition(E);
      break;
    
    case LOOKAROUND_2:
      roboEyes.setPosition(E);
      break;
    
    case LOOKAROUND_3:
      roboEyes.setPosition(W);
      break;
    
    // case LOOKAROUND_4:
    //   roboEyes.setPosition(W);
    //   break;
    
    case LOOKAROUND_5:
      roboEyes.setPosition(DEFAULT);
      // roboEyes.setCyclops(OFF);
      roboEyes.setMood(HAPPY);
      roboEyes.anim_laugh();
      break;
  }
}

//
// ===== IDLE CONFIG (dipanggil sekali saja) =====
//

void enterIdleState() {
  roboEyes.setIdleMode(ON, 2, 2);
  roboEyes.setAutoblinker(ON, 3, 2);
  roboEyes.setMood(DEFAULT);
  roboEyes.setCuriosity(OFF);
  roboEyes.setCyclops(OFF);
  roboEyes.setPosition(DEFAULT);
  roboEyes.setWidth(36, 36); // byte leftEye, byte rightEye
  roboEyes.setHeight(36, 36); // byte leftEye, byte rightEye
  roboEyes.setBorderradius(8, 8); // byte leftEye, byte rightEye
  roboEyes.setSpacebetween(10); // int space -> can also be negative
}
