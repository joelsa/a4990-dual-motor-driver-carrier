# Arduino library for the Pololu A4990 Dual Motor Driver Carrier

Version: 1.0.0 <br>
Release date: 2017-12-27 <br>

## Summary

This is a library for an
[Arduino-compatible controller](https://www.pololu.com/arduino) that
interfaces with the
[Pololu A4990 Dual Motor Driver Carrier](https://www.pololu.com/catalog/product/2137).
It makes it simple to drive two brushed, DC motors.

## Getting started

### Hardware

The
[Pololu A4990 Dual Motor Driver Carrier](https://www.pololu.com/catalog/product/2137).
can be purchased on Pololu's website. Before continuing, careful
reading of the product page is recommended.

#### Compatible Arduino boards

This shield should work with all Arduino boards and clones that behave
like a standard Arduino board. Please note that I have only tested this version
for an Arduino Nano (specificially with an ATMega328P).


### Software

You can manually install this library:

1. Download the
   [latest .zip archive from GitHub](https://github.com/joelsa/a4990-dual-motor-driver-carrier/archive/master.zip)
   and decompress it.
2. Rename the folder "master.zip" to "A4990DualMotorDriverCarrier.
3. Drag the "A4990DualMotorDriverCarrier" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Example program

An example sketch is available that shows how to use the library.  You
can access it from the Arduino IDE by opening the "File" menu,
selecting "Examples", and then selecting "A4990DualMotorDriverCarrierDemo".  If
you cannot find this examples, the library was probably installed
incorrectly and you should retry the installation instructions above.

### Demo

The demo sets the speed for Motor2 to 127 and then displays the encoder data.
Please be careful to check your connected pins and enter them in the correct manner
in the demo file (and then save it in your sketch folder).

## Documentation

- `void A4990DualMotorDriverCarrier(int in1, int in2, int in3, int in4)`
  Initialize controller. Note that all in pins must be PWM-capable.
- `void enableInterrupts(int encint1, int encdir1, int encint2, int encdir2)`
  Enable interrupts, encint1 and encint2 must be interrupt-capable.
  Note that you must not enter the GPIO Number (PDxx) here, but instead
  the interrupt number (0 for INT0, 1 for INT1 etc.).
- `void enableErrorHandling(int ef1, int ef2)` Set up the error handler.
  ef1 and ef2 need to be digital pins.
- `void enableSleepControl(int inh)` Turn on sleep control. inh needs
   to be a digital pin.
- `void setSleep(boolean sleep)` Either put the A4990 to sleep (sleep
  is true) or wake it up (sleep is false);
- `void setM1Speed(int speed)` <br> Set speed and direction for
  motor 1. Speed should be between -255 and 255. The motors brake at 0
  speed. Positive speeds correspond to motor current flowing from M1A
  to M1B. Negative speeds correspond to motor current flowing from M1B
  to M1A.
- `void setM2Speed(int speed)` <br> Set speed and direction for
  motor 2. Speed should be between -255 and 255. The motors brake at 0
  speed. Positive speeds correspond to motor current flowing from M2A
  to M2B. Negative speeds correspond to motor current flowing from M2B
  to M2A.
- `void setSpeeds(int m1Speed, int m2Speed)` <br> Set speed and
  direction for motor 1 and 2.
- `void flipM1(bool flip)` <br> Flip the direction meaning of the
  speed passed to the setSpeeds function for motor 1.  The default
  direction corresponds to flipM1(false) having been called.
- `void flipM2(bool flip)` <br> Flip the direction meaning of the
  speed passed to the setSpeeds function for motor 2.  The default
  direction corresponds to flipM2(false) having been called.
- `long readM1Rotation()` Get Encodercount for Motor1.
- `long readM2Rotation()` Get Encodercount for Motor2.
- `void resetM1Rotation()` Reset rotation for Motor1.
- `void resetM2Rotation()` Reset rotation for Motor2.
- `void resetRotations()` Reset rotations for both Motors.
- `bool getFault()` <br> Returns true if pin EF1 or EF2 is being driven low by
  one of the error flag outputs.
- `int determineFault()` Get the exact fault that is preventing the motor from
  turning, if it is an error happening on the controller-side.
- `void stopIfFault()` Set motor speeds to 0 if an error is occuring.

## Version history

* 1.0.0 (2017-12-27): Original release.
