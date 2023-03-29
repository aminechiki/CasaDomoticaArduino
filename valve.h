#ifndef valve_h
#define valve_h
#include <Servo.h>

class valve {

private:
  float min_power_request_range;
  float max_power_request_range;
  int actuator;
  int angle_valve;

public:
  Servo servo;
  bool safety;
  valve(Servo servo, float set_min_power_request_range, float set_max_power_request_range, int actuator);
  int spin(float a);
};

//constructor
valve::valve(Servo set_servo, float set_min_power_request_range, float set_max_power_request_range, int set_actuator) {
  servo = set_servo;
  actuator = set_actuator;
  min_power_request_range = set_min_power_request_range;
  max_power_request_range = set_max_power_request_range;
}

//function
int valve::spin(float power_request) {

  if (safety) {
    angle_valve = 0;
  }else{
    angle_valve = map(power_request, min_power_request_range, max_power_request_range, 0, 180);
    servo.attach(actuator);
    servo.write(angle_valve);
  }
  return angle_valve;
}

#endif