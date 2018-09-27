#include "pitches.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//* Haley DeLaura *//

//int lights[] = { 10, 7, 9, 5 };
int buttons[] = { 23, 25, 27, 29 };
int sequence[100];
int level = 1;
int notes[] = {NOTE_D4, NOTE_C4, NOTE_A4, NOTE_B4};

#define PIN1 11
#define PIN2 10
#define PIN3 9
#define PIN4 8

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(7, PIN1, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel ring2 = Adafruit_NeoPixel(16, PIN2, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel ring3 = Adafruit_NeoPixel(7, PIN3, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel ring4 = Adafruit_NeoPixel(16, PIN4, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  ring1.setBrightness(15);
  ring2.setBrightness(15);
  ring3.setBrightness(15);
  ring4.setBrightness(15);

  ring1.begin();
  ring1.show(); // Initialize all pixels to 'off'
  
  ring2.begin();
  ring2.show();
  
  ring3.begin();
  ring3.show();
  
  ring4.begin();
  ring4.show();

  
  
  // set pin modes to input/output
//  playMelody();
  for (int i = 0; i < 4; i++) {
    //pinMode(lights[i], OUTPUT);
    lightRing(ringForButton(i), ring1.Color(0, 0, 0), 0);
    pinMode(buttons[i], INPUT_PULLUP);
  }
  // seed the random generator
  randomSeed(analogRead(A0));
  seedSequence();


  // intro lights
  // display entire light sequence, one light at a time
//  colorWipe(ring1.Color(255, 0, 0), 50); // Red
//  colorWipe(ring2.Color(255, 0, 0), 50); // Red
//  colorWipe(ring3.Color(255, 0, 0), 50); // Red
//  colorWipe(ring4.Color(255, 0, 0), 50); // Red
//  
//  colorWipe(ring1.Color(0, 255, 0), 50); // Green
//  colorWipe(ring2.Color(0, 255, 0), 50); // Green
//  colorWipe(ring3.Color(0, 255, 0), 50); // Green
//  colorWipe(ring4.Color(0, 255, 0), 50); // Green
//  
//  colorWipe(ring1.Color(0, 0, 255), 50); // Blue
//  colorWipe(ring2.Color(0, 0, 255), 50); // Blue
//  colorWipe(ring3.Color(0, 0, 255), 50); // Blue
//  colorWipe(ring4.Color(0, 0, 255), 50); // Blue
//  
//  colorWipe(ring1.Color(0, 0, 0, 255), 50); // White RGBW
//  colorWipe(ring2.Color(0, 0, 0, 255), 50); // White RGBW
//  colorWipe(ring3.Color(0, 0, 0, 255), 50); // White RGBW
//  colorWipe(ring4.Color(0, 0, 0, 255), 50); // White RGBW
}

void seedSequence() {
  // randomize the sequence
  for (int i = 0; i < 100; i++) {
    sequence[i] = random(4);
  }
}

void loop() {
      
    for (int i = 0; i < level; i++) {
      int s = sequence[i];
      tone(4, notes[s], 200);
      lightRing(ringForButton(s), colorForButton(s), 200);
      noTone(4);
      lightRing(ringForButton(s), ring1.Color(0, 0, 0), 200);
    }
// wait for buttons to be pressed
  for (int i = 0; i < level; i++) {
    int b = waitForButton();
    if (b != sequence[i]) { 
      // game over
//      gameOver();
      level = 0;
      seedSequence();
    }
  }
  delay(500);
  level++;
}

int waitForButton() {
  // record the current time
  unsigned long now = millis();
  // while(true) {
  while (millis() - now < 2000) {
    // check all four buttons
    for (int i = 0; i < 4; i++) {
      // check if each button is pressed
      if (digitalRead(buttons[i]) == LOW) {
        tone(4, notes[i]);
        lightRing(ringForButton(i), colorForButton(i), 50);

        while (digitalRead(buttons[i]) == LOW);
        // delay for a bit
        delay(150);
        noTone(4);
        lightRing(ringForButton(i), ring1.Color(0, 0, 0), 50);
        // finally, return the button that was pressed
        return i;
      }
    }
  }
  return -1;
}



void playMelody() {
  int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
  int noteDurations[] = {
 4, 8, 8, 4, 4, 4, 4, 4
};
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 14; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(4, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(4);
  }
}

void gameOver() {
  for (int i = 0; i <4; i+=1) {
    rainbow(20);
    rainbowCycle(20);
    theaterChaseRainbow(50);
//    lights on
//    delay(450);
//    lights off
//    noTone(8);
//    delay(450);.
  }
  playMelody();
  delay(1000);
}

Adafruit_NeoPixel * ringForButton(int b) {
  if (b == 0) {
    return &ring1;
  } else if (b == 1) {
    return &ring2;
  } else if (b == 2) {
    return &ring3;
  } else if (b == 3) {
    return &ring4; 
  }
}

uint32_t colorForButton(int b) {
  if (b == 0) {
    return ring1.Color(255, 0, 0);
  } else if (b == 1) {
    return ring2.Color(255, 255, 0);
  } else if (b == 2) {
    return ring3.Color(0, 255, 0);
  } else if (b == 3) {
    return ring4.Color(0, 0, 255); 
  }
}

void lightRing(Adafruit_NeoPixel *ring, uint32_t color, uint8_t wait) {
  for(uint16_t i=0; i<ring->numPixels(); i++) {
    ring->setPixelColor(i, color);
  }
  ring->show();
  delay(wait);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<ring1.numPixels(); i++) {
    ring1.setPixelColor(i, c);
    ring1.show();
    delay(wait);
  }
  for(uint16_t i=0; i<ring2.numPixels(); i++) {
    ring2.setPixelColor(i, c);
    ring2.show();
    delay(wait);
  }
  for(uint16_t i=0; i<ring3.numPixels(); i++) {
    ring3.setPixelColor(i, c);
    ring3.show();
    delay(wait);
  }
  for(uint16_t i=0; i<ring4.numPixels(); i++) {
    ring4.setPixelColor(i, c);
    ring4.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<ring1.numPixels(); i++) {
      ring1.setPixelColor(i, Wheel((i+j) & 255));
    }
    ring1.show();
    delay(wait);
  }
  for(j=0; j<256; j++) {
    for(i=0; i<ring2.numPixels(); i++) {
      ring2.setPixelColor(i, Wheel((i+j) & 255));
    }
    ring2.show();
    delay(wait);
  }
  for(j=0; j<256; j++) {
    for(i=0; i<ring3.numPixels(); i++) {
      ring3.setPixelColor(i, Wheel((i+j) & 255));
    }
    ring3.show();
    delay(wait);
  }
  for(j=0; j<256; j++) {
    for(i=0; i<ring4.numPixels(); i++) {
      ring4.setPixelColor(i, Wheel((i+j) & 255));
    }
    ring4.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< ring1.numPixels(); i++) {
      ring1.setPixelColor(i, Wheel(((i * 256 / ring1.numPixels()) + j) & 255));
    }
    ring1.show();
    delay(wait);
  }
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< ring2.numPixels(); i++) {
      ring2.setPixelColor(i, Wheel(((i * 256 / ring2.numPixels()) + j) & 255));
    }
    ring2.show();
    delay(wait);
  }
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< ring3.numPixels(); i++) {
      ring3.setPixelColor(i, Wheel(((i * 256 / ring3.numPixels()) + j) & 255));
    }
    ring3.show();
    delay(wait);
  }
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< ring4.numPixels(); i++) {
      ring4.setPixelColor(i, Wheel(((i * 256 / ring4.numPixels()) + j) & 255));
    }
    ring4.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ring1.numPixels(); i=i+3) {
        ring1.setPixelColor(i+q, c);    //turn every third pixel on
      }
      ring1.show();

      delay(wait);

      for (uint16_t i=0; i < ring1.numPixels(); i=i+3) {
        ring1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ring2.numPixels(); i=i+3) {
        ring2.setPixelColor(i+q, c);    //turn every third pixel on
      }
      ring2.show();

      delay(wait);

      for (uint16_t i=0; i < ring2.numPixels(); i=i+3) {
        ring2.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ring3.numPixels(); i=i+3) {
        ring3.setPixelColor(i+q, c);    //turn every third pixel on
      }
      ring3.show();

      delay(wait);

      for (uint16_t i=0; i < ring3.numPixels(); i=i+3) {
        ring3.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ring4.numPixels(); i=i+3) {
        ring4.setPixelColor(i+q, c);    //turn every third pixel on
      }
      ring4.show();

      delay(wait);

      for (uint16_t i=0; i < ring4.numPixels(); i=i+3) {
        ring4.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ring1.numPixels(); i=i+3) {
        ring1.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      ring1.show();

      delay(wait);

      for (uint16_t i=0; i < ring1.numPixels(); i=i+3) {
        ring1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ring2.numPixels(); i=i+3) {
        ring2.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      ring2.show();

      delay(wait);

      for (uint16_t i=0; i < ring2.numPixels(); i=i+3) {
        ring2.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ring3.numPixels(); i=i+3) {
        ring3.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      ring3.show();

      delay(wait);

      for (uint16_t i=0; i < ring3.numPixels(); i=i+3) {
        ring3.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ring4.numPixels(); i=i+3) {
        ring4.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      ring4.show();

      delay(wait);

      for (uint16_t i=0; i < ring4.numPixels(); i=i+3) {
        ring4.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ring1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    return ring2.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    return ring3.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    return ring3.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ring1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    return ring2.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    return ring3.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    return ring4.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ring1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  return ring2.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  return ring3.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  return ring4.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



