/*============ Library Setup ===========*/

#include <String.h>

/*============ Data Setup ===========*/

/*-- LED Strip Information --*/
/*
 * Red Pin:   5
 * Blue Pin:  6
 * Green Pin: 3
 */

#define RED_PIN     5
#define BLUE_PIN    6
#define GREEN_PIN   3

int r = 0;
int g = 0;
int b = 0;

/*-- Sound Sensor Information --*/

#define SENSOR_PIN  A7

int sensorValue = 0;

/*-- Logic Information --*/

#define FADE_SPEED      20
#define AUDIO_EQUIPED   true

enum LED_MODE {
  OFF,
  SINGLE,
  AUDIO,
  FADE3,
  FADE7,
  FLASH3,
  FLASH7
};

LED_MODE currentMode;

/*-- Event Information --*/

/*  Event Name      |     Data
 *  -------------------------------------
 *  ON              |     light name
 *  OFF             |     light name
 *  ALL_ON          |     null
 *  ALL_OFF         |     null
 *  AUDIO           |     null
 *  SOLID           |     hex value
 *  FADE            |     3 or 7
 *  FLASH           |     3 or 7
 *  SPEED           |     up or down
 */

const String   EVENT_ON        = "ON";
const String   EVENT_OFF       = "OFF";
const String   EVENT_ALL_ON    = "ALL_ON";
const String   EVENT_ALL_OFF   = "ALL_OFF";
const String   EVENT_AUDIO     = "AUDIO";
const String   EVENT_SOLID     = "SOLID";
const String   EVENT_FADE      = "FADE";
const String   EVENT_FLASH     = "FLASH";
const String   EVENT_SPEED     = "SPEED";

/*============ Function Prototypes ===========*/

/* ----- writeLEDStrip
 * - Purpose: Helper function to write values to LED Strip
 * - In:      Red, Green and Blue int values
 * - Out:     None
 */
 
void writeLEDStrip(int red, int green, int blue);

/* ----- audioMode
 * - Purpose: Executes if the arduino is in audio mode
 * - In:      None
 * - Out:     None
 */

void audioMode();

/* ----- fade3
 * - Purpose: LEDs Fade between 3 colors
 * - In:      None
 * - Out:     None
 */

 void fade3();

 /* ----- fade7
 * - Purpose: LEDs Fade between 7 colors
 * - In:      None
 * - Out:     None
 */

 void fade7();

  /* ----- checkModeUpdate
 * - Purpose: Checks if the mode has been updated
 * - In:      None
 * - Out:     None
 */

 bool checkModeUpdate();

/*============ Initial Setup ===========*/

void setup() {

  // declare color pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // set initial values of colors
  r = 0;
  g = 0;
  b = 0;

  // set default mode
  currentMode = SINGLE;

  // set up serial
  Serial.begin(9600);
  Serial.setTimeout(500);
}

/*============ Main Loop ===========*/

void loop() {

  // execute the appropriate led state
  switch (currentMode) {
    case SINGLE:
      writeLEDStrip(r,g,b);
      checkModeUpdate();
      break;
    
    case AUDIO:
      if (AUDIO_EQUIPED) {
        audioMode();
        checkModeUpdate();
      }
      break;

    case FADE3:
      fade3();
      break;

    case FADE7:
      fade7();
      break;
  }
}

/*============ Function Declarations ===========*/
 
void writeLEDStrip(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void audioMode() {
  r = 255;
  g = 255;
  b = 255;
  
  // first read the level of the sound sensor
  sensorValue = analogRead(SENSOR_PIN);

  // calculation to pull down the levels of
  // GREEN and BLUE pins
  int tempG = g - (sensorValue * 50);
  int tempB = b - (sensorValue * 50);

  // Make sure that 
  if (tempG < 0) {
    tempG = 0;
  }

  if (tempB < 0) {
    tempB = 0;
  }

  // update the values on the LED Strip
  writeLEDStrip(r, tempG, tempB);

  // delay of 4 milli seconds to smooth the effect
  delay(4);
}

void fade3() {
  r = 0;
  g = 255;
  b = 0;

  // fade from red to green
  while (g > 0) {
    writeLEDStrip(r,g,b);
    delay(FADE_SPEED);

    g--;
    r++;
  }

  while (r > 0) {
    writeLEDStrip(r,g,b);
    delay(FADE_SPEED);

    r--;
    b++;
  }

  while (b > 0) {
    writeLEDStrip(r,g,b);
    delay(FADE_SPEED);

    b--;
    g++;
  }
}

void fade7() {
  r, g, b = 0;
}

bool checkModeUpdate() {
  /* 
   * AUDIO
   * FADE3
   * FADE7
   * SINGLE r g b
   */
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    Serial.println(input);
    Serial.println(input.equals("AUDIO"));
    Serial.println(input.equals("AUDIO\n"));
    Serial.println(input.equals("AUDIO\r"));

    if (input.equals("AUDIO\r") == true) {
      currentMode = AUDIO;
      Serial.println("Audio Mode Set");
    }
  }
  else {
    //Serial.println("Serial Not Available");
    return false;
  }
}

