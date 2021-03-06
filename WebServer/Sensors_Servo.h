
#ifndef SENSORS_SERVO
#define SENSORS_SERVO

#define SWITCH_1_PIN  5
#define SERVO_PIN     9

//http://www.servodatabase.com/servo/towerpro/mg90
#define SERVO_PULSE_MAX 2700
#define SERVO_PULSE_MIN 500
#define SERVO_MAX 180
#define SERVO_MIN 0
#define SERVO_DELAY 20 // delay between increments. aka speed
#define SERVO_INCREMENT 1


#define SERVO_CLOSE 0
#define SERVO_OPEN  1
#define SERVO_OPENING 2
#define SERVO_CLOSING 3


// prototypes
void initSensors();
void getSwitches();

void ServoSet(int newstate);
void ServoUpdate();

#endif
