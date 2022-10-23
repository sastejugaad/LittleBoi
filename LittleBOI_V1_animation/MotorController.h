#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H
#include <Arduino.h>
#include <analogWrite.h>

void forward(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed);
void backward(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed);
void right(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed);
void left(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed);
void Stop(int PWM1,int AIN1,int AIN2,int PWM2,int BIN1,int BIN2,int Speed);

#endif
