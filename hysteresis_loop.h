#ifndef hysteresis_loop_h
#define hysteresis_loop_h

class hysteresis_loop {

//parameters
private:

  float max_measurement_setpoint;
  float min_measurement_setpoint;
  float min_power_request_range;
  float max_power_request_range;
  float power_request;

  bool safety_measurement;

public:
  bool action_measurement;
  bool safety_power_request;
  float measurement_setpoint;
  float measurement_setpoint_range;
  hysteresis_loop(float min_power_request_range, float max_power_request_range);
  float find_power_request(float measurement);
};

//constructor
hysteresis_loop::hysteresis_loop(float set_min_power_request_range, float set_max_power_request_range) {

  max_measurement_setpoint = measurement_setpoint + measurement_setpoint_range;
  min_measurement_setpoint = measurement_setpoint - measurement_setpoint_range;
  min_power_request_range = set_min_power_request_range;
  max_power_request_range = set_max_power_request_range;
}

float hysteresis_loop::find_power_request(float measurement) {

  if (isnan(measurement)) safety_measurement = true;
  if (!isnan(measurement)) safety_measurement = false;

  //if safety_power_request equals true the output will be equal to 0
  if (safety_power_request) {
    power_request = 0;
  } else {
    if(action_measurement) power_request = ((measurement - min_measurement_setpoint) / (max_measurement_setpoint - min_measurement_setpoint)) * (max_power_request_range - min_power_request_range) + min_power_request_range;
    if(!action_measurement) power_request = ((measurement - min_measurement_setpoint) / (max_measurement_setpoint - min_measurement_setpoint) * (min_power_request_range - max_power_request_range)) + max_power_request_range;  
  }

  //limits range of power_request values to between min_power_request_range and max_power_request_range
  return constrain(power_request, min_power_request_range ,max_power_request_range);
}

#endif