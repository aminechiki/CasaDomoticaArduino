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
  float sensor_light_value;
  bool automatic_mode_shutters;
  bool first_automatic_mode_shutters;
  hysteresis_loop regolate_ligth_automatic = hysteresis_loop(min_power_request_range, max_power_request_range);
  
public:
  bool action_measurement;
  float measurement_setpoint;
  float measurement_setpoint_range;
  control_shutters(bool automatic_mode_shutters, int pin_sensor_light, int button_on_shutters, int button_down_shutters, float min_power_request_range, float max_power_request_range);
  int control_rotation();
  float regolate_shutters(float sensor_light_value, int shutter_opening_closing_time, int rolling_shutter_rotation_time, int max_shutter_working_time);
};

//constructor
control_shutters::control_shutters(bool set_automatic_mode_shutters, int set_pin_sensor_light, int set_button_on_shutters, int set_button_down_shutters, float set_min_power_request_range, float set_max_power_request_range) {
  button_on_shutters = set_button_on_shutters;
  pinMode(button_on_shutters, 1);
  button_down_shutters = set_button_down_shutters;
  pinMode(button_down_shutters, 1);
  min_power_request_range = set_min_power_request_range;
  max_power_request_range = set_max_power_request_range;
  //set the pin where the light sensor is located as input
  pin_sensor_light = set_pin_sensor_light;
  automatic_mode_shutters = set_automatic_mode_shutters;
}

//functions
int control_shutters::control_rotation() {

  //read and take button states
  pressed_button_on_shutters = digitalRead(button_on_shutters);
  pressed_button_down_shutters = digitalRead(button_down_shutters);

  if (automatic_mode_shutters) {
    if (pressed_button_on_shutters || pressed_button_down_shutters) {
      state_out = stopped;
      automatic_mode_shutters = false;
    }
  } else {
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
  }

  //when one of the two buttons is released ...
  if ((previous_state_pressed_button_on_shutters && !pressed_button_on_shutters) || (previous_state_pressed_button_down_shutters && !pressed_button_down_shutters)) {
    //saves the current state of the buttons
    previous_state_pressed_button_on_shutters = pressed_button_on_shutters;
    previous_state_pressed_button_down_shutters = pressed_button_down_shutters;
  }

  return state_out;
}

