//    ____   ____ ____  _ _             
//   |  _ \ / ___| __ )| (_)_ __   __ _ 
//   | |_) | |  _|  _ \| | | '_ \ / _` |
//   |  _ <| |_| | |_) | | | | | | (_| |
//   |_| \_\\____|____/|_|_|_| |_|\__, |
//                                |___/ 
//
// RGBling is a fork from https://gitlab.com/gabmus/ardhue_core
// designed to work with command line utilites in both Linux and Windows

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define DATA_PIN 5 // Set this to your data pin on your Arduino
#define NUM_LEDS 60 // Set this to the amount of LED's on you strip. NB That some function will not work correctly or at all with <20 LED's
#define BAUDRATE 9600
#define ADA_BLACK strip.Color(0,0,0)
#define MY_BLACK makeMyColor(0,0,0)

// Constants for EEPROM
#define CONFIG_VERSION "ls2"
#define CONFIG_START 32

struct MyColor {
  byte r;
  byte g;
  byte b;
};

struct CmdStorage {
  char version[4];
  byte cmd_index;
  byte color_count;
  struct MyColor colors[3];
};

struct CmdStorage storage = {
  CONFIG_VERSION,
  3,
  3,
  {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}}
};

String cmd_list[] = {
  "wave",
  "digitalrgb",
  "spectrum",
  "static",
  "none",
  "supercar",
  "flux",
  "wipe",
  "fade",
  "running",
  "meteor"
};

#define NUM_FX 11

int get_fx_index(String fx) {
  for (int i = 0; i < NUM_FX; i++) {
    if (cmd_list[i] == fx) return i;
  }
  return 0;
}

String inputStr = "";
String validCmd = "wave";

bool loadConfig() {
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]) {

    for (unsigned int t = 0; t < sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
    return true;
  }
  else return false;
}

void saveConfig() {
  for (unsigned int t = 0; t < sizeof(storage); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
}

String zeropad(byte val) {
  String out = String(val);
  while (out.length() < 3) out = "0" + out;
  return out;
}

void initEEPROM() {
  if (!loadConfig()) saveConfig();
  inputStr = cmd_list[storage.cmd_index];
  for (byte i = 0; i < storage.color_count; i++) {
    inputStr += String(" ") + zeropad(storage.colors[i].r) + String(" ") + zeropad(storage.colors[i].g) + String(" ") + zeropad(storage.colors[i].b);
  }

  filterValidCmd();
}

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(BAUDRATE);
  strip.begin();
  strip.show();
  initEEPROM();
}

struct MyColor makeMyColor(byte r, byte g, byte b) {
  struct MyColor toret;
  toret.r = r;
  toret.g = g;
  toret.b = b;
  return toret;
}

uint32_t MyColor_toInt(struct MyColor c) {
  return strip.Color(c.r, c.g, c.b);
}

struct MyColor mcolor0;
struct MyColor mcolor1;
struct MyColor mcolor2;

