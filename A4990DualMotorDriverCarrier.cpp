#include <4990DualMotorDriverCarrier.h>
boolean A4990DualMotorDriverCarrier::flipM1 = false;
boolean A4990DualMotorDriverCarrier::flipM2 = false;

boolean A4990DualMotorDriverCarrier::changeFlagM1 = false;
boolean A4990DualMotorDriverCarrier::changeFlagM2 = false;

unsigned char A4990DualMotorDriverCarrier::M1IN1;
unsigned char A4990DualMotorDriverCarrier::M1IN2;
unsigned char A4990DualMotorDriverCarrier::M2IN3;
unsigned char A4990DualMotorDriverCarrier::M2IN4;
unsigned char A4990DualMotorDriverCarrier::ENCINT1;
unsigned char A4990DualMotorDriverCarrier::ENCDIR1;
unsigned char A4990DualMotorDriverCarrier::ENCINT2;
unsigned char A4990DualMotorDriverCarrier::ENCDIR2;
unsigned char A4990DualMotorDriverCarrier::EF1;
unsigned char A4990DualMotorDriverCarrier::EF2;
unsigned char A4990DualMotorDriverCarrier::INH;

unsigned int A4990DualMotorDriverCarrier::errorType = A4990DualMotorDriverCarrier::NOFAULT;
boolean A4990DualMotorDriverCarrier::error = false;
boolean A4990DualMotorDriverCarrier::initialized = false;
boolean A4990DualMotorDriverCarrier::interruptsEnabled = false;
boolean A4990DualMotorDriverCarrier::interruptsInitialized = false;
boolean A4990DualMotorDriverCarrier::errorsEnabled = false;
boolean A4990DualMotorDriverCarrier::errorsInitialized = false;
boolean A4990DualMotorDriverCarrier::sleepControlEnabled = false;
boolean A4990DualMotorDriverCarrier::sleepControlInitialized = false;
volatile long A4990DualMotorDriverCarrier::encoderM1Count = 0;
volatile long A4990DualMotorDriverCarrier::encoderM2Count = 0;

const int A4990DualMotorDriverCarrier::MAXSPEED = 255;

A4990DualMotorDriverCarrier::A4990DualMotorDriverCarrier(int in1, int in2, int in3, int in4) {
  A4990DualMotorDriverCarrier::M1IN1 = in1;
  A4990DualMotorDriverCarrier::M1IN2 = in2;
  A4990DualMotorDriverCarrier::M2IN3 = in3;
  A4990DualMotorDriverCarrier::M2IN4 = in4;
  init(); // initialize if necessary
  initialized = true;
}

void A4990DualMotorDriverCarrier::enableInterrupts(int encint1, int encdir1, int encint2, int encdir2) {
  A4990DualMotorDriverCarrier::ENCINT1 = encint1;
  A4990DualMotorDriverCarrier::ENCDIR1 = encdir1;
  A4990DualMotorDriverCarrier::ENCINT2 = encint2;
  A4990DualMotorDriverCarrier::ENCDIR2 = encdir2;
  interruptsEnabled = true;
  init(); // initialize if necessary
}

void A4990DualMotorDriverCarrier::enableErrorHandling(int ef1, int ef2) {
  A4990DualMotorDriverCarrier::EF1 = ef1;
  A4990DualMotorDriverCarrier::EF2 = ef2;
  errorsEnabled = true;
  init(); // initialize if necessary
}

void A4990DualMotorDriverCarrier::enableSleepControl(int inh) {
  A4990DualMotorDriverCarrier::INH = inh;
  sleepControlEnabled = true;
  init(); // initialize if necessary
}

void A4990DualMotorDriverCarrier::initPinsAndTimer()
{
  // Initialize the pin states used by the motor driver carrier
  // digitalWrite is called before and after setting pinMode.
  // It called before pinMode to handle the case where the board
  // is using an ATmega AVR to avoid ever driving the pin high, 
  // even for a short time.
  // 
  // It is called after pinMode to handle the case where the board
  // is based on the Atmel SAM3X8E ARM Cortex-M3 CPU, like the Arduino
  // Due. This is necessary because when pinMode is called for the Due
  // it sets the output to high (or 3.3V) regardless of previous
  // digitalWrite calls.

  digitalWrite(M1IN1, LOW);
  pinMode(M1IN1, OUTPUT);
  digitalWrite(M1IN1, LOW);

  digitalWrite(M1IN2, LOW);
  pinMode(M1IN2, OUTPUT);
  digitalWrite(M1IN2, LOW);

  digitalWrite(M2IN3, LOW);
  pinMode(M2IN3, OUTPUT);
  digitalWrite(M2IN3, LOW);

  digitalWrite(M2IN4, LOW);
  pinMode(M2IN4, OUTPUT);
  digitalWrite(M2IN4, LOW);
}


void A4990DualMotorDriverCarrier::encoderIntM1() {
  if (digitalRead(ENCDIR1) == HIGH)
    A4990DualMotorDriverCarrier::encoderM1Count--;
  else
    A4990DualMotorDriverCarrier::encoderM1Count++;
  A4990DualMotorDriverCarrier::changeFlagM1 = true;
}