float control_shutters::regolate_shutters(float sensor_light_value, int shutter_opening_closing_time, int rolling_shutter_rotation_time, int max_shutter_working_time) {

  if ((millis() >= 20000) && (millis() <= 20200)) automatic_mode_shutters = true;

  if(!automatic_mode_shutters) first_automatic_mode_shutters = false;

  if (automatic_mode_shutters) {

    if (total_difference_time_start_down_on_shutters <= 0) {

       //Serial.println(sensor_light_value);
      //  Serial.println(action_measurement);
      //  Serial.println(measurement_setpoint);
      //  Serial.println(measurement_setpoint_range);

       // 1 - finds the value from 0 to 1023 with respect to the desired brightness percentage

       //float light_value_percentage = (measurement_setpoint / 100) * 1023;
       //Serial.println(light_value_percentage);

       // 2 - 

        //regolate_ligth_automatic.action_measurement = action_measurement;
        regolate_ligth_automatic.measurement_setpoint = measurement_setpoint;
        regolate_ligth_automatic.measurement_setpoint_range = measurement_setpoint_range;

       float min_measurement_setpoint = regolate_ligth_automatic.measurement_setpoint - regolate_ligth_automatic.measurement_setpoint_range;
       float max_measurement_setpoint = regolate_ligth_automatic.measurement_setpoint + regolate_ligth_automatic.measurement_setpoint_range;

       power_request = ((sensor_light_value - min_measurement_setpoint) / (max_measurement_setpoint - min_measurement_setpoint) * (min_power_request_range - max_power_request_range)) + max_power_request_range; 

       power_request = constrain(power_request, min_power_request_range ,max_power_request_range);

       Serial.println(sensor_light_value);

       Serial.println(power_request);
        
       if(power_request > 0 || power_request < 1000) state_out = stopped; 
       
       if(power_request <= min_power_request_range) { 
         raise_up = false;
         state_out = on_shutters;
         if(!first_automatic_mode_shutters) {
         sum_difference_time_start_down_shutters_rolling = 0;
         sum_difference_time_start_on_shutters_rolling = 0;
         }

         first_automatic_mode_shutters = true;

       }

       if(power_request >= max_power_request_range) {
         lower_down = false;
         state_out = down_shutters;
       }

    } else {
      lower_down= false;
      lower_down = false;
      state_out = down_shutters;
    }

      Serial.print("+++++++++++ PERCENTAGE REGOLATION +++++++++++");
  Serial.print("\n");
  Serial.print("PERCENTUALE SALI SCENDI TENDE : ");
  Serial.print(((float)total_difference_time_start_down_on_shutters / (float)shutter_opening_closing_time) * 100);
  Serial.print("\n");
  Serial.print("PERCENTUALE REGOLAZIONE TENDE : ");
  Serial.print(((float)total_difference_time_start_down_on_shutters_rolling / (float)rolling_shutter_rotation_time) * 100);
  Serial.print("\n");
  }





  switch (state_out) {

    case stopped:

      //first_automatic_mode_shutters = false;

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
      if (total_difference_time_start_down_on_shutters_rolling >= rolling_shutter_rotation_time) total_difference_time_start_down_on_shutters_rolling = rolling_shutter_rotation_time;

      // go up and down the roller shutter
      if (!is_down_stopped && is_down_shutters) sum_difference_time_start_down_shutters += difference_time_start_down_shutters;
      if (!is_on_stopped && is_on_shutters) sum_difference_time_start_on_shutters += difference_time_start_on_shutters;

      if (sum_difference_time_start_down_shutters > sum_difference_time_start_on_shutters) {
        total_difference_time_start_down_on_shutters = 0;
      } else {
        total_difference_time_start_down_on_shutters = sum_difference_time_start_on_shutters - sum_difference_time_start_down_shutters;
      }

      if (total_difference_time_start_down_on_shutters >= shutter_opening_closing_time) total_difference_time_start_down_on_shutters = shutter_opening_closing_time;

<<<<<<< HEAD:buttons_shutters.h
      Serial.print("+++++++++++ ROLLING +++++++++++");
      Serial.print("\n");
      Serial.print("ON : ");
      Serial.print(sum_difference_time_start_on_shutters_rolling);
      Serial.print("\n");
      Serial.print("DOWN : ");
      Serial.print(sum_difference_time_start_down_shutters_rolling);
      Serial.print("\n");
      Serial.print("SUM : ");
      Serial.print(total_difference_time_start_down_on_shutters_rolling);
      Serial.print("\n");

      // ON - OFF

      Serial.print("+++++++++++ ON / OFF +++++++++++");
      Serial.print("\n");
      Serial.print("ON : ");
      Serial.print(sum_difference_time_start_on_shutters);
      Serial.print("\n");
      Serial.print("DOWN : ");
      Serial.print(sum_difference_time_start_down_shutters);
      Serial.print("\n");
      Serial.print("SUM : ");
      Serial.print(total_difference_time_start_down_on_shutters);
      Serial.print("\n");
=======
      //PERCENTAGE REGOLATION
      // Serial.print("+++++++++++ PERCENTAGE REGOLATION +++++++++++");
      // Serial.print("\n");
      // Serial.print("PERCENTUALE SALI SCENDI TENDE : ");
      // Serial.print(((float)total_difference_time_start_down_on_shutters / (float)shutter_opening_closing_time) * 100);
      // Serial.print("\n");
      // Serial.print("PERCENTUALE REGOLAZIONE TENDE : ");
      // Serial.print(((float)total_difference_time_start_down_on_shutters_rolling / (float)rolling_shutter_rotation_time) * 100);
      // Serial.print("\n");

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
>>>>>>> 43321d282ccae75f94de98d08519c20500148592:control_shutters.h

      //reset states
      is_on_shutters = false;
      is_down_shutters = false;
      is_on_stopped = true;
      is_down_stopped = true;
      is_on_shutters_rolling = false;
      is_down_shutters_rolling = false;
<<<<<<< HEAD:buttons_shutters.h

      
       raise_up = false;
       lower_down = false;


=======
      raise_up = false;
      lower_down = false;
>>>>>>> 43321d282ccae75f94de98d08519c20500148592:control_shutters.h
      break;

    case on_shutters:

      //roller shutter rotation
      if (!raise_up) {
        
        if (!is_on_shutters_rolling) time_start_on_shutters_rolling = millis();
        difference_time_start_on_shutters_rolling = millis() - time_start_on_shutters_rolling;

        Serial.println(difference_time_start_on_shutters_rolling);
        power_request = ((((float)rolling_shutter_rotation_time - (float)min_power_request_range) / ((float)max_shutter_working_time) - (float)min_power_request_range) * (-max_power_request_range)) + max_power_request_range;

        //percentage update
        if (sum_difference_time_start_down_shutters_rolling > (sum_difference_time_start_on_shutters_rolling + difference_time_start_on_shutters_rolling)) {
          total_difference_time_start_down_on_shutters_rolling = 0;
        } else {
         
          total_difference_time_start_down_on_shutters_rolling = (sum_difference_time_start_on_shutters_rolling + difference_time_start_on_shutters_rolling) - sum_difference_time_start_down_shutters_rolling;
          if (total_difference_time_start_down_on_shutters_rolling >= rolling_shutter_rotation_time) {
            total_difference_time_start_down_on_shutters_rolling = rolling_shutter_rotation_time;
            raise_up = true;
          }

          if(automatic_mode_shutters) {

            if(total_difference_time_start_down_on_shutters_rolling >= rolling_shutter_rotation_time) state_out = stopped; 
            raise_up = false;
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

<<<<<<< HEAD:buttons_shutters.h
        Serial.println(difference_time_start_down_shutters_rolling);

        power_request = (((rolling_shutter_rotation_time / 1000) * 0.1) * (-1000)) + 1000;

        if (difference_time_start_down_shutters_rolling >= total_difference_time_start_down_on_shutters_rolling) {

          lower_down = true;
=======
        //percentage update
        if ((sum_difference_time_start_down_shutters_rolling + difference_time_start_down_shutters_rolling) > sum_difference_time_start_on_shutters_rolling) {
          total_difference_time_start_down_on_shutters_rolling = 0;
        } else {
          total_difference_time_start_down_on_shutters_rolling = sum_difference_time_start_on_shutters_rolling - (sum_difference_time_start_down_shutters_rolling + difference_time_start_down_shutters_rolling);
>>>>>>> 43321d282ccae75f94de98d08519c20500148592:control_shutters.h
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