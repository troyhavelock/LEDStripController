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

#define FADE_SPEED    20
#define AUDIO_EQUIPED  true

enum LED_MODE {
  SINGLE,
  AUDIO,
  FADE3,
  FADE7
};

LED_MODE currentMode;

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
    String input = Serial.readString();
    Serial.print(input);
    if (input.equals("AUDIO")) {
      currentMode = AUDIO;
      Serial.print("Audio Mode Set");
    }
  }
  else {
    return false;
  }
}

