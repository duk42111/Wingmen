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

// Motion.h

#ifndef _MSP432_MOTION_
#define _MSP432_MOTION_

#define MPU_ADDRESS 0x68       // I2C address of the MPU-6050

// MPU6050 Registers:
#define PWR_MGMT_1 0x6B        // PWR_MGMT_1 register
#define PWR_MGMT_2 0x6C        // PWR_MGMT_2 register
#define INT_ENABLE 0x38        // MPU6050 interrupt enable 
#define INT_STATUS 0x3A        // MPU6050 interrupt status
#define CONFIG 0x1A		       // MPU6050 LPF settings
#define ACCEL_CONFIG 0x1C      // configures MPU6050 accelerometer
#define MOT_EN 0x40            // enables MPU6050 "Motion Detection" to generate an interrupt
#define MOT_DUR 0x20		   // sets duration for MPU6050 "Motion Detection"
#define MOT_THR 0x1F           // sets threshold for MPU6050 "Motion Detection"

#define ACCEL_XOUT_H 0x3B      // x-direction register -- bits [15:8]
#define ACCEL_XOUT_L 0x3C      // x-direction register -- bits [7:0] 
#define ACCEL_YOUT_H 0x3D      // y-direction register -- bits [15:8]
#define ACCEL_YOUT_L 0x3E      // y-direction register -- bits [7:0]
#define ACCEL_ZOUT_H 0x3F      // z-direction register -- bits [15:8]
#define ACCEL_ZOUT_L 0x40      // z-direction register -- bits [7:0]
#define TEMP_OUT_H 0x41        // temperature register -- bits [15:8]
#define TEMP_OUT_L 0x42        // temperature register -- bits [7:0]
#define GYRO_XOUT_H 0x43       // x-direction register -- bits [15:8]
#define GYRO_XOUT_L 0x44       // x-direction register -- bits [7:0]
#define GYRO_YOUT_H 0x45       // y-direction register -- bits [15:8]
#define GYRO_YOUT_L 0x46       // y-direction register -- bits [7:0]
#define GYRO_ZOUT_H 0x47       // z-direction register -- bits [15:8]
#define GYRO_ZOUT_L 0x48       // z-direction register -- bits [7:0]

#define STDBY_ACCEL 0xC7	   // used to set bits [5:3] in the PWR_MGMT_2 register
#define ACCEL_HPF 0xF8         // used to set bits [2:0] in the ACCEL_CONFIG register (high-pass filter)
#define ACCEL_HPF_HIGH 7       // used to set bits [2:0] in the ACCEL_CONFIG register (high-pass filter)
#define DLPF_CFG 0xF8		   // used to set bits [2:0] in the CONFIG register (high-pass filter bandwidth)
#define WKUP_FREQ 0x3F         // used to set wakeup frequency of 5Hz (PWR_MGMT_2 register)
#define SET_CYCLE 0x9F		   // isolates PWR_MGMT_1 bits [6:5]
#define CYCLE_ENABLE 0x10      // enables cycling of MPU6050 (disables sleep)

#define DEFAULT_DURATION 1
#define THEFT_THRESHOLD 40     // threshold for MPU6050 "Motion Detection" during theft
#define MOTION_THRESHOLD 20    // threshold for MPU6050 "Motion Detection" during a crash
#define MOTION_DETECTED 0x41   // if the INT_STATUS register returns 0x41 theft has occured
#define WKUP_GYR_OFF 0x47      // sets PWR_MGMT_2 resgister to desired refresh rate (gyro DISABLED)
#define WKUP_GYR_ON 0x40       // sets PWR_MGMT_2 resgister to desired refresh rate (gyro ENABLED)

#define SAMPLES 10             // defines the number of samples for averaging within getMotionData()
#define DEBUG_BAUD 9600        // baud-rate for debugging (9600 works best)
#define READING_DELAY 100      // delay-rate for MPU6050 readings (in ms)
#define ROLLOVER 4000		   // value that deterimines a "crash event" (based on X and Z gyro values)
#define ON true                // used for brake light functionality
#define OFF false              // used for brake light functionality

class motion
{
    public:
	    motion();
		void setupMPU6050(void);    	       // begins Wire communication and serial debugging
		void runDefaultSetup(void); 	       // initializes the MPU6050 for normal readings (always on)
		void runTheftSetup(void);   	       // initializes the MPU6050 "Motion Detection" (cycles as defined in THEFT_CYCLE_RATE)
		void runSafetySetup(void);  	       // initializes the MPU6050 "Motion Detection" and gyrometer readings (non-cycle mode)
		void getAllMotionData(void);
		void getAccData(void);
		void getRotData(void);
		void getCrashData(void);
		void interruptDebug(void);
		void brakeLight(bool signal);				   // signals MSP430 to activate brake light 			
		void emergencyFlash(void); 			   // used in event of a crash, tells MSP430 to flash all 3 channels constantly
		bool motionDetected(void);             // returns true if motion is occuring -- threshold set by either theft or safety setup	
		bool crashDetected(void);              // returns TRUE if a crash has been detected
		int accX,accY,accZ,tmp,rotX,rotY,rotZ;
	private:
		int _interrupt_pin;			
}; 

#endif
