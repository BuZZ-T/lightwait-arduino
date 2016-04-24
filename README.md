### Arduino

#### Build

You can use different tools to build the .ino file. For example:

* The arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)
* The Sublime Text plugin for Arduino development [stino](https://github.com/Robot-Will/Stino)
* ino 
    * `pip install ino`
    * `ino build`
* arduino-mk

#### Flash

Flash it to Arduino using `avrdude` or tools which use `avrdude`, like the Arduino IDE or stino.

### RBG Led

6 Pins with the following combinations

* 5+4- is red
* 1+ 3- is green
* 2+ 4- is blue
* 6+ 3- is blue 

### Pinout

![Arduino Pinout](lw_arduino.png  "Arduino Pinout")

### Arduino Pins

    RED_PIN 9 
    GREEN_PIN 10
    BLUE_PIN 11

| Arduino | LED |
|-|-|
|GND|3/4|
|9|5|
|10|1|
|11|2/6|
