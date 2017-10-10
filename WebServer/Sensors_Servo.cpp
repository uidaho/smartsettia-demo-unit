#include "Arduino.h"
#include "Sensors_Servo.h"
#include <Servo.h>

Servo myservo;
short servo_state = SERVO_OPEN;

/*servo states
 *  0 open
 *  1 opening
 *  2 close
 *  3 closing
 */

void initSensors()
{
  Serial.println(F("Initializing pins"));
  pinMode(SWITCH_1_PIN, INPUT);
  pinMode(SERVO_PIN, OUTPUT);

  Serial.println(F("Initializing servo"));
  myservo.attach(SERVO_PIN, SERVO_PULSE_MIN, SERVO_PULSE_MAX);
  delay(10);
  ServoSet(SERVO_OPEN);
}


void getSwitches()
{
  static int val_prev = 0;
  int val = digitalRead(SWITCH_1_PIN);
  if (val == 1 && val_prev == 0) {
    if (servo_state == SERVO_OPEN)
      ServoSet(SERVO_CLOSE);
    else if (servo_state == SERVO_CLOSE)
      ServoSet(SERVO_OPEN);
    // else its moving. ignoring
  }

  val_prev = val;
}

void ServoSet(int newstate)
{
  if (newstate == SERVO_OPEN) {
    servo_state = SERVO_OPENING;
    Serial.println("toggle open");
  }
  else {
    servo_state = SERVO_CLOSING;
    Serial.println("toggle close");
  }

  myservo.attach(SERVO_PIN, SERVO_PULSE_MIN, SERVO_PULSE_MAX);
}

void ServoUpdate()
{
  static short servo_pos   = 90;
  short new_pos;
  static unsigned int previousMillis = 0;  // store last update time

  // Return if still waiting till SERVO_DELAY time
  unsigned int currentMillis = millis();
  if (currentMillis - previousMillis < SERVO_DELAY) { // if less than SERVO_DELAY
    return;
  }

  // We waited long enough. Lets do some work
  previousMillis = currentMillis;

  if (servo_state == SERVO_OPEN || servo_state == SERVO_CLOSE)
    return;  // already open or close. Do nothing


  if (servo_state == SERVO_CLOSING) {      // closing
    new_pos = servo_pos + SERVO_INCREMENT;
    if (new_pos >= SERVO_MAX) {   // if over max, set to max, state now close
      new_pos = SERVO_MAX;
      servo_state = SERVO_CLOSE;
      myservo.detach();
    }
    myservo.write(new_pos);
    servo_pos = new_pos;
  }
  else if (servo_state == SERVO_OPENING) { // opening
      new_pos = servo_pos - SERVO_INCREMENT;
    if (new_pos <= SERVO_MIN) {   // if under min, set to min, state now open
      new_pos = SERVO_MIN;
      servo_state = SERVO_OPEN;
      myservo.detach();
    }
    myservo.write(new_pos);
    servo_pos = new_pos;
  }
}
