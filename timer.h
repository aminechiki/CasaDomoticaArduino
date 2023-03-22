#ifndef timer_h
#define timer_h

class timer {

private:
  int time;
  long time_start;
  long difference_time_system_time_start;

public:
  timer(int time);
  bool check_time();
};

//constructor

timer::timer(int set_time) {

  time = set_time;
}

bool timer::check_time() {

  difference_time_system_time_start = millis() - time_start;

  if (difference_time_system_time_start >= time) {

    time_start = millis();
    return true;

  } else {
    return false;
  }
}


#endif