#ifndef button_h
#define button_h

class button {

private:
  //PIN INPUT
  int pin_in;
  //POWER REQUEST
  float power_request;  
  float previous_power_request;
  int min_power_request_range;
  int max_power_request_range;
  //STATE
  bool state_out;
  bool previous_state_button;
  bool button_pressed_light_led;
  bool diminish_light_led;
  bool first_time_turn_on_light;
  bool first_time_dimmer_light;
  bool flag_first_time_dimmer_light;
  bool start_dimmer;
  bool minium_percentage_power_request;
  bool first_moment_start_dimmer;
  //TIME
  unsigned long button_press_time;
  unsigned long difference_time;
  unsigned long time_start_diminish_light_led;
  unsigned long time_start_diminuish; 
  unsigned long time_start_dimmer;
  unsigned long time_start_dimmer_difference;
  unsigned long first_time_dimmer_light_time_start;
  unsigned long first_time_dimmer_light_difference;

public:
  int turn_on_time;
  int maximum_light_on_time;
  bool state_in;
  button(int pin_in, int min_power_request_range, int max_power_request_range);
  float light_regolate(int minium_light_percentage ,int max_light_percentage);
};

//constructor
button::button(int set_pin_in, int set_min_power_request_range, int set_max_power_request_range) {
  
  //take the number of pin where connect led and put his state in high
  pin_in = set_pin_in;
  pinMode(pin_in, 1);
  //take the range of power request
  min_power_request_range = set_min_power_request_range;
  max_power_request_range = set_max_power_request_range;
}

float button::light_regolate(int minium_light_percentage, int max_light_percentage) {
  
  //are the percentages of power_request in which the led will light up
  int max_light = max_power_request_range * max_light_percentage / 100;
  int minium_light = max_power_request_range * minium_light_percentage / 100;

  //if the button is pressed ...
  if (!previous_state_button && state_in) {

    //toggles the illumination on and off when the button is pressed
    if (state_out) {
      power_request = 0;
      button_pressed_light_led = false;
    } else {
      power_request = previous_power_request;
      button_press_time = millis();
      button_pressed_light_led = true;
    }
    previous_state_button = state_in;
    state_out = !state_out;
  }

  //if the button hath been pressed it is to ignite ...
  if (button_pressed_light_led) {
    //time decrease since the button is pressed
    difference_time = millis() - button_press_time;

    //if the button spure the minimum detection time then it has been pressed to adjust the brightness
    if (difference_time > turn_on_time && !diminish_light_led) {
 
      //if it's the first time I want to illuminate the room by controlling the brightness
      if(!first_time_dimmer_light){

        if(!flag_first_time_dimmer_light) first_time_dimmer_light_time_start = millis();

      first_time_dimmer_light_difference = millis() - first_time_dimmer_light_time_start;
      
      flag_first_time_dimmer_light = true;
      power_request = max_light * first_time_dimmer_light_difference / maximum_light_on_time;
      }

      //if it's not the first time i want to adjust the brightness then ...
      if(start_dimmer){

      //I get the time difference based on the power_request
      float difference_time_power_request;

      if(!first_moment_start_dimmer) {
        difference_time_power_request = (maximum_light_on_time / max_light ) * power_request;
        time_start_dimmer = millis();
      }

      time_start_dimmer_difference = millis() - time_start_dimmer;
      power_request = max_light * (difference_time_power_request + time_start_dimmer_difference) / maximum_light_on_time;  
      first_moment_start_dimmer = true;
      }

      //if the power_request exceeds or is equal to the maximum then it starts to decrease the brightness
      if(power_request >= max_light) {    
        time_start_diminuish = millis();
        diminish_light_led = true;       
      }       
    }
    
    //diminish light led    
    //if the minium_light has been exceeded then the phase of brightness reduction begins
    if(diminish_light_led){
      
      //if I want to decrease I remove the possibility that the LED lights up
      start_dimmer = false;

      //if power_request is equal to or less than minium_light then power_request will be equal to minium_light      
      if(power_request <= minium_light){
              power_request = minium_light;
      }else{
      //if the power_request is greater than minium_light then it decreases it according to the elapsed time
      time_start_diminish_light_led = millis() - time_start_diminuish;
      power_request = max_light * (maximum_light_on_time - time_start_diminish_light_led) / maximum_light_on_time;
      
      //I check to verify that the power_request does not go into negativity
      if(power_request - (previous_power_request - power_request) < 0) power_request = minium_light;
      }
      // previous_power_request = power_request;
    }
      previous_power_request = power_request;
  }

  //if the button is released ...
  if (previous_state_button && !state_in) {

    if ((difference_time <= turn_on_time) && state_out && !first_time_turn_on_light && !start_dimmer) {
      //only first time light led with percentage
      first_time_turn_on_light = true;
      //calcolate percentage 
      power_request = minium_light_percentage * max_power_request_range / 100;
      
    }

    if(flag_first_time_dimmer_light) first_time_dimmer_light = true;
    
    first_moment_start_dimmer = false;
    previous_state_button = state_in;
    button_pressed_light_led = false;
    diminish_light_led = false;
    if(flag_first_time_dimmer_light && !diminish_light_led) start_dimmer = true;
  }

  Serial.print("POWER_REQUEST: ");
  Serial.print(power_request);
  Serial.print("\n");
  // Serial.print("difference_time : ");
  // Serial.print(difference_time);
  // Serial.print("\n");
  Serial.print("===============================================");
  Serial.print("\n");

  return constrain(power_request, min_power_request_range, max_power_request_range);
}

#endif