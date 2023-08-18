#include "MotorController.h"

void forward(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed)
{
  digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); //Motor B Rotate Clockwise
  digitalWrite(BIN2,LOW);
  analogWrite(PWM1,Speed); //Speed control of Motor A
  analogWrite(PWM2,Speed); //Speed control of Motor B
  //Serial.println("Fwd");
}
void backward(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed)
{
  digitalWrite(AIN1,LOW); //Motor A Rotate Clockwise
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW); //Motor B Rotate Clockwise
  digitalWrite(BIN2,HIGH);
  analogWrite(PWM1,Speed); //Speed control of Motor A
  analogWrite(PWM2,Speed); //Speed control of Motor B
  //Serial.println("Back");
}
void right(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed)
{
  digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); //Motor B Rotate Clockwise
  digitalWrite(BIN2,HIGH);
  analogWrite(PWM1,Speed); //Speed control of Motor A
  analogWrite(PWM2,Speed); //Speed control of Motor B
  //Serial.println("right");
}
void left(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed)
{
  digitalWrite(AIN1,LOW); //Motor A Rotate Clockwise
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); //Motor B Rotate Clockwise
  digitalWrite(BIN2,LOW);
  analogWrite(PWM1,Speed); //Speed control of Motor A
  analogWrite(PWM2,Speed); //Speed control of Motor B
  //Serial.println("left");
}
void Stop(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed)
{
  digitalWrite(AIN1,LOW); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); //Motor B Rotate Clockwise
  digitalWrite(BIN2,LOW);
  analogWrite(PWM1,0); //Speed control of Motor A
  analogWrite(PWM2,0); //Speed control of Motor B
  //Serial.println("STOP");
}
