#define BLYNK_TEMPLATE_ID    "TMPL9iiBHsD5"
#define BLYNK_TEMPLATE_NAME  "Smart Irrigation System"

#define BLYNK_AUTH_TOKEN     "49fjlRtJkK-X2R-nXlJ4lyhhwm0ky9QE"

#define BLYNK_PRINT Serial


#include <WiFi.h>

#include <BlynkSimpleEsp32.h>

#include "DHT.h"

#include <FastLED.h>


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Harshal";

char pass[] = "87654321";


#define LED_PIN     21

#define BRIGHTNESS  255

#define LED_TYPE    WS2811

#define COLOR_ORDER GRB


const uint8_t kMatrixWidth  = 16;

const uint8_t kMatrixHeight = 16;

const bool    kMatrixSerpentineLayout = true;


#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)


// The leds

CRGB leds[kMatrixWidth * kMatrixHeight];

BlynkTimer timer;

bool ledStripOn = false;

// The 16 bit version of our coordinates

static uint16_t x;

static uint16_t y;

static uint16_t z;



uint16_t speed = 20; // speed is set dynamically once we've started up

uint16_t scale = 30; // scale is set dynamically once we've started up

uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];


CRGBPalette16 currentPalette( PartyColors_p );

uint8_t       colorLoop = 1;


#define DHTPIN 4

#define DHTTYPE DHT22


DHT dht(DHTPIN, DHTTYPE);

const int soilMoisturePin = 35; // Analog pin for the soil moisture sensor

const int waterLevelPin = 34;   // Analog pin for the water level sensor

const int ledPin = 2;



// Constants for soil moisture thresholds

const int SOIL_MOISTURE_LOW = 50;  // Adjusted to match the 0-100 range

const int SOIL_MOISTURE_HIGH = 50; // Adjusted to match the 0-100 range


const int pumpPin = 5;  // Change this to the actual pin connected to the DC pump on ESP32

int pumpState = LOW;    // Initialize pumpState to LOW (off)

int pumpMode = 0;       // 0 for automatic, 1 for manual


int waterLevel = 0;     // Water level indicator value

unsigned long previousMillis = 0;

const unsigned long interval = 1000; // Interval in milliseconds


float mappedSoilMoisture = 0; // Declare mappedSoilMoisture as a global variable


void setup() {

  Serial.begin(115200);

  digitalWrite(pumpPin,LOW);

  pinMode(ledPin, OUTPUT);

  pinMode(pumpPin, OUTPUT);

  dht.begin();

  Blynk.begin(auth, ssid, pass);

  Blynk.virtualWrite(V2, LOW);

  Blynk.virtualWrite(V3, "LED OFF");

  Blynk.virtualWrite(V6, LOW); // Initialize the mode button to automatic mode

  Blynk.virtualWrite(V5, LOW); // Initialize the pump button to OFF

  Blynk.virtualWrite(V8, LOW); // Initialize the new feature button to OFF

  delay(3000);

  LEDS.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds,80);

  LEDS.setBrightness(BRIGHTNESS);


  // Initialize our coordinates to some random values

  x = random16();

  y = random16();

  z = random16();

}


BLYNK_WRITE(V5) { // This function is called when the pump button state changes (Manual Control)

  if (pumpMode == 1) { // Check if the mode is manual

    pumpState = param.asInt(); // Read the state of the pump button (0 for OFF, 1 for ON)

    digitalWrite(pumpPin, pumpState); // Turn the pump on or off based on button state

  }

}


BLYNK_WRITE(V6) { // This function is called when the mode button state changes

  pumpMode = param.asInt(); // Read the state of the mode button (0 for automatic, 1 for manual)

}


BLYNK_WRITE(V8) {

  int value = param.asInt();

  if (value == 1) {

    // Turn on the LED strip and start the animation

    ledStripOn = true;

  } else {

    // Turn off the LED strip

    ledStripOn = false;

    // You can also add code to clear the LED strip here if needed

  }

}



// Fill the x/y array of 8-bit noise values using the inoise8 function.

void fillnoise8() {

  // If we're runing at a low "speed", some 8-bit artifacts become visible

  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.

  // The amount of data smoothing we're doing depends on "speed".

  uint8_t dataSmoothing = 0;

  if( speed < 50) {

    dataSmoothing = 200 - (speed * 4);

  }

  

  for(int i = 0; i < MAX_DIMENSION; i++) {

    int ioffset = scale * i;

    for(int j = 0; j < MAX_DIMENSION; j++) {

      int joffset = scale * j;

      

      uint8_t data = inoise8(x + ioffset,y + joffset,z);


      // The range of the inoise8 function is roughly 16-238.

      // These two operations expand those values out to roughly 0..255

      // You can comment them out if you want the raw noise data.

      data = qsub8(data,16);

      data = qadd8(data,scale8(data,39));


      if( dataSmoothing ) {

        uint8_t olddata = noise[i][j];

        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);

        data = newdata;

      }

      

      noise[i][j] = data;

    }

  }

  

  z += speed;

  

  // apply slow drift to X and Y, just for visual variation.

  x += speed / 8;

  y -= speed / 16;

}