void filterValidCmd() {
  // digitalrgb
  if (inputStr.substring(0, 10) == "digitalrgb") {
    validCmd = "digitalrgb";
  }
  // wave
  else if (inputStr.substring(0, 4) == "wave") {
    validCmd = "wave";
  }
  // spectrum
  else if (inputStr.substring(0, 8) == "spectrum") {
    validCmd = "spectrum";
  }
  // static 255 000 000 (red only)
  // static 255 000 000 000 255 000 (red and green)
  // static 255 000 000 000 255 000 000 000 255 (red, green and blue)
  else if (inputStr.substring(0, 6) == "static") { 
    validCmd = "static";
    //length 18 => 1 color
    mcolor0 = makeMyColor(inputStr.substring(7, 10).toInt(), inputStr.substring(11, 14).toInt(), inputStr.substring(15, 18).toInt());
    if (inputStr.length() >= 30) {
      mcolor1 = makeMyColor(inputStr.substring(19, 22).toInt(), inputStr.substring(23, 26).toInt(), inputStr.substring(27, 30).toInt());
    }
    else mcolor1 = mcolor0;
    if (inputStr.length() >= 42) {
      mcolor2 = makeMyColor(inputStr.substring(31, 34).toInt(), inputStr.substring(35, 38).toInt(), inputStr.substring(39, 42).toInt());
    }
    else mcolor2 = mcolor1;
  }
  else if (inputStr.substring(0, 4) == "none") {
    validCmd = "none";
  }
  // supercar 255 000 000 (red)
  else if (inputStr.substring(0, 8) == "supercar") { 
    validCmd = "supercar";
    mcolor0 = makeMyColor(inputStr.substring(9, 12).toInt(), inputStr.substring(13, 16).toInt(), inputStr.substring(17, 20).toInt());
  }
  // flux 255 000 000 (red)
  else if (inputStr.substring(0, 4) == "flux") { 
    validCmd = "flux";
    mcolor0 = makeMyColor(inputStr.substring(5, 8).toInt(), inputStr.substring(9, 12).toInt(), inputStr.substring(13, 16).toInt());
  }
  // wipe 255 000 000 (red)
  // wipe 255 000 000 000 255 000 (red and green)
  // wipe 255 000 000 000 255 000 000 000 255 (red, green and blue)
  else if (inputStr.substring(0, 4) == "wipe") {
    validCmd = "wipe";
    //length 16 => 1 color
    mcolor0 = makeMyColor(inputStr.substring(5, 8).toInt(), inputStr.substring(9, 12).toInt(), inputStr.substring(13, 16).toInt());
    if (inputStr.length() >= 28) {
      mcolor1 = makeMyColor(inputStr.substring(17, 20).toInt(), inputStr.substring(21, 24).toInt(), inputStr.substring(25, 28).toInt());
    }
    else mcolor1 = mcolor0;
    if (inputStr.length() >= 40) {
      mcolor2 = makeMyColor(inputStr.substring(29, 32).toInt(), inputStr.substring(33, 36).toInt(), inputStr.substring(37, 40).toInt());
    }
    else mcolor2 = mcolor1;
  }
  // fade 255 000 000 (red)
  // fade 255 000 000 000 255 000 (red and green)
  // fade 255 000 000 000 255 000 000 000 255 (red, green and blue)
  else if (inputStr.substring(0, 4) == "fade") {
    validCmd = "fade";
    //length 16 => 1 color
    mcolor0 = makeMyColor(inputStr.substring(5, 8).toInt(), inputStr.substring(9, 12).toInt(), inputStr.substring(13, 16).toInt());
    if (inputStr.length() >= 28) {
      mcolor1 = makeMyColor(inputStr.substring(17, 20).toInt(), inputStr.substring(21, 24).toInt(), inputStr.substring(25, 28).toInt());
    }
    else mcolor1 = mcolor0;
    if (inputStr.length() >= 40) {
      mcolor2 = makeMyColor(inputStr.substring(29, 32).toInt(), inputStr.substring(33, 36).toInt(), inputStr.substring(37, 40).toInt());
    }
    else mcolor2 = mcolor1;
  }
  // running 255 000 000 (red)
  else if (inputStr.substring(0, 7) == "running") {
    validCmd = "running";
    mcolor0 = makeMyColor(inputStr.substring(8, 11).toInt(), inputStr.substring(12, 15).toInt(), inputStr.substring(16, 19).toInt());
  }
  // meteor 255 000 000 (red)
  else if (inputStr.substring(0, 6) == "meteor") {
    validCmd = "meteor";
    mcolor0 = makeMyColor(inputStr.substring(7, 10).toInt(), inputStr.substring(11, 14).toInt(), inputStr.substring(15, 18).toInt());
  }
  else {
    Serial.println("ERROR: Invalid command   " + inputStr);
    return;
  }
  Serial.println(inputStr);
}

void getFromSerial() {
  if (Serial.available()) {
    inputStr = Serial.readString();
    filterValidCmd();
  }
}

