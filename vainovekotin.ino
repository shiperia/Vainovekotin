
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <arduino-timer.h>

#define ledpin 14
#define NUM_LEDS 16
#define fade_time 15
#define check_time 1000

CRGB leds[NUM_LEDS] = {CRGB::Black};
CRGB curr_leds[NUM_LEDS] = {CRGB::Green};
CRGB target_leds[NUM_LEDS] = {CRGB::Black};
uint8_t fadeAmount[NUM_LEDS] = {0};
Timer<1> fadeTimer, checkTimer;

void newTarget(const unsigned int i, const CRGB target, unsigned int stepp=1) {
  if (target == target_leds[i]) return;
  fadeAmount[i] = stepp;
  leds[i] = curr_leds[i];
  target_leds[i] = target;
}

inline void setTargets(const CRGB target) {
  for (unsigned int al=0;al<NUM_LEDS;al++) {
    newTarget(al, target);
  }
}

static unsigned int c = 0, wi = 0,wee = 0;
unsigned int unrandom_walk[NUM_LEDS] = {0,3,4,7,12,6,2,15,1,8,9,10,5,11,13,14};
bool everyCheck(void*) {

  auto r_ind = random8(NUM_LEDS);
  wi = ++wi % 16;
  wee = ++wee % 12;
  auto ind = unrandom_walk[wi];
  auto r = random8(4);
  c = (c=c+r+r) % 256;

  unsigned int pwr=64;
  if (r==0) pwr = 0; else if (r==3) pwr=255;
  
  if (digitalRead(2) == LOW) {
    newTarget(ind, CRGB(pwr,0,0));
  }

  if (digitalRead(3) == LOW) {
    newTarget(ind, CRGB(0,pwr,0));
  }
  
  if (digitalRead(4) == LOW) {
    newTarget(ind, CRGB(0,0,pwr));
  }
    
  if (digitalRead(5) == LOW) {
    newTarget(ind, CHSV( c, 255, 255 ), 255);
  }
  
  if (digitalRead(6) == LOW) {
    if (r == 0) {
      newTarget(ind, CRGB::Black);
    } else if (r == 1) {
      newTarget(ind, CRGB::Red);
    } else if (r == 2) {
      newTarget(ind, CRGB::Green);
    } else if (r == 3) {
      newTarget(ind, CRGB::Blue);
    }
  }
  
  if (digitalRead(7) == LOW) {
    if (r == 0) {
      newTarget(ind, CRGB::Black);
    } else if (r == 1) {
      newTarget(ind, CRGB::Yellow);
    } else if (r == 2) {
      newTarget(ind, CRGB::Cyan);
    } else if (r == 3) {
      newTarget(ind, CRGB::Purple);
    }
  }
  
  if (digitalRead(8) == LOW) {
    unsigned int lt = unrandom_walk[ (wi+3) % 16 ];
    newTarget(lt,CRGB(random8(255),random8(255),random8(255)),254);
    newTarget(ind,CRGB::Black,254);
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
  }
  
  if (digitalRead(9) == LOW) {
    for (unsigned int h=0;h<4;h++) {
      newTarget(h, CRGB::Black, 254);
    }
    newTarget(4+((wee+4)%12), CHSV( c, 255, 255 ), 250);
    newTarget(4+wee,CRGB::Black,50);
    
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
  }
  return true;
}

bool everyFade(void*) {
  for(unsigned int l=0;l<NUM_LEDS;l++) {
    if ((leds[l] != target_leds[l]) && fadeAmount[l]==0) fadeAmount[l] = 1;
    if (fadeAmount[l] > 0) {
      curr_leds[l] = blend( leds[l], target_leds[l], ++fadeAmount[l] );
      if (fadeAmount[l] >= 255) {
        leds[l] = target_leds[l];
        curr_leds[l] = target_leds[l];
        fadeAmount[l] = 0;
      }
    }
  }
  FastLED.show();
  return true;
}

void setup() {
  for (unsigned int p=2;p<10;p++)
    pinMode(p, INPUT_PULLUP);

  pinMode( 10, OUTPUT ); //ledi1
  digitalWrite( 10, LOW );

  pinMode( 11, OUTPUT ); //ledi2
  digitalWrite( 11, LOW );

  FastLED.addLeds<NEOPIXEL, ledpin>(curr_leds, NUM_LEDS);
  FastLED.setBrightness(33);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 140);
  FastLED.clear();

  fadeTimer.every(fade_time, everyFade);
  checkTimer.every(check_time, everyCheck);
}

void loop() {
  while (true) {
    delay(1);
    fadeTimer.tick();
    checkTimer.tick();
  }
}
