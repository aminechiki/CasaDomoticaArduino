#ifndef led_h
#define led_h

class led {

  private:
  float power_request;
  float min_power_request_range;
  float max_power_request_range;
  int actuator;

  public:
  bool safety;
  led(float set_min_power_request_range ,float set_max_power_request_range, int actuator);
  void blink(float power_request);

};

//constructor
led::led(float set_min_power_request_range ,float set_max_power_request_range, int set_actuator){

 min_power_request_range = set_min_power_request_range;
 max_power_request_range = set_max_power_request_range;
 actuator = set_actuator;
 pinMode(actuator, OUTPUT);
}

//function

void led::blink(float power_request){

  if(safety){
   power_request = 0;
  }else{
   power_request = map(power_request, min_power_request_range, max_power_request_range, 0, 255);
   analogWrite(actuator, power_request);
  }
}

#endif