void mapNoiseToLEDsUsingPalette()

{

  static uint8_t ihue=0;

  

  for(int i = 0; i < kMatrixWidth; i++) {

    for(int j = 0; j < kMatrixHeight; j++) {

      // We use the value at the (i,j) coordinate in the noise

      // array for our brightness, and the flipped value from (j,i)

      // for our pixel's index into the color palette.


      uint8_t index = noise[j][i];

      uint8_t bri =   noise[i][j];


      // if this palette is a 'loop', add a slowly-changing base value

      if( colorLoop) { 

        index += ihue;

      }


      // brighten up, as the color palette itself often contains the 

      // light/dark dynamic range desired

      if( bri > 127 ) {

        bri = 255;

      } else {

        bri = dim8_raw( bri * 2);

      }


      CRGB color = ColorFromPalette( currentPalette, index, bri);

      leds[XY(i,j)] = color;

    }

  }

  

  ihue+=1;

}


void loop() {

  Blynk.run();

  timer.run();

 

  updateLEDStrip();

  unsigned long currentMillis = millis();

  

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;


    // Read soil moisture or water level based on the flag

    if (pumpMode == 0) {

      int soilSensorValue = analogRead(soilMoisturePin);

      Serial.println(soilSensorValue);

      mappedSoilMoisture = map(soilSensorValue, 1023, 2750, 100, 0); // Update mappedSoilMoisture

      autoControlPump(mappedSoilMoisture); // Automatically control the pump based on soil moisture if in automatic mode

    }

    

    // Read water level

    waterLevel = analogRead(waterLevelPin);

    Serial.println(waterLevel);

    if (waterLevel < 450) {

      waterLevel = 0; // Set water level to 0 if below a threshold

    } else {

      waterLevel = map(waterLevel, 1200, 1600, 0, 100); // Map water level to 0-100

    }

    

    // Control LED

    controlLED(mappedSoilMoisture);


    // Read and send sensor data

    readAndSendSensorData();

    Blynk.virtualWrite(V7, waterLevel); // Send water level to Blynk

  }

}


void autoControlPump(float mappedSoilMoisture) {

  if (waterLevel < 30 || waterLevel < 20) {

    // Water level is too low, do not turn on the pump

    pumpState = LOW;

    digitalWrite(ledPin, LOW);

    Blynk.virtualWrite(V2, HIGH);

    Blynk.setProperty(V2, "color", "#D3435C");

    

  } else if (mappedSoilMoisture <= SOIL_MOISTURE_LOW) {

    // If soil moisture is below the threshold and water level is okay, turn the pump ON

    pumpState = HIGH;

  } else if (mappedSoilMoisture >= SOIL_MOISTURE_HIGH) {

    // If soil moisture is above the threshold, turn the pump OFF

    pumpState = LOW;

  }


  digitalWrite(pumpPin, pumpState); // Update the pump state

  if (pumpMode == 0) {

    Blynk.virtualWrite(V5, pumpState); // Update the pump button status on Blynk

  }

}


void controlLED(float mappedSoilMoisture) {

  if (mappedSoilMoisture <= SOIL_MOISTURE_LOW) {

    digitalWrite(ledPin, HIGH);

    Blynk.virtualWrite(V2, HIGH);

    Blynk.setProperty(V2, "color", "#00FF00");

    Blynk.virtualWrite(V3, "LED ON");

    Blynk.setProperty(V3, "color", "#00FF00");

  } else if (mappedSoilMoisture >= SOIL_MOISTURE_HIGH) {

    digitalWrite(ledPin, LOW);

    Blynk.virtualWrite(V2, HIGH);

    Blynk.setProperty(V2, "color", "#D3435C");

    Blynk.virtualWrite(V3, "LED OFF");

    Blynk.setProperty(V3, "color", "#D3435C");

  }

}


void readAndSendSensorData() {

  float humidity = dht.readHumidity();

  float temperature = dht.readTemperature();


  if (!isnan(humidity) && !isnan(temperature)) {

    Blynk.virtualWrite(V0, temperature);

    Blynk.virtualWrite(V1, humidity);

    Serial.println(temperature);

    Serial.println(humidity);

  } else {

    Serial.println(F("Failed to read from DHT sensor!"));

  }


  if (pumpMode == 0) {

    Blynk.virtualWrite(V4, mappedSoilMoisture); // Update mappedSoilMoisture on Blynk

  }

}


