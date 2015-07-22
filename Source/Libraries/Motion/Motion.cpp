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

const int button_pin = PUSH1;
const int MPU6050_power = 24;

int count,lock_out = 0;

unsigned long start_time = 0;

bool checking = true;
bool theft = false;
bool stopped = false;

motion::motion()
{
  accX,accY,accZ,tmp,rotX,rotY,rotZ = 0;
  _button_pin = button_pin;
  _MPU6050_power = MPU6050_power;
}

void motion::setupMPU6050()
{
  digitalWrite(_MPU6050_power, LOW);
  Serial.begin(DEBUG_BAUD); // DEBUGGING ONLY
  pinMode(_button_pin, INPUT_PULLUP);
  //pinMode(_MPU6050_power, OUTPUT);
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
  resetMPU6050();

  int temp = 0;
  count = 0;

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & STDBY_ACCEL;  // only bits [5:3]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(temp);  // write 0 to bits [5:3]
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & ACCEL_HPF;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(temp);  // write 0 to bits [2:0]
  Wire.endTransmission();
  
  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & DLPF_CFG;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG);
  Wire.write(temp);  // write 0 to bits [2:0]
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(INT_ENABLE);
  Wire.write(0);
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(INT_ENABLE);
  Wire.write(MOT_EN);
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_DUR);
  Wire.write(DEFAULT_DURATION);
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_THR);
  Wire.write(THEFT_THRESHOLD);
  Wire.endTransmission();

  delay(50);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & ACCEL_HPF;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(temp + ACCEL_HPF_HIGH);  // write B111 to bits [2:0]
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & WKUP_FREQ;  // only bits [7:6]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(temp + WKUP_GYR_OFF);  // write B01 to bits [7:6] (frequency of 5Hz) and B111 to bits [2:0] (sleeps gyro)
  Wire.endTransmission();

  delay(10);

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
  resetMPU6050();
  int temp = 0;
  count = 0;
  lock_out = 0;

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & STDBY_ACCEL;  // only bits [5:3]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(temp);  // write 0 to bits [5:3]
  Wire.endTransmission();
  
  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & ACCEL_HPF;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(temp);  // write 0 to bits [2:0]
  Wire.endTransmission();
  
  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & DLPF_CFG;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG);
  Wire.write(temp);  // write 0 to bits [2:0]
  Wire.endTransmission();
  
  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(INT_ENABLE);
  Wire.write(0);
  Wire.endTransmission();
  
  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(INT_ENABLE);
  Wire.write(ZRMOT_EN + MOT_EN);
  Wire.endTransmission();
  
  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ZRMOT_DUR);
  Wire.write(DEFAULT_DURATION);
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_DUR);
  Wire.write(DEFAULT_DURATION);
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ZRMOT_THR);
  Wire.write(ZRMOTION_THRESHOLD);
  Wire.endTransmission();
  
  delay(50);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_THR);
  Wire.write(THEFT_THRESHOLD);
  Wire.endTransmission();

  delay(50);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & ACCEL_HPF;  // only bits [2:0]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(temp + ACCEL_HPF_HIGH);  // write B111 to bits [2:0]
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  temp = Wire.read() & WKUP_FREQ;  // only bits [7:6]
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_2);
  Wire.write(temp + WKUP_GYR_OFF);  // write B01 to bits [7:6] (frequency of 5Hz) and B111 to bits [2:0] (disables gyro)
  Wire.endTransmission();
}

void motion::resetMPU6050()
{
  digitalWrite(_MPU6050_power, HIGH);
  delay(100);
  digitalWrite(_MPU6050_power, LOW);
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

  Serial.print("AcX = "); Serial.print(accX);
  Serial.print(" | AcY = "); Serial.print(accY);
  Serial.print(" | AcZ = "); Serial.print(accZ);
  //Serial.print(" | Tmp = "); Serial.print(wingman.temp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(rotX);
  Serial.print(" | GyY = "); Serial.print(rotY);
  Serial.print(" | GyZ = "); Serial.println(rotZ);
  //delay(1000);
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
    Wire.requestFrom(MPU_ADDRESS,6,true);  // request a total of 6 registers
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

void motion::brakeLight(bool signal)
{
  // placeholder -- in final code, will be air-pack commands
  if(signal == true)
  {
    digitalWrite(RED_LED, HIGH);
  }
  else
  {
    count = 0;
    digitalWrite(RED_LED, LOW);
  }
}

void motion::emergencyFlash()
{
  // placeholder -- in final code, will be air-pack commands
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);
  delay(500);
}

void motion::checkForStop()
{
  if(!stopped && stopDetected())
    {
      brakeLight(ON);
      stopped = true;
    }
  if(stopped)
  {
    if(motionDetected())
    {
      brakeLight(OFF);
      stopped = false;
    }
  }
}

bool motion::motionDetected()
{
  bool theft = false;
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(INT_STATUS);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  int temp = Wire.read();
  Serial.print("INT_STATUS: ");Serial.print(temp, HEX); Serial.print(" | "); Serial.println(temp, BIN);
  if(temp == THEFT_DETECTED)
  {
    count++;
    if(count > 5)
    {
      theft = true;
    }
  }
  return theft;
}

bool motion::stopDetected()
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MOT_DETECT_STATUS);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS,1,true);
  int temp = Wire.read();
  Serial.print("MOT_DETECT_STATUS: "); Serial.print(temp, HEX); Serial.print(" | "); Serial.println(temp);
  Serial.print("count: "); Serial.println(count);
  if(count == 0 && temp == STOP_DETECTED)
  {
    count++;
  }
  else
  {
    if(count == 1 && temp == STOP_DETECTED)
    {
      //Serial.println("First Stop");
      //Serial.println(checking);
      if(checking)
      {
        start_time = millis();
        checking = false;
      }
      if(millis() - start_time > STOP_QUALIFICATION)
      {
        Serial.println(start_time);
        checking = true;
        Wire.beginTransmission(MPU_ADDRESS);
        Wire.write(MOT_DETECT_STATUS);
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_ADDRESS,1,true);
        //Serial.println("Second Stop");
        if(Wire.read() == STOP_DETECTED)
        {       
          return true;
        }
      }
    }
  }
  return false;
}

bool motion::crashDetected() // void --> debugging, bool --> correct/release mode
{
  double pitch,roll = 0;
 /// if(digitalRead(_button_pin) == LOW) {
  ///for(int i = 0; i < 1000; i++)
  ///{
  int X,Y,Z = 0;
  getAccData();
  X = accX;
  Y = accY;
  Z = accZ;
  double temp = pow((pow(Y,2) + pow(Z,2)),0.5);
  roll  = (atan2(-Y, Z)*180.0)/PI;
  pitch = (atan2(X, temp)*180.0)/PI;
  if(abs(pitch) <= 60)
  {
    pitch = 0;
  }
  if(abs(roll) <= 60)
  {
    roll = 0;
  } 

  if(lock_out >= 1)
  {
    return true;
  }
  else
  {
    if((abs(pitch) + abs(roll)) >= 150)
    {
      lock_out++;
      return true;
    }
    else
    {
      return false;
    }
  }
   /* Serial.print(abs(pitch) + abs(roll)); Serial.print(",");
    delay(100);
  }
    Serial.println(" "); Serial.println("All Done!");
    while(1)
    {
        
    }
}*/}
