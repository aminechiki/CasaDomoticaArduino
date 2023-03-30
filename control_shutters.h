#ifndef control_shutters_h
#define control_shutters_h

class control_shutters {

private:
  //power request
  float power_request;
  int min_power_request_range;
  int max_power_request_range;
  //time
  int shutter_opening_closing_time;
  int rolling_shutter_rotation_time;

  //time on shutters
  unsigned long time_start_on_shutters;
  unsigned long difference_time_start_on_shutters;
  bool is_on_shutters;
  unsigned long sum_difference_time_start_on_shutters;
  //time down shutters
  unsigned long time_start_down_shutters;
  unsigned long difference_time_start_down_shutters;
  unsigned long sum_difference_time_start_down_shutters;
  bool is_down_shutters;
  //time down/on
  unsigned long difference_time_start_down_on_shutters;
  //on shutters
  int button_on_shutters;
  int pressed_button_on_shutters;
  int previous_state_pressed_button_on_shutters;
  //down_shutters
  int button_down_shutters;
  int pressed_button_down_shutters;
  int previous_state_pressed_button_down_shutters;
  //out
  int state_out;
  enum states_motor_shutters { stopped,
                               on_shutters,
                               down_shutters };

  bool is_on_stopped;
  bool is_down_stopped;
  unsigned long total_difference_time_start_down_on_shutters;

  //ROLLING
  //time on shutters
  unsigned long time_start_on_shutters_rolling;
  unsigned long difference_time_start_on_shutters_rolling;
  // bool is_on_shutters;
  unsigned long sum_difference_time_start_on_shutters_rolling;
  //time down shutters
  unsigned long time_start_down_shutters_rolling;
  unsigned long difference_time_start_down_shutters_rolling;
  unsigned long sum_difference_time_start_down_shutters_rolling;
  // bool is_down_shutters;
  //time down/on
  unsigned long difference_time_start_down_on_shutters_rolling;
  unsigned long total_difference_time_start_down_on_shutters_rolling;

  //states
  bool raise_up;
  bool lower_down;

  bool is_on_shutters_rolling;
  bool is_down_shutters_rolling;

  //automatic mode shutters
  int pin_sensor_light;

  bool automatic_mode_shutters;


public:
  control_shutters(int pin_sensor_light, int button_on_shutters, int button_down_shutters, float min_power_request_range, float max_power_request_range);
  int control_rotation();
  float regolate_shutters(bool automatic_mode_shutters, int shutter_opening_closing_time, int rolling_shutter_rotation_time, int max_shutter_working_time);
};

//constructor
control_shutters::control_shutters(int set_pin_sensor_light, int set_button_on_shutters, int set_button_down_shutters, float set_min_power_request_range, float set_max_power_request_range) {
  button_on_shutters = set_button_on_shutters;
  pinMode(button_on_shutters, 1);
  button_down_shutters = set_button_down_shutters;
  pinMode(button_down_shutters, 1);
  min_power_request_range = set_min_power_request_range;
  max_power_request_range = set_max_power_request_range;
  //set the pin where the light sensor is located as input
  pin_sensor_light = set_pin_sensor_light;
}

//functions
int control_shutters::control_rotation() {

  //read and take button states
  pressed_button_on_shutters = digitalRead(button_on_shutters);
  pressed_button_down_shutters = digitalRead(button_down_shutters);

  //when one of the two buttons is pressed...
  if ((!previous_state_pressed_button_on_shutters && pressed_button_on_shutters) || (!previous_state_pressed_button_down_shutters && pressed_button_down_shutters)) {

    switch (state_out) {

      case stopped:
        if (!previous_state_pressed_button_on_shutters && pressed_button_on_shutters) state_out = on_shutters;
        if (!previous_state_pressed_button_down_shutters && pressed_button_down_shutters) state_out = down_shutters;
        break;

      case on_shutters:
        if (!previous_state_pressed_button_on_shutters && pressed_button_on_shutters) state_out = stopped;
        if (!previous_state_pressed_button_down_shutters && pressed_button_down_shutters) state_out = stopped;
        break;

      case down_shutters:
        if (!previous_state_pressed_button_on_shutters && pressed_button_on_shutters) state_out = stopped;
        if (!previous_state_pressed_button_down_shutters && pressed_button_down_shutters) state_out = stopped;
        break;
    }

    //saves the current state of the buttons
    previous_state_pressed_button_on_shutters = pressed_button_on_shutters;
    previous_state_pressed_button_down_shutters = pressed_button_down_shutters;
  }

  //when one of the two buttons is released ...
  if ((previous_state_pressed_button_on_shutters && !pressed_button_on_shutters) || (previous_state_pressed_button_down_shutters && !pressed_button_down_shutters)) {
    //saves the current state of the buttons
    previous_state_pressed_button_on_shutters = pressed_button_on_shutters;
    previous_state_pressed_button_down_shutters = pressed_button_down_shutters;
  }

  return state_out;
}

