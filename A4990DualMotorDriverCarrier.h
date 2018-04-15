#ifndef A4990DualMotorDriverCarrier_h
#define A4990DualMotorDriverCarrier_h

#include <Arduino.h>
#include <WString.h>

class A4990DualMotorDriverCarrier
{
  public:
    enum FAULTS { OVER_VOLT_TEMP, OPENLOAD, OVERCURRENT, NOFAULT };

    A4990DualMotorDriverCarrier(int in1 = 5, int in2 = 6, int in3 = 9, int in4 = 10);
    static void enableInterrupts(int encint1 = 2, int encdir1 = 7, int encint2 = 3, int encdir2 = 8);
    static void enableErrorHandling(int ef1 = 11, int ef2 = 12);
    static void enableSleepControl(int inh = 4);

    static void setM1Speed(int speed);
    static void setM2Speed(int speed);
    static void setSpeeds(int m1Speed, int m2Speed);
    static void setFlipM1(boolean flip);
    static void setFlipM2(boolean flip);

    static void setSleep(boolean sleep);

    static long readM1Rotation();
    static long readM2Rotation();
    static void resetM1Rotation();
    static void resetM2Rotation();
    static void resetRotations();
    static String errorToString(int errorCode);

    //  static void synchronizeMotors(int speed);

    static boolean getFault();
    static unsigned int determineFault();
    static boolean stopIfFault();

    static boolean changeFlagM1;
    static boolean changeFlagM2;
    static boolean changeFlag;

  private:
    static void initPinsAndTimer();
    static void initInterruptPins();
    static void initErrorPins();
    static void initSleepPins();

    static void encoderIntM1();
    static void encoderIntM2();

    static unsigned char M1IN1;
    static unsigned char M1IN2;
    static unsigned char M2IN3;
    static unsigned char M2IN4;
    static unsigned char ENCINT1;
    static unsigned char ENCDIR1;
    static unsigned char ENCINT2;
    static unsigned char ENCDIR2;
    static unsigned char EF1;
    static unsigned char EF2;
    static unsigned char INH;
    static unsigned int errorType;
    static boolean flipM1;
    static boolean flipM2;
    static boolean error;
    static boolean initialized;
    static boolean interruptsEnabled;
    static boolean interruptsInitialized;
    static boolean errorsEnabled;
    static boolean errorsInitialized;
    static boolean sleepControlEnabled;
    static boolean sleepControlInitialized;
    static volatile long encoderM1Count;
    static volatile long encoderM2Count;

    const static int MAXSPEED;

    static inline void init() {
      static boolean initialized = false;
      if (!initialized) {
        initPinsAndTimer();
        initialized = true;
      }
      static boolean interruptsInitialized = false;
      if (!interruptsInitialized && interruptsEnabled) {
        initInterruptPins();
        interruptsInitialized = true;
      }
      static boolean errorsInitialized = false;
      if (!errorsInitialized && errorsEnabled) {
        initErrorPins();
        errorsInitialized = true;
      }
      static boolean sleepControlInitialized = false;
      if (!sleepControlInitialized && sleepControlEnabled) {
        initSleepPins();
        sleepControlInitialized = true;
      }
    }
};
#endif
