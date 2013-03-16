/*
MakerBar Arduino/RasPi Drum Set
 Rock Band/Guitar Hero Hack
 
 By Zack Freedman
 With help from John Mayer (not the artist), Jordan DeLiso
 */

byte me;

/* Define NUMBER_OF_PADS, PAD_BINDINGS, 
   NUMBER_OF_ACCESSORIES, and ACCESSORY_BINDINGS 
   in a header and include it */

//#include "LeftBindings.h"
//#include "CenterBindings.h"
#include "RightBindings.h"

#define padHitThreshold 200
#define debounceTime 50 // Change if sounds are repeated

char padKeys[NUMBER_OF_PADS] = PAD_BINDINGS;
char accessoryKeys[NUMBER_OF_ACCESSORIES] = ACCESSORY_BINDINGS;

unsigned long padLastHit[NUMBER_OF_PADS];
unsigned long accessoryLastChanged[NUMBER_OF_ACCESSORIES];
boolean accessoryState[NUMBER_OF_ACCESSORIES];

unsigned long time;

void setup()
{
  delay(1000);
  
  for (int i = 0; i < NUMBER_OF_ACCESSORIES; i++)
  {
    pinMode(2 + NUMBER_OF_ACCESSORIES, INPUT);
  }
}


void loop()
{
  time = millis();

  for (int i = 0; i < NUMBER_OF_PADS; i++)
  {
    if (analogRead(A0 + i) > padHitThreshold &&
      time - padLastHit[i] > debounceTime)
    {
      Keyboard.print(padKeys[i]);
      padLastHit[i] = time;
    }
  }

  for (int i = 0; i < NUMBER_OF_ACCESSORIES; i++)
  {
    if (time - accessoryLastChanged[i] > debounceTime)
    {
      boolean accessoryRead = !digitalRead(2 + i);

      if (!accessoryState[i] && accessoryRead)
      {
        Keyboard.print(accessoryKeys[i]); // Lower case
        accessoryState[i] = true;
        accessoryLastChanged[i] = time;
      }
      else if (accessoryState[i] && !accessoryRead)
      {
        Keyboard.print((char)(accessoryKeys[i] - 32)); // Upper case
        accessoryState[i] = false;
        accessoryLastChanged[i] = time;
      }
    }
  }
}