void A4990DualMotorDriverCarrier::encoderIntM2() {
  if (digitalRead(ENCDIR1) == HIGH)
    A4990DualMotorDriverCarrier::encoderM2Count--;
  else
    A4990DualMotorDriverCarrier::encoderM2Count++;
  A4990DualMotorDriverCarrier::changeFlagM2 = true;
}

void A4990DualMotorDriverCarrier::initInterruptPins()
{
  // Initialize the pin states used by the encoder
  // and attach interrupts

  pinMode(ENCINT1, INPUT);  
  pinMode(ENCDIR1, INPUT);  
  attachInterrupt(ENCINT1, encoderIntM1, RISING); 
  pinMode(ENCINT2, INPUT);  
  pinMode(ENCDIR1, INPUT); 
  attachInterrupt(ENCINT2, encoderIntM2, RISING); 
}

void A4990DualMotorDriverCarrier::initErrorPins()
{
  // Initialize the pin states used by the error handler

  pinMode(EF1, INPUT_PULLUP);  
  pinMode(EF2, INPUT_PULLUP);  
}

void A4990DualMotorDriverCarrier::initSleepPins()
{
  // Initialize the pin state used by the inhibit pin

  pinMode(INH, OUTPUT);
  digitalWrite(INH, HIGH);
}

void A4990DualMotorDriverCarrier::setSleep(boolean sleep)
{
  // Put to sleep or wake up
  if (sleepControlEnabled == true) {
    digitalWrite(INH, sleep);
  }
}

// speed should be a number between -255 and 255
void A4990DualMotorDriverCarrier::setM1Speed(int turnSpeed)
{
  init(); // initialize if necessary
    
  boolean reverse = 0;
  
  if (turnSpeed < 0)
  {
    turnSpeed = -turnSpeed; // make speed a positive quantity
    reverse = 1;    // preserve the direction
  }
  if (turnSpeed > MAXSPEED)  // max PWM duty cycle
    turnSpeed = MAXSPEED;
    
  if (reverse ^ flipM2) {
    // Clockwise
    digitalWrite(M1IN2, LOW);  
    analogWrite(M1IN1, MAXSPEED-turnSpeed); // use analogWrite from 0 to 255
  } else {
    // Counterclockwise
    digitalWrite(M1IN1, HIGH);
    analogWrite(M1IN2, turnSpeed); // use analogWrite from 0 to 255
  }
}

// speed should be a number between -400 and 400
void A4990DualMotorDriverCarrier::setM2Speed(int turnSpeed)
{
  init(); // initialize if necessary
    
  boolean reverse = 0;
  
  if (turnSpeed < 0)
  {
    turnSpeed = -turnSpeed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (turnSpeed > MAXSPEED)  // max PWM duty cycle
    turnSpeed = MAXSPEED;

  if (reverse ^ flipM2) {
    // Clockwise
    digitalWrite(M2IN4, LOW);  
    analogWrite(M2IN3, MAXSPEED-turnSpeed); // use analogWrite from 0 to 255
  } else {
    // Counterclockwise
    digitalWrite(M2IN3, HIGH);
    analogWrite(M2IN4, turnSpeed); // use analogWrite from 0 to 255
  }
}

// set speed for both motors
// speed should be a number between -255 and 255
void A4990DualMotorDriverCarrier::setSpeeds(int m1Speed, int m2Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}

void A4990DualMotorDriverCarrier::setFlipM1(boolean flip)
{
  A4990DualMotorDriverCarrier::flipM1 = flip;
}

void A4990DualMotorDriverCarrier::setFlipM2(boolean flip)
{
  A4990DualMotorDriverCarrier::flipM2 = flip;
}

long A4990DualMotorDriverCarrier::readM1Rotation(){
  if (interruptsEnabled == true) {
    return encoderM1Count;    
  }
  return 0;
}

long A4990DualMotorDriverCarrier::readM2Rotation(){
  if (interruptsEnabled == true) {
    return encoderM2Count;    
  }
  return 0;
}

void A4990DualMotorDriverCarrier::resetM1Rotation() {
  A4990DualMotorDriverCarrier::encoderM1Count = 0;
}

void A4990DualMotorDriverCarrier::resetM2Rotation() {
  A4990DualMotorDriverCarrier::encoderM1Count = 0;
}

void A4990DualMotorDriverCarrier::resetRotations() {
  resetM1Rotation();
  resetM2Rotation();
}

boolean A4990DualMotorDriverCarrier::getFault()
{
  init(); // initialize if necessary
  if (errorsEnabled == true) {
    error = (digitalRead(EF1) == LOW || digitalRead(EF1) == LOW);
    return error;
  }
  return false;
}

unsigned int A4990DualMotorDriverCarrier::determineFault()
{
  init(); // initialize if necessary
  if (errorsEnabled == true) {
    errorType = (digitalRead(EF1) << 1) | (digitalRead(EF2));
    return errorType;
  } 
  return A4990DualMotorDriverCarrier::NOFAULT;
}

boolean A4990DualMotorDriverCarrier::stopIfFault()
{
  init(); // initialize if necessary
  error = getFault();
  if (error)
  {
    setSpeeds(0,0);
  }
  return error;
}