void updateLEDStrip() {

  if (ledStripOn) {

    // Your LED strip animation code here

    // This code will run when the LED strip is turned on

    // Make sure to update the leds array

    // For example, mapNoiseToLEDsUsingPalette();

    ChangePaletteAndSettingsPeriodically();

    fillnoise8(); // Call the noise generation function

    mapNoiseToLEDsUsingPalette(); // Map the noise data to LEDs

    LEDS.show(); // Show the LEDs

    // You may need to adjust the animation speed based on your preference

    delay(10); // Delay between frames (adjust as needed)

  } else {

    // Turn off the LED strip

    // You can also add code to clear the LED strip here if needed

    fill_solid(leds, NUM_LEDS, CRGB::Black);

    LEDS.show();

  }

}

// There are several different palettes of colors demonstrated here.

//

// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,

// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.

//

// Additionally, you can manually define your own color palettes, or you can write

// code that creates color palettes on the fly.


// 1 = 5 sec per palette

// 2 = 10 sec per palette

// etc

#define HOLD_PALETTES_X_TIMES_AS_LONG 1


void ChangePaletteAndSettingsPeriodically()

{

  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;

  static uint8_t lastSecond = 99;

  

  if( lastSecond != secondHand) {

    lastSecond = secondHand;

    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         speed = 20; scale = 30; colorLoop = 1; }

    if( secondHand ==  5)  { SetupPurpleAndGreenPalette();             speed = 10; scale = 50; colorLoop = 1; }

    if( secondHand == 10)  { SetupBlackAndWhiteStripedPalette();       speed = 20; scale = 30; colorLoop = 1; }

    if( secondHand == 15)  { currentPalette = ForestColors_p;          speed =  8; scale =120; colorLoop = 0; }

    if( secondHand == 20)  { currentPalette = CloudColors_p;           speed =  4; scale = 30; colorLoop = 0; }

    if( secondHand == 25)  { currentPalette = LavaColors_p;            speed =  8; scale = 50; colorLoop = 0; }

    if( secondHand == 30)  { currentPalette = OceanColors_p;           speed = 20; scale = 90; colorLoop = 0; }

    if( secondHand == 35)  { currentPalette = PartyColors_p;           speed = 20; scale = 30; colorLoop = 1; }

    if( secondHand == 40)  { SetupRandomPalette();                     speed = 20; scale = 20; colorLoop = 1; }

    if( secondHand == 45)  { SetupRandomPalette();                     speed = 50; scale = 50; colorLoop = 1; }

    if( secondHand == 50)  { SetupRandomPalette();                     speed = 90; scale = 90; colorLoop = 1; }

    if( secondHand == 55)  { currentPalette = RainbowStripeColors_p;   speed = 30; scale = 20; colorLoop = 1; }

  }

}


// This function generates a random palette that's a gradient

// between four different colors.  The first is a dim hue, the second is 

// a bright hue, the third is a bright pastel, and the last is 

// another bright hue.  This gives some visual bright/dark variation

// which is more interesting than just a gradient of different hues.

void SetupRandomPalette()

{

  currentPalette = CRGBPalette16( 

                      CHSV( random8(), 255, 32), 

                      CHSV( random8(), 255, 255), 

                      CHSV( random8(), 128, 255), 

                      CHSV( random8(), 255, 255)); 

}


// This function sets up a palette of black and white stripes,

// using code.  Since the palette is effectively an array of

// sixteen CRGB colors, the various fill_* functions can be used

// to set them up.

void SetupBlackAndWhiteStripedPalette()

{

  // 'black out' all 16 palette entries...

  fill_solid( currentPalette, 16, CRGB::Black);

  // and set every fourth one to white.

  currentPalette[0] = CRGB::White;

  currentPalette[4] = CRGB::White;

  currentPalette[8] = CRGB::White;

  currentPalette[12] = CRGB::White;


}


// This function sets up a palette of purple and green stripes.

void SetupPurpleAndGreenPalette()

{

  CRGB purple = CHSV( HUE_PURPLE, 255, 255);

  CRGB green  = CHSV( HUE_GREEN, 255, 255);

  CRGB black  = CRGB::Black;

  

  currentPalette = CRGBPalette16( 

    green,  green,  black,  black,

    purple, purple, black,  black,

    green,  green,  black,  black,

    purple, purple, black,  black );

}



//

// Mark's xy coordinate mapping code.  See the XYMatrix for more information on it.

//

uint16_t XY( uint8_t x, uint8_t y)

{

  uint16_t i;

  if( kMatrixSerpentineLayout == false) {

    i = (y * kMatrixWidth) + x;

  }

  if( kMatrixSerpentineLayout == true) {

    if( y & 0x01) {

      // Odd rows run backwards

      uint8_t reverseX = (kMatrixWidth - 1) - x;

      i = (y * kMatrixWidth) + reverseX;

    } else {

      // Even rows run forwards

      i = (y * kMatrixWidth) + x;

    }

  }

  return i;

}