String prev_cmd = "";
String prev_inputStr = inputStr;

int i = 0;
int j = 0;
uint8_t current_step = 0;
uint8_t max_step = 0;

void reset_variables() {
  i = 0;
  j = 0;
}

void save_status() {
  // save current status to EEPROM

  storage.colors[0] = mcolor0;
  storage.colors[1] = mcolor1;
  storage.colors[2] = mcolor2;
  storage.color_count = 3;

  storage.cmd_index = get_fx_index(validCmd);

  saveConfig();
}

void increment_step() {
  current_step++;
  if (current_step > max_step) current_step = 0;
}

void setPixel(int Pixel, struct MyColor color) {
  strip.setPixelColor(Pixel, MyColor_toInt(color));
}

void setAll(struct MyColor color) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, color);
  }
}

void setAllManual(byte r, byte g, byte b) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}

void setPixelManual(int Pixel, byte r, byte g, byte b) {
  strip.setPixelColor(Pixel, strip.Color(r, g, b));
}

void FadeInOut(struct MyColor color) {
  float r, g, b;

  for (int k = 0; k < 256; k = k + 1) {
    r = (k / 256.0) * color.r;
    g = (k / 256.0) * color.g;
    b = (k / 256.0) * color.b;
    setAllManual(r, g, b);
    strip.show();
  }
  delay(900);
  for (int k = 255; k >= 0; k = k - 2) {
    r = (k / 256.0) * color.r;
    g = (k / 256.0) * color.g;
    b = (k / 256.0) * color.b;
    setAllManual(r, g, b);
    strip.show();
  }
}

void Supercar(struct MyColor mcolor, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = 0; i < NUM_LEDS - EyeSize - 2; i++) {
    setAll(MY_BLACK);
    setPixelManual(i, mcolor.r / 10, mcolor.g / 10, mcolor.b / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, mcolor);
    }
    setPixelManual(i + EyeSize + 1, mcolor.r / 10, mcolor.g / 10, mcolor.b / 10);
    strip.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
  for (int i = NUM_LEDS - EyeSize - 2; i > 0; i--) {
    setAll(MY_BLACK);
    setPixelManual(i, mcolor.r / 10, mcolor.g / 10, mcolor.b / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, mcolor);
    }
    setPixelManual(i + EyeSize + 1, mcolor.r / 10, mcolor.g / 10, mcolor.b / 10);
    strip.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void RunningLights(struct MyColor mcolor, int WaveDelay) {
  int Position = 0;
  for (int j = 0; j < NUM_LEDS * 2; j++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < NUM_LEDS; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      setPixelManual(i, ((sin(i + Position) * 127 + 128) / 255)*mcolor.r,
                     ((sin(i + Position) * 127 + 128) / 255)*mcolor.g,
                     ((sin(i + Position) * 127 + 128) / 255)*mcolor.b);
    }
    strip.show();
    delay(WaveDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, r, g, b);
}

