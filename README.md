# RGBling

## RGBling is a set of commandline utilities for controlling the WS2812B Addressable RGB LEDs for Linux and Windows with 11 lighting effects for 1 LED strip. It is a fork from https://gitlab.com/gabmus/ardhue_core but designed to work with commandline utilities instead. The Arduino waits upon recieving echo'ed commands from the PC.



## Install:
Clone the repository ```git clone https://github.com/jag75auau/RGBling.git```

You will need to install the ADAFRUIT_NEOPIXEL Library to compile the sketch

Using the Arduino IDE or editor and arduino-cli make the changes to the data pin and LED number:
```
#define DATA_PIN 5 // Set this to your data pin on your Arduino
#define NUM_LEDS 60 // Set this to the amount of LED's on you strip.
```
Upload the sketch to your Arduino

### Linux
RGBling needs to be exicutable ```chmod +x RGBling``` and the user needs to be in dialout group ```sudo usermod -a -G dialout $USER```

Copy RGBling into your ~/.local/bin or ~/.bin depending on your profile to access in anywhere. For Windows you can add the folder to you PATH enviroment variable.


## Usage:

### Linux
```
RGBling /dev/{arduino_port} (e.g. RGBling /dev/ttyUSB0)
```

### Windows
```
RGBling {COM Port} (e.g. RGBling com3)
```
