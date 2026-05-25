//Michael Klements
//The DIY Life
//3 May 2020


#include <Adafruit_PWMServoDriver.h>                             //Include the PWM Driver library

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);    //Create an object of board 1
//可以级联第二个模块，注意要修改模块上的焊盘
//Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);    //Create an object of board 2 (A0 Address Jumper)

int servoMin =  150;        // This is the servos minimum pulse length count (out of 4096)
int servoMax =  600;        // This is the servos maximum pulse length count (out of 4096)
int servoFrequency = 50;    // Servo update frequency, analog servos typically run at ~50 Hz

void setup()
{
  pwm1.begin();             //Start each board
  //pwm2.begin();
  pwm1.setOscillatorFrequency(27000000);    //Set the PWM oscillator frequency, used for fine calibration
  //pwm2.setOscillatorFrequency(27000000);
  pwm1.setPWMFreq(servoFrequency);          //Set the servo operating frequency
  //pwm2.setPWMFreq(servoFrequency);
}

void loop()
{
  for (int i=0 ; i<=5 ; i++)   //Cycle through moving 6 servos on each board
  {
    for (int pulseLength = servoMin ; pulseLength <= servoMax ; pulseLength++)    //Move each servo from servoMin to servoMax
    {
      pwm1.setPWM(i, 0, pulseLength);           //Set the current PWM pulse length on board 1, servo i
      //pwm2.setPWM(i, 0, pulseLength);           //Set the current PWM pulse length on board 2, servo i
      delay(1);
    }
    delay(100);
    for (int pulseLength = servoMax ; pulseLength >= servoMin ; pulseLength--)   // Move each servo from servoMax to servoMin
    {
      pwm1.setPWM(i, 0, pulseLength);           //Set the current PWM pulse length on board 1, servo i
      //pwm2.setPWM(i, 0, pulseLength);           //Set the current PWM pulse length on board 2, servo i
      delay(1);
    }
    delay(100);
  }
  delay(500);
}
