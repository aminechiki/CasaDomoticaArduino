#ifndef motor_shutters_h
#define motor_shutters_h

class motor_shutters {

private:
  int pin_on_motor_shutters;
  int pin_down_motor_shutters;
  int pin_control_speed_motor;
  int direction_motor_shutters;
  float min_power_request_range;
  float max_power_request_range;
  enum states_motor_shutters { stopped,
                               on_shutters,
                               down_shutters };

public:
  bool safety;
  motor_shutters(int set_pin_on_motor_shutters ,int set_pin_down_motor_shutters ,int pin_control_speed_motor, float set_min_power_request_range ,float set_max_power_request_range);
  void control_direction(int direction_motor_shutters);
  void speed(float power_request);
};

//constructor
motor_shutters::motor_shutters(int set_pin_on_motor_shutters ,int set_pin_down_motor_shutters ,int set_pin_control_speed_motor, float set_min_power_request_range ,float set_max_power_request_range) {
 
pin_on_motor_shutters = set_pin_on_motor_shutters;
pinMode(pin_on_motor_shutters, 0);
pin_down_motor_shutters = set_pin_down_motor_shutters;
pinMode(pin_down_motor_shutters, 0);
pin_control_speed_motor = set_pin_control_speed_motor;
min_power_request_range = set_min_power_request_range;
max_power_request_range = set_max_power_request_range;

}
//function

void motor_shutters::control_direction(int direction_motor_shutters) {

  // if (direction_motor_shutters == stopped) Serial.println("motore fermo");
  // if (direction_motor_shutters == on_shutters) Serial.println("motore avanti");
  // if (direction_motor_shutters == down_shutters) Serial.println("motore indietro");

  switch (direction_motor_shutters) {

    case stopped:
      //Serial.println("motore fermo");
      digitalWrite(pin_on_motor_shutters, 0);
      digitalWrite(pin_down_motor_shutters, 0);

      break;

    case on_shutters:
      //Serial.println("motore avanti");
      digitalWrite(pin_on_motor_shutters, 1);
      digitalWrite(pin_down_motor_shutters, 0);
      break;

    case down_shutters:
      
      //Serial.println("motore indietro");
      digitalWrite(pin_on_motor_shutters, 0);
      digitalWrite(pin_down_motor_shutters, 1);
      break;
  }
}


void motor_shutters::speed(float power_request) {

if(safety) power_request = 0;
if(!safety) power_request = map(power_request, min_power_request_range, max_power_request_range, 0, 255);

analogWrite(pin_control_speed_motor ,power_request);
}

#endif