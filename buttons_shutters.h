#ifndef buttons_shutters_h
#define buttons_shutters_h

class buttons_shutters {

private:
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
  float power_request;
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

public:
  buttons_shutters(int button_on_shutters, int button_down_shutters);
  int control_rotation();
  float control_speed(int shutter_opening_closing_time, int rolling_shutter_rotation_time);
};

//constructor
buttons_shutters::buttons_shutters(int set_button_on_shutters, int set_button_down_shutters) {
  button_on_shutters = set_button_on_shutters;
  pinMode(button_on_shutters, 1);
  button_down_shutters = set_button_down_shutters;
  pinMode(button_down_shutters, 1);
}

//functions
int buttons_shutters::control_rotation() {

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

float buttons_shutters::control_speed(int shutter_opening_closing_time, int rolling_shutter_rotation_time) {

  switch (state_out) {

    case stopped:

      //ROLLING

      if (!is_down_stopped && is_down_shutters_rolling) sum_difference_time_start_down_shutters_rolling += difference_time_start_down_shutters_rolling;
      if (!is_on_stopped && is_on_shutters_rolling) sum_difference_time_start_on_shutters_rolling += difference_time_start_on_shutters_rolling;

      if (sum_difference_time_start_down_shutters_rolling > sum_difference_time_start_on_shutters_rolling) {
        total_difference_time_start_down_on_shutters_rolling = 0;
      } else {
        total_difference_time_start_down_on_shutters_rolling = sum_difference_time_start_on_shutters_rolling - sum_difference_time_start_down_shutters_rolling;
      }

      // ON - OFF

      if (!is_down_stopped && is_down_shutters) sum_difference_time_start_down_shutters += difference_time_start_down_shutters;
      if (!is_on_stopped && is_on_shutters) sum_difference_time_start_on_shutters += difference_time_start_on_shutters;

      if (sum_difference_time_start_down_shutters > sum_difference_time_start_on_shutters) {
        total_difference_time_start_down_on_shutters = 0;
      } else {
        total_difference_time_start_down_on_shutters = sum_difference_time_start_on_shutters - sum_difference_time_start_down_shutters;
      }

      // ROLLING

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

      is_on_shutters = false;
      is_down_shutters = false;

      is_on_stopped = true;
      is_down_stopped = true;

      ////// ROLLING

      is_on_shutters_rolling = false;
      is_down_shutters_rolling = false;

      
       raise_up = false;
       lower_down = false;


      break;

    case on_shutters:

      //ROLLING

      if (!raise_up) {

        if (!is_on_shutters_rolling) time_start_on_shutters_rolling = millis();
        difference_time_start_on_shutters_rolling = millis() - time_start_on_shutters_rolling;

        power_request = (((rolling_shutter_rotation_time / 1000) * 0.1) * (-1000)) + 1000;
        Serial.println(difference_time_start_on_shutters_rolling);

        if (total_difference_time_start_down_on_shutters_rolling + difference_time_start_on_shutters_rolling >= rolling_shutter_rotation_time) raise_up = true;

        is_on_shutters_rolling = true;
      }

      //ON - OFF

      if (raise_up) {

        if (!is_on_shutters) time_start_on_shutters = millis();
        difference_time_start_on_shutters = millis() - time_start_on_shutters;

        power_request = (((shutter_opening_closing_time / 1000) * 0.1) * (-1000)) + 1000;
        Serial.println(difference_time_start_on_shutters);

        if (total_difference_time_start_down_on_shutters + difference_time_start_on_shutters >= shutter_opening_closing_time) {

          state_out = stopped;
        }

        is_on_shutters = true;
      }

      is_on_stopped = false;
      break;

    case down_shutters:

      //ROLLING

      if (!lower_down) {

        if (!is_down_shutters_rolling) time_start_down_shutters_rolling = millis();
        difference_time_start_down_shutters_rolling = millis() - time_start_down_shutters_rolling;

        Serial.println(difference_time_start_down_shutters_rolling);

        power_request = (((rolling_shutter_rotation_time / 1000) * 0.1) * (-1000)) + 1000;

        if (difference_time_start_down_shutters_rolling >= total_difference_time_start_down_on_shutters_rolling) {

          lower_down = true;
        }
        is_down_shutters_rolling = true;
      }


      if (lower_down) {

        if (!is_down_shutters) time_start_down_shutters = millis();
        difference_time_start_down_shutters = millis() - time_start_down_shutters;

        Serial.println(difference_time_start_down_shutters);
        power_request = (((shutter_opening_closing_time / 1000) * 0.1) * (-1000)) + 1000;

        if (difference_time_start_down_shutters >= total_difference_time_start_down_on_shutters) {

          state_out = stopped;
        }

        is_down_shutters = true;
      }

      is_down_stopped = false;

      break;
  }

  return power_request;
}

#endif