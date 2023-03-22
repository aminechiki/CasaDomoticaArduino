#ifndef pid_h
#define pid_h

class pid {

private:

  float power_request;
  float max_measurement_setpoint;
  float min_measurement_setpoint;
  float min_power_request_range;
  float max_power_request_range;
  bool safety_measurement;
  float integral;
  int sampling_time;
  int Ti;

public:
  float measurement_setpoint;
  float measurement_setpoint_range;
  float measurement;
  bool action_measurement;
  pid(float min_power_request_range, float max_power_request_range, int sampling_time, int Ti);
  float find_power_request();
};

//constructor
pid::pid(float set_min_power_request_range, float set_max_power_request_range, int set_sampling_time, int set_Ti) {
  min_power_request_range = set_min_power_request_range;
  max_power_request_range = set_max_power_request_range;
  sampling_time = set_sampling_time;
  Ti = set_Ti;
}
//function

float pid::find_power_request() {

  //check the measurement id nan
  if (isnan(measurement)) safety_measurement = true;
  if (!isnan(measurement)) safety_measurement = false;

  //1 - find the errore between measurement and setpoint
  float errore = measurement - (measurement_setpoint - measurement_setpoint_range);

  //2 - calcolate Kp
  float KP = max_power_request_range / (measurement_setpoint_range * 2);

  //3 - calcolate proportional and restrict the exit between min_power_request_range and max_power_request_range
  float proportional = errore * KP;

  //4 - calcolate integral
  integral += proportional * sampling_time / Ti;
  proportional = constrain(proportional, min_power_request_range, max_power_request_range);
  integral = constrain(integral, -1000, 1000);
  
  //5 - pid is the of proportional and integral
  float pid = proportional + integral;
  pid = constrain(pid, 0, 1000);

  float power_request = 0;

    //if action_measurement is true the power_request will be equal to the pid otherwise you will have a contrary output
    if (action_measurement) power_request = pid;
    if (!action_measurement) power_request = ((pid - min_power_request_range) / (max_power_request_range - min_power_request_range) * (min_power_request_range - max_power_request_range)) + max_power_request_range; 

  // Serial.print("input: ");
  // Serial.print(measurement);

  // Serial.print(", integrale: ");
  // Serial.print(integral);

  // Serial.print(", proporzionale: ");
  // Serial.print(proportional);

  // Serial.print("output: ");
  // Serial.println(pid);

  return constrain(power_request, 0, 1000);
}

#endif