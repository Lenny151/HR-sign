// Params for width and height

#include <FastLED.h>

#define DATA_PIN 5
#define NUM_LEDS 146
#define BRIGHTNESS 50
#define LED_TYPE    SK6812
#define COLOR_ORDER GRB
CRGB leds[ NUM_LEDS ];

uint16_t paletteIndex = 0;

DEFINE_GRADIENT_PALETTE (hr_colours) {
  0,   255, 145, 0,   //orange
  51,  0,   255, 200,   //blue
  127, 120, 0,  255,   //purple
  204, 255, 0,   200,    //pink
  255, 255, 145, 0,   //orange
};

DEFINE_GRADIENT_PALETTE (hr_colours2) {
  0,   120, 0,  255,   //purple
  27,  120, 0,  255,   //purple
  37,  255, 0,   200,    //pink
  64,  255, 0,   200,    //pink
  91,  255, 0,   200,    //pink
  101, 255, 145, 0,   //orange
  128, 255, 145, 0,   //orange
  155, 255, 145, 0,   //orange
  165, 0,   255, 200,   //blue
  192, 0,   255, 200,   //blue
  219, 0,   255, 200,   //blue
  229, 120, 0,  255,   //purple
  255, 120, 0,  255,   //purple
};

DEFINE_GRADIENT_PALETTE (hr_colours3) {
  0,   255, 145, 0,   //orange 
  64,  120, 0,  255,   //purple
  128, 0,   255, 200,   //blue
  192, 120, 0,  255,   //purple
  255, 255, 145, 0,   //orange
};

uint16_t hardCodedColours[4][3] = {
  {255, 145, 0}, //orange
  {255, 0,   200}, //pink
  {0,   255, 200}, //blue
  {120, 0,  255}, //purple
};

CRGBPalette16 myPal = hr_colours;
CRGBPalette16 myPal2 = hr_colours2;
CRGBPalette16 myPal3 = hr_colours3;

void setup() {
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(57600);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { fade_palette, colour_wipe, fade_colour, fade_palette_short };

uint16_t gCurrentPatternNumber = 0; // Index number of which pattern is current

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest

  // do some periodic updates
  EVERY_N_SECONDS( 300 ) {
    nextPattern();  // change patterns periodically
    paletteIndex = 0;
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  
}

void fade_palette()
{
  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / 255, myPal, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(150) {
    paletteIndex--;
  }
}

void fade_palette_short()
{
  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / 20, myPal3, 255, LINEARBLEND); //may need to play with length "50" to get looking right
  
  EVERY_N_MILLISECONDS(6) {
    paletteIndex--;
  }
}

void colour_wipe()
{
 for(int j = 0; j < 4; j++){
  for(int i = 0; i < 146; i++){
    leds[i].setRGB(hardCodedColours[j][0], hardCodedColours[j][1], hardCodedColours[j][2]);
    FastLED.show();
    delay(50);
  }
 }
}

void fade_colour()
{
  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / 256, myPal2, 255, LINEARBLEND);
//  addGlitter(150);
  EVERY_N_MILLISECONDS(100) {
    paletteIndex++;
  }
}

//void addGlitter(fract8 chanceOfGlitter) {
//  if( random8() < chanceOfGlitter) {
//    leds[ random16(NUM_LEDS) ] += CRGB::White;}
//}
