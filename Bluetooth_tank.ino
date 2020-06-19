#include <AFMotor.h>
AF_DCMotor M1 (1);
AF_DCMotor M2 (2);
char command;

void setup() {
  Serial.begin(9600);

 }

void loop() {
  if (Serial.available() > 0)
  {
    command = Serial.read();
    stop();
    switch(command)
    {
      case 'F':
      forward();
      break;
      case 'B':
      back();
      break;
      case 'L':
      left();
      break;
      case 'R':
      right();
      break;
    }
  }
}

void forward()
{
  M1.setSpeed(255);
  M1.run(FORWARD);
  M2.setSpeed(255);
  M2.run(FORWARD);
}

void back()
{
  M1.setSpeed(255);
  M1.run(BACKWARD);
  M2.setSpeed(255);
  M2.run(BACKWARD);
}

void left()
{
  M1.setSpeed(255);
  M1.run(BACKWARD);
  M2.setSpeed(255);
  M2.run(FORWARD);
}

void right()
{
  M1.setSpeed(255);
  M1.run(FORWARD);
  M2.setSpeed(255);
  M2.run(BACKWARD);
}

void stop()
{
  M1.setSpeed(0);
  M1.run(RELEASE);
  M2.setSpeed(0);
  M2.run(RELEASE);
}
