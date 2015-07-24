//*
//*  Energia Library for the MPU6050 Accelerometer
//* 
//*  Partially based off of: "MPU-6050 Short Example Sketch"
//*  By Arduino User JohnChi, August 17, 2014
//*
//*  Written by SpEcTrUm
//*  07/10/2015
//*  
//*  Public Domain
//*

// Motion.cpp

#include <Arduino.h>
#include <Wire.h>
#include "Motion.h"

const int interrupt_pin = A0;

int count,lock_out = 0;

motion::motion()
{
  accX,accY,accZ,tmp,rotX,rotY,rotZ = 0;
  _interrupt_pin = interrupt_pin;
}

void motion::setupMPU6050()
{
  Serial.begin(DEBUG_BAUD); // DEBUGGING ONLY
  pinMode(_interrupt_pin, INPUT);
  Wire.begin();
}

void motion::runDefaultSetup()
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();
}

void motion::runTheftSetup()
{
  int temp = 0;
  count = 0;

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & STDBY_ACCEL;  // only bits [5:3]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(temp);  // write 0 to bits [5:3]
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & ACCEL_HPF;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(temp);  // write 0 to bits [2:0]
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & DLPF_CFG;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG);
  Wire.write(temp);  // write 0 to bits [2:0]
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(INT_ENABLE);
  Wire.write(MOT_EN);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_DUR);
  Wire.write(DEFAULT_DURATION);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_THR);
  Wire.write(THEFT_THRESHOLD);
  Wire.endTransmission();

  delay(100);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & ACCEL_HPF;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(temp + ACCEL_HPF_HIGH);  // write B111 to bits [2:0]
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & WKUP_FREQ;  // only bits [7:6]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(temp + WKUP_GYR_OFF);  // write B01 to bits [7:6] (frequency of 5Hz) and B111 to bits [2:0] (sleeps gyro)
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & SET_CYCLE;  // only bits [6:5]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(temp + CYCLE_ENABLE);  // write B01 to bits [6:5]
  Wire.endTransmission();

}

void motion::runSafetySetup()
{
  int temp = 0;
  lock_out = 0;

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & STDBY_ACCEL;  // only bits [5:3]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(temp);  // write 0 to bits [5:3]
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & ACCEL_HPF;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(temp);  // write 0 to bits [2:0]
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & DLPF_CFG;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG);
  Wire.write(temp);  // write 0 to bits [2:0]
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(INT_ENABLE);
  Wire.write(MOT_EN);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_DUR);
  Wire.write(DEFAULT_DURATION);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_THR);
  Wire.write(MOTION_THRESHOLD);
  Wire.endTransmission();

  delay(100);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & ACCEL_HPF;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(temp + ACCEL_HPF_HIGH);  // write B111 to bits [2:0]
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & WKUP_FREQ;  // only bits [7:6]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(temp + WKUP_GYR_ON);  // write B01 to bits [7:6] (frequency of 5Hz) and B000 to bits [2:0] (enables gyro)
  Wire.endTransmission();
}

void motion::getAllMotionData()
{
  accX,accY,accZ,rotX,rotY,rotZ = 0;
  int aX,aY,aZ,rX,rY,rZ = 0;
  for(int i = 0; i < SAMPLES; i++)
  {
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(ACCEL_XOUT_H);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDRESS,14,true);  // request a total of 14 registers
    aX += Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
    aY += Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    aZ += Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    tmp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    rX += Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    rY += Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    rZ += Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  }
  accX = aX/SAMPLES;
  accY = aY/SAMPLES;
  accZ = aZ/SAMPLES; 
  rotX = rX/SAMPLES;
  rotY = rY/SAMPLES;
  rotZ = rZ/SAMPLES;
}

void motion::getAccData()
{
  accX,accY,accZ = 0;
  int aX,aY,aZ = 0;
  for(int i = 0; i < SAMPLES; i++)
  {
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(ACCEL_XOUT_H);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDRESS,6,true);  // request a total of 14 registers
    aX += Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
    aY += Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    aZ += Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  }
  accX = aX/SAMPLES;
  accY = aY/SAMPLES;
  accZ = aZ/SAMPLES; 
}

void motion::getRotData()
{
  rotX, rotY, rotZ = 0;
  int rX,rY,rZ = 0;
  for(int i = 0; i < SAMPLES; i++)
  {
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(GYRO_XOUT_H);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDRESS,6,true);  // request a total of 14 registers
    rX += Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
    rY += Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    rZ += Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  }
  rotX = rX/SAMPLES;
  rotY = rY/SAMPLES;
  rotZ = rZ/SAMPLES; 
}

void motion::getCrashData()
{
  int currX,currY,currZ,prevX,prevY,prevZ = 0;
  int cX,cY,cZ,pX,pY,pZ = 0;
  for(int i = 0; i < SAMPLES/2; i++)
  {
    getRotData();
    pX += rotX;
    pY += rotY;
    pZ += rotZ;
    delay(100);
    getRotData();
    cX += rotX;
    cY += rotY;
    cZ += rotZ;
  }
  prevX =   (pX/SAMPLES)/2;
  prevY =   (pY/SAMPLES)/2;
  prevZ =   (pZ/SAMPLES)/2;
  currX =   (cX/SAMPLES)/2;
  currY =   (cY/SAMPLES)/2;
  currZ =   (cZ/SAMPLES)/2;

  Serial.print(" | GyXDiff = "); Serial.print(currX-prevX);
  Serial.print(" | GyYDiff = "); Serial.print(currY-prevY);
  Serial.print(" | GyZDiff = "); Serial.println(currZ-prevZ);
}

void motion::interruptDebug()
{
  Serial.println(digitalRead(_interrupt_pin));
  delay(100);
}

void motion::brakeLight(bool signal)
{
  // placeholder -- in final code, will be air-pack commands
  if(signal == true)
  {
    digitalWrite(RED_LED, HIGH);
  }
  else
  {
    digitalWrite(RED_LED, LOW);
  }
}

void motion::emergencyFlash()
{
  // placeholder -- in final code, will be air-pack commands
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
}

bool motion::motionDetected()
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(INT_STATUS);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  int temp = Wire.read();
  if(temp == MOTION_DETECTED)
  {
    count++;
  }
  if(count > 1 && temp == MOTION_DETECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool motion::crashDetected()
{
  if(lock_out >= 1)
  {
    return true;
  }
  else
  {
    if(abs(rotZ) >= ROLLOVER && abs(rotX) >= ROLLOVER)
    {
      lock_out++;
      return true;
    }
    else
    {
      return false;
    }
  }
}