void meteorRain(struct MyColor mcolor, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
  setAll(MY_BLACK);

  for (int i = 0; i < NUM_LEDS + NUM_LEDS; i++) {


    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        fadeToBlack(j, meteorTrailDecay );
      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < NUM_LEDS) && (i - j >= 0) ) {
        setPixel(i - j, mcolor);
      }
    }

    strip.show();
    delay(SpeedDelay);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  strip.setPixelColor(i, c);
  strip.show();
  delay(wait);
  i++;
  if (i >= NUM_LEDS) {
    i = 0;
    increment_step();
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


void rainbow(uint8_t wait) {

  strip.setPixelColor(i, Wheel((i + j) & 255));

  i++;
  if (i >= NUM_LEDS) {
    i = 0;
    strip.show();
    delay(wait);
    j++;
    if (j >= 256) {
      j = 0;
    }
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {

  strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));

  i++;
  if (i >= NUM_LEDS) {
    i = 0;
    strip.show();
    delay(wait);
    j++;
    if (j >= 256 * 5) {
      j = 0;
    }
  }
}

void fluxfade(uint8_t size, uint8_t wait, struct MyColor color) {
  setAll(MY_BLACK);
  for (int k = 0; k < size; k++) {
    int pixel = i - k;
    struct MyColor c = color;
    if (k != 0) {
      c.r = c.r / pow(1.3, k);
      c.g = c.g / pow(1.3, k);
      c.b = c.b / pow(1.3, k);
    }
    if (!(pixel < 0 || pixel >= NUM_LEDS)) {
      setPixel(pixel, c);
    }
  }
  if (i - size >= NUM_LEDS) i = 0;
  else i++;
  strip.show();
  delay(wait);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Some example procedures showing how to display to the pixels:
  getFromSerial();

  if (prev_cmd != validCmd) {
    reset_variables();
    prev_cmd = validCmd;
  }

  if (prev_inputStr != inputStr) {
    save_status();
    prev_inputStr = inputStr;
  }

  if (validCmd == "digitalrgb") {
    max_step = 2;
    switch (current_step) {
      case 0:
        colorWipe(strip.Color(255, 0, 0), 50); // Red
        break;
      case 1:
        colorWipe(strip.Color(0, 255, 0), 50); // Green
        break;
      case 2:
        colorWipe(strip.Color(0, 0, 255), 50); // Blue
        break;
      default:
        colorWipe(strip.Color(0, 0, 255), 50); // Blue
    }
  }
  else if (validCmd == "wave") {
    rainbowCycle(5);
  }
  else if (validCmd == "spectrum") {
    rainbow(20);
  }
  else if (validCmd == "static") {
    int section_count = NUM_LEDS / 3;
    int current_section = 0;
    int led = 0;
    for (int i = 0; i < section_count; i++) {
      for (j = 0; j < section_count; j++) {
        switch (current_section) {
          case 0:
            setPixel(led, mcolor0);
            break;
          case 1:
            setPixel(led, mcolor1);
            break;
          case 2:
            setPixel(led, mcolor2);
            break;
          default:
            setPixel(led, mcolor2);
        }
        led++;
      }
      current_section++;
    }
    strip.show();
  }
  else if (validCmd == "flux") {
    fluxfade(20, 50, mcolor0);
  }
  else if (validCmd == "none") {
    setAll(MY_BLACK);
    strip.show();
  }
  else if (validCmd == "supercar") {
    Supercar(mcolor0, 4, 50, 10);
  }
  else if (validCmd == "wipe") {
    max_step = 5;
    switch (current_step) {
      case 0:
        colorWipe(MyColor_toInt(mcolor0), 50);
        break;
      case 1:
        colorWipe(ADA_BLACK, 50);
        break;
      case 2:
        colorWipe(MyColor_toInt(mcolor1), 50);
        break;
      case 3:
        colorWipe(ADA_BLACK, 50);
        break;
      case 4:
        if (mcolor2.r != mcolor1.r || mcolor2.g != mcolor1.g || mcolor2.b != mcolor1.b) {
          colorWipe(MyColor_toInt(mcolor2), 50);
        }
        else current_step = 0;
        break;
      case 5:
        if (mcolor2.r != mcolor1.r || mcolor2.g != mcolor1.g || mcolor2.b != mcolor1.b) {
          colorWipe(ADA_BLACK, 50);
        }
        else current_step = 0;
        break;
      default:
        colorWipe(strip.Color(0, 0, 255), 50); // Blue
    }
  }
  else if (validCmd == "fade") {
    FadeInOut(mcolor0);
    delay(100);
    FadeInOut(mcolor1);
    delay(100);
    if (mcolor2.r != mcolor1.r || mcolor2.g != mcolor1.g || mcolor2.b != mcolor1.b) {
      FadeInOut(mcolor2);
      delay(100);
    }
  }
  else if (validCmd == "running") {
    RunningLights(mcolor0, 50);
  }
  else if (validCmd == "meteor") {
    meteorRain(mcolor0, 10, 64, true, 30);
  }
}
