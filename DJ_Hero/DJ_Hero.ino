#include <Wire.h>

#define LEFT 0
#define CENTER 1
#define RIGHT 2

#define CLOCKWISE 0
#define STOPPED 1
#define COUNTERCLOCKWISE 2

#define BUTTON 4
#define LIGHT 5
#define FADER A0
#define KNOB A1

#define DEBOUNCE 50
#define KNOB_THRESHOLD 50
#define FADER_LEFT_UPPER_THRESHOLD 200
#define FADER_CENTER_LOWER_THRESHOLD 400
#define FADER_CENTER_UPPER_THRESHOLD 600
#define FADER_RIGHT_LOWER_THRESHOLD 800
#define TURNTABLE_THRESHOLD 3

byte a[10];

byte lightIntensity; 

boolean buttonWasPressed;

boolean redWasPressed;
boolean greenWasPressed;
boolean blueWasPressed;

boolean redIsPressed;
boolean greenIsPressed;
boolean blueIsPressed;

int lastKnobValue = 12345;

byte turntableState = STOPPED;
byte lastTurntableState = STOPPED;
int lastFaderState = CENTER;

//int lastTurntableState = STOPPED;

//byte 

void setup()
{
  Serial.begin(9600); // start serial for output
  Wire.begin(0x0D);

  pinMode(BUTTON, INPUT);
  pinMode(LIGHT, OUTPUT);

  lastFaderState = faderValueToState(analogRead(FADER), CENTER);
  lastKnobValue = analogRead(KNOB);
}

void loop()
{ 
  boolean _redWasPressed = redWasPressed;
  boolean _greenWasPressed = greenWasPressed;
  boolean _blueWasPressed = blueWasPressed;

  boolean bat = buttonWasPressed;
  boolean man = lightIntensity;

  int _lastKnobValue = lastKnobValue;

  byte _turntableState = turntableState;
  byte _lastTurntableState = lastTurntableState;

  readTurntable();

  redIsPressed = (a[19] & 2) > 0;
  greenIsPressed = (a[19] & 1) > 0;
  blueIsPressed = (a[19] & 4) > 0;

/*
  Serial.print(redIsPressed);
  Serial.print(' ');
  Serial.print(_redWasPressed);
  Serial.print('\t');
  Serial.print(greenIsPressed);
  Serial.print(' ');
  Serial.print(_greenWasPressed);
  Serial.print('\t');
  Serial.print(blueIsPressed); 
  Serial.print(' ');
  Serial.println(_blueWasPressed);
  delay(1);
*/

  if (redIsPressed && !_redWasPressed)
  { 
    Keyboard.print('x');
    //redWasPressed = true;
  }
  else if (!redIsPressed && _redWasPressed)
  { 
    Keyboard.print('X');
    // redWasPressed = false;
  }

  if (greenIsPressed && !_greenWasPressed) 
  {
    Keyboard.print('y');
    //greenWasPressed = true;
  }
  else if (!greenIsPressed && _greenWasPressed)
  {
    Keyboard.print('Y');
    //greenWasPressed = false;
  }

  if (blueIsPressed && !_blueWasPressed)
  { 
    Keyboard.print('z');
    //blueWasPressed = true;
  }
  else if (!blueIsPressed && _blueWasPressed)
  {
    Keyboard.print('Z');
    // blueWasPressed = false;
  }

  redWasPressed = redIsPressed;
  greenWasPressed = greenIsPressed;
  blueWasPressed = blueIsPressed;

  _lastTurntableState = _turntableState;

  byte rotationSpeed = a[20] > 128 ? 255 - a[20] : a[20];

  if (rotationSpeed > TURNTABLE_THRESHOLD)
  { 
    if (a[21] == 0) _turntableState = CLOCKWISE;
    else _turntableState = COUNTERCLOCKWISE;
  }
  else _turntableState = STOPPED;
  
  if (_turntableState != _lastTurntableState)
  {
    switch (_turntableState)
    {
      case STOPPED:
        Keyboard.print('0');
        break;
      
      case CLOCKWISE:
        Keyboard.print('1');
        break;
      
      case COUNTERCLOCKWISE:
        Keyboard.print('2');
        break;
      
      default:
    }
  }

  lastTurntableState = _lastTurntableState;
  turntableState = _turntableState;

  boolean button = digitalRead(BUTTON);
  //Serial.println(button);
  //delay(1);
  
  if (button && !bat)
  {
    Keyboard.print('s');
    man = 255;
    buttonWasPressed = true;
  }
  else if (!button && bat)
  {
    Keyboard.print('S');
    buttonWasPressed = false;
  }
  buttonWasPressed = button;

  analogWrite(LIGHT, man);
  if (man > 0) lightIntensity = man - 1;

  int knobValue = analogRead(KNOB);
  int wrappedDifference = wrappedDifferenceBetween(_lastKnobValue, knobValue);

  if (abs(wrappedDifference) > KNOB_THRESHOLD)
  {
    if (wrappedDifference < 1)
    {
      Keyboard.print('t'); // TODO: Check for CCW and CW
      _lastKnobValue = knobValue;
    }
    else
    {
      Keyboard.print('T');
      _lastKnobValue = knobValue;
    }

    lastKnobValue = _lastKnobValue;
  }

  byte faderState = faderValueToState(analogRead(FADER), lastFaderState);

  if (faderState != lastFaderState)
  {
    switch (faderState)
    {
    case LEFT:
      Keyboard.print('u');
      break;

    case CENTER:
      Keyboard.print('v');
      break;

    case RIGHT:
      Keyboard.print('w');
      break;
    }

    lastFaderState = faderState;
  }
}


int wrappedDifferenceBetween(int first, int second)
{ // TODO: There's some funny businesses wrapping. Fix it
  int difference = second - first;

  if (abs(difference) < 512) return difference;
  else 
  {
    if (difference < 0) // Went from 1023 to 0
        return difference + 1023;
    else // Went from 0 to 1023
    return difference - 1023;
  }
}

byte faderValueToState(int faderValue, int defaultValue)
{
  faderValue = 1023 - faderValue;

  if (faderValue < FADER_LEFT_UPPER_THRESHOLD) return LEFT;
  else if (faderValue > FADER_CENTER_LOWER_THRESHOLD &&
    faderValue < FADER_CENTER_UPPER_THRESHOLD) return CENTER;
  else if (faderValue > FADER_RIGHT_LOWER_THRESHOLD) return RIGHT;
  else return defaultValue;
}

void readTurntable()
{
  Wire.beginTransmission(0x0D);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(0x0D, 23);
  for (int i=0; i <= 22; i++) a[i]=Wire.read();
}







