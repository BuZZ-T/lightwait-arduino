# lightwait-arduino

This is the code for an [Arduino](https://arduino.cc) based hardware device with one RBG LED which works as a [trigger](https://github.com/BuZZ-T/lightwait#trigger) in the [lightwait-stack](https://github.com/BuZZ-T/lightwait).

It both supports common-anode and common-cathode LEDs. Adapt `IS_COMMON_ANODE` in the code accordingly (See the [Pinouts](#pinouts)).

## Features

* Listening to receive messages in the [lightwait-tp](https://github.com/BuZZ-T/lightwait#lightwait-tp) communication protocol via serial communication.
* Is currently tested on and supports the following [Arduino](https://arduino.cc)-based platforms
    * [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3)
## Build

You can use different tools to build the .ino file. For example:

* The arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)
* The Sublime Text plugin for Arduino development [stino](https://github.com/Robot-Will/Stino)
* ino 
    * `pip install ino`
    * `ino build`
* arduino-mk

## Flash

Flash it to Arduino using `avrdude` or tools which use `avrdude`, like the Arduino IDE or stino.

TODO:

## RBG Led

6 Pins with the following combinations

* 5+4- is red
* 1+ 3- is green
* 2+ 4- is blue
* 6+ 3- is blue 

## Test

### Serial connection in terminal

##### Set the correct configuration for the serial communication:
_(taken from here: https://playground.arduino.cc/Interfacing/LinuxTTY/)_

```
stty -F /dev/ttyACM0 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
```

* Be sure to use the correct tty interface (here: `/dev/ttyACM0`)
* The baud rate is set to 9600 in the sketch, so this rate is also used here, adapt it here accordingly if you changed it in the sketch at `Serial.begin(9600)`

##### Send commands via echo
You can now send commands to the Arduino by piping the echo output to the tty interface:
```
echo "255:0:0" > /dev/ttyACM0
```
(Again, use the correct tty interface here)

<a name="pinouts"></a>
## Pinouts

There are two example pinouts, both for the Arduino UNO.

They also may contain an optional 10µF capasitor between RESET and GND to prevent resetting the arduino on establishing a new serial connection.  
Although this capasitor is optional, it has two advantages:

* The serial connection is established faster, making the color change apply faster
* A reset of the arduino implies a reset of all output pins, leading the LED to flicker to off before the color change applies.

### 6-pin common cathode LED

| Color | Arduino Pins | LED Pins 
|-|-|-|
| - | GND | 3/4 |
| red | 9 | 5 |
| green | 10 | 1 |
| blue | 11 | 2/6 |

![Arduino Pinout](lw_arduino.png  "Arduino Pinout")

### 4-pin common anode LED

| Color | Arduino Pins | LED Pins 
|-|-|-|
| - | 5V | 2 |
| red | 9 | 1 |
| green | 10 | 3 |
| blue | 11 | 4 |

![Arduino with common anode LED](lw_arduino_common.png "Arduino with common anode LED")