float control_shutters::regolate_shutters(bool automatic_mode_shutters, int shutter_opening_closing_time, int rolling_shutter_rotation_time, int max_shutter_working_time) {

   

  if(automatic_mode_shutters) {
     
     //if one of the two buttons is pressed      
     if(pressed_button_on_shutters || pressed_button_down_shutters) automatic_mode_shutters = false;
     
     if((total_difference_time_start_down_on_shutters <= 0) && (total_difference_time_start_down_on_shutters_rolling <= 0)){
           
     }else{
          lower_down = false;
          state_out = down_shutters;
     }


    //  float sensor_light_value = analogRead(pin_sensor_light);
    //  Serial.println(sensor_light_value);
  }


      Serial.print("+++++++++++ PERCENTAGE REGOLATION +++++++++++");
      Serial.print("\n");
      Serial.print("PERCENTUALE SALI SCENDI TENDE : ");
      Serial.print(((float)total_difference_time_start_down_on_shutters / (float)shutter_opening_closing_time) * 100);
      Serial.print("\n");
      Serial.print("PERCENTUALE REGOLAZIONE TENDE : ");
      Serial.print(((float)total_difference_time_start_down_on_shutters_rolling / (float)rolling_shutter_rotation_time) * 100);
      Serial.print("\n");


  switch (state_out) {

    case stopped:

      power_request = 0;
      // - calculates the difference in engine displacements based on the displacements it has made

      //roller shutter rotation
      if (!is_down_stopped && is_down_shutters_rolling) sum_difference_time_start_down_shutters_rolling += difference_time_start_down_shutters_rolling;
      if (!is_on_stopped && is_on_shutters_rolling) sum_difference_time_start_on_shutters_rolling += difference_time_start_on_shutters_rolling;
      if (sum_difference_time_start_down_shutters_rolling > sum_difference_time_start_on_shutters_rolling) {
        total_difference_time_start_down_on_shutters_rolling = 0;
      } else {
        total_difference_time_start_down_on_shutters_rolling = sum_difference_time_start_on_shutters_rolling - sum_difference_time_start_down_shutters_rolling;
      }
      if(total_difference_time_start_down_on_shutters_rolling >= rolling_shutter_rotation_time) total_difference_time_start_down_on_shutters_rolling = rolling_shutter_rotation_time;

      // go up and down the roller shutter
      if (!is_down_stopped && is_down_shutters) sum_difference_time_start_down_shutters += difference_time_start_down_shutters;
      if (!is_on_stopped && is_on_shutters) sum_difference_time_start_on_shutters += difference_time_start_on_shutters;
      
      if (sum_difference_time_start_down_shutters > sum_difference_time_start_on_shutters) {
        total_difference_time_start_down_on_shutters = 0;
      } else {
        total_difference_time_start_down_on_shutters = sum_difference_time_start_on_shutters - sum_difference_time_start_down_shutters;
      }

      if(total_difference_time_start_down_on_shutters >= shutter_opening_closing_time) total_difference_time_start_down_on_shutters = shutter_opening_closing_time;

      //PERCENTAGE REGOLATION
      Serial.print("+++++++++++ PERCENTAGE REGOLATION +++++++++++");
      Serial.print("\n");
      Serial.print("PERCENTUALE SALI SCENDI TENDE : ");
      Serial.print(((float)total_difference_time_start_down_on_shutters / (float)shutter_opening_closing_time) * 100);
      Serial.print("\n");
      Serial.print("PERCENTUALE REGOLAZIONE TENDE : ");
      Serial.print(((float)total_difference_time_start_down_on_shutters_rolling / (float)rolling_shutter_rotation_time) * 100);
      Serial.print("\n");

      // - roller shutter rotation
      // Serial.print("+++++++++++ ROLLING +++++++++++");
      // Serial.print("\n");
      // Serial.print("ON : ");
      // Serial.print(sum_difference_time_start_on_shutters_rolling);
      // Serial.print("\n");
      // Serial.print("DOWN : ");
      // Serial.print(sum_difference_time_start_down_shutters_rolling);
      // Serial.print("\n");
      // Serial.print("SUM : ");
      // Serial.print(((float)total_difference_time_start_down_on_shutters_rolling / (float)rolling_shutter_rotation_time) * 100);
      // Serial.print("\n");

      // - go up and down the roller shutter
      // Serial.print("+++++++++++ ON / OFF +++++++++++");
      // Serial.print("\n");
      // Serial.print("ON : ");
      // Serial.print(sum_difference_time_start_on_shutters);
      // Serial.print("\n");
      // Serial.print("DOWN : ");
      // Serial.print(sum_difference_time_start_down_shutters);
      // Serial.print("\n");
      // Serial.print("SUM : ");
      // Serial.print(((float)total_difference_time_start_down_on_shutters / (float)shutter_opening_closing_time) * 100) ;
      // Serial.print("\n");

      //reset states
      is_on_shutters = false;
      is_down_shutters = false;
      is_on_stopped = true;
      is_down_stopped = true;
      is_on_shutters_rolling = false;
      is_down_shutters_rolling = false;
      raise_up = false;
      lower_down = false;
      break;

    case on_shutters:

      //roller shutter rotation
      if (!raise_up) {
        if (!is_on_shutters_rolling) time_start_on_shutters_rolling = millis();
        difference_time_start_on_shutters_rolling = millis() - time_start_on_shutters_rolling;
        power_request = ((((float)rolling_shutter_rotation_time - (float)min_power_request_range) / ((float)max_shutter_working_time) - (float)min_power_request_range) * (-max_power_request_range)) + max_power_request_range;

        //percentage update
        if (sum_difference_time_start_down_shutters_rolling > (sum_difference_time_start_on_shutters_rolling + difference_time_start_on_shutters_rolling)) {
          total_difference_time_start_down_on_shutters_rolling = 0;
        } else {
          total_difference_time_start_down_on_shutters_rolling = (sum_difference_time_start_on_shutters_rolling + difference_time_start_on_shutters_rolling) - sum_difference_time_start_down_shutters_rolling;
          if (total_difference_time_start_down_on_shutters_rolling >= rolling_shutter_rotation_time) { 
            total_difference_time_start_down_on_shutters_rolling =  rolling_shutter_rotation_time;
            raise_up = true;
          }
        }
        is_on_shutters_rolling = true;
      }

      // go up and down the roller shutter
      if (raise_up) {
        if (!is_on_shutters) time_start_on_shutters = millis();
        difference_time_start_on_shutters = millis() - time_start_on_shutters;
        power_request = ((((float)shutter_opening_closing_time - (float)min_power_request_range) / ((float)max_shutter_working_time) - (float)min_power_request_range) * (-max_power_request_range)) + max_power_request_range;
      
      //percentage update
      if (sum_difference_time_start_down_shutters > (sum_difference_time_start_on_shutters + difference_time_start_on_shutters)) {
        total_difference_time_start_down_on_shutters = 0;
      } else {
        total_difference_time_start_down_on_shutters = (sum_difference_time_start_on_shutters + difference_time_start_on_shutters) - sum_difference_time_start_down_shutters;
        if (total_difference_time_start_down_on_shutters >= shutter_opening_closing_time) {
          total_difference_time_start_down_on_shutters = shutter_opening_closing_time;
          state_out = stopped;
        }
      }
        is_on_shutters = true;
      }
      is_on_stopped = false;
      break;

    case down_shutters:

      //roller shutter rotation
      if (!lower_down) {
        if (!is_down_shutters_rolling) time_start_down_shutters_rolling = millis();
        difference_time_start_down_shutters_rolling = millis() - time_start_down_shutters_rolling;
        power_request = ((((float)rolling_shutter_rotation_time - (float)min_power_request_range) / ((float)max_shutter_working_time) - (float)min_power_request_range) * (-max_power_request_range)) + max_power_request_range;

        //percentage update
        if ((sum_difference_time_start_down_shutters_rolling + difference_time_start_down_shutters_rolling) > sum_difference_time_start_on_shutters_rolling) {
          total_difference_time_start_down_on_shutters_rolling = 0;
        } else {
          total_difference_time_start_down_on_shutters_rolling = sum_difference_time_start_on_shutters_rolling - (sum_difference_time_start_down_shutters_rolling + difference_time_start_down_shutters_rolling);
        }

        if (total_difference_time_start_down_on_shutters_rolling == 0) lower_down = true;

        is_down_shutters_rolling = true;
      }

      // go up and down the roller shutter
      if (lower_down) {
        if (!is_down_shutters) time_start_down_shutters = millis();
        difference_time_start_down_shutters = millis() - time_start_down_shutters;
        power_request = ((((float)shutter_opening_closing_time - (float)min_power_request_range) / ((float)max_shutter_working_time) - (float)min_power_request_range) * (-max_power_request_range)) + max_power_request_range;

        //percentage update
        if ((sum_difference_time_start_down_shutters + difference_time_start_down_shutters) > sum_difference_time_start_on_shutters) {
        total_difference_time_start_down_on_shutters = 0;
      } else {
        total_difference_time_start_down_on_shutters = sum_difference_time_start_on_shutters - (sum_difference_time_start_down_shutters + difference_time_start_down_shutters); 
      }
        if (total_difference_time_start_down_on_shutters == 0) state_out = stopped;
        is_down_shutters = true;
      }
      is_down_stopped = false;
      break;
  }
  return power_request;
}

#endif