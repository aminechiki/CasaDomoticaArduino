#ifndef buttons_shutters_h
#define buttons_shutters_h

class buttons_shutters {

private:
  //time
  int shutter_opening_closing_time;
  int rolling_shutter_rotation_time;
  //on shutters
  int button_on_shutters;
  int pressed_button_on_shutters;
  int previous_state_pressed_button_on_shutters;
  //down_shutters
  int button_down_shutters;
  int pressed_button_down_shutters;
  int previous_state_pressed_button_down_shutters;

  int state_out;
  enum states_motor_shutters { stopped,
                               on_shutters,
                               down_shutters };

public:
  buttons_shutters(int button_on_shutters, int button_down_shutters);
  int control_rotation();
  float control_speed(int shutter_opening_closing_time ,int rolling_shutter_rotation_time);
};

//constructor
buttons_shutters::buttons_shutters(int set_button_on_shutters, int set_button_down_shutters) {

  button_on_shutters = set_button_on_shutters;
  pinMode(button_on_shutters, 1);
  button_down_shutters = set_button_down_shutters;
  pinMode(button_down_shutters, 1);
}

//function

int buttons_shutters::control_rotation() {

  pressed_button_on_shutters = digitalRead(button_on_shutters);

  if (!previous_state_pressed_button_on_shutters && pressed_button_on_shutters) {

    previous_state_pressed_button_on_shutters = pressed_button_on_shutters;

    switch (state_out) {

      case stopped:
        state_out = on_shutters;
        break;

      case on_shutters:
        state_out = stopped;
        break;

      case down_shutters:
        state_out = stopped;
        break;
    }
  }

  if (previous_state_pressed_button_on_shutters && !pressed_button_on_shutters) {

    previous_state_pressed_button_on_shutters = pressed_button_on_shutters;
  }

  // - MOTORE INDIETRO

  pressed_button_down_shutters = digitalRead(button_down_shutters);

  if (!previous_state_pressed_button_down_shutters && pressed_button_down_shutters) {

    previous_state_pressed_button_down_shutters = pressed_button_down_shutters;

    switch (state_out) {

      case stopped:
        state_out = down_shutters;
        break;

      case on_shutters:
        state_out = stopped;
        break;

      case down_shutters:
        state_out = stopped;
        break;        
    }
  }

  if (previous_state_pressed_button_down_shutters && !pressed_button_down_shutters) {

    previous_state_pressed_button_down_shutters = pressed_button_down_shutters;
  }

  return state_out;
}



float buttons_shutters::control_speed(int shutter_opening_closing_time ,int rolling_shutter_rotation_time) {




}


#endif