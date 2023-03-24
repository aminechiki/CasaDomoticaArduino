#include <DHT.h>
#include "hysteresis_loop.h"
#include "led.h"
#include "valve.h"
#include "pid.h"
#include "timer.h"
#include <Servo.h>
#include "button.h"
#include "motor_shutters.h"
#include "buttons_shutters.h"

//CONTROL ROOM - (Arduino)
//INPUT
#define SENSOR_TEMPERATURE_HUMIDITY 2
#define LIGHT_CONTROL 9
#define BUTTON_ON_SHUTTERS 6
#define BUTTON_DOWN_SHUTTERS 5

//OUTPUT
#define TEMPERATURE_VALVE 5
#define HUMIDITY_VALVE 6
#define ROOM_LIGHTS 11
#define MOTOR_ON_SHUTTERS 3
#define MOTOR_DOWN_SHUTTERS 4
#define MOTOR_SPEED 9

//VARIABLES SET BY THE APPLICATION

//temperature
float TEMPERATURE_SETPOINT = 23;
float TEMPERATURE_SETPOINT_RANGE = 3;
//if it is true it cools the room
bool ACTION_TEMPERATURE = true;
//if the power_request is true the actuator will be equal to 0
bool TEMPERATURE_SAFETY = false;

//humidity
float HUMIDITY_SETPOINT = 30;
float HUMIDITY_SETPOINT_RANGE = 5;
//if true, it dehumidifies the room
bool ACTION_HUMIDITY = false;
//if the power_request is true the actuator will be equal to 0
bool HUMIDITY_SAFETY = false;

//are the range of values ​​that the actuator receives as input
float min_power_request_range = 0;
float max_power_request_range = 1000;
//it is the power_request that the actuators receive as input
float power_request_temperature_valve;
float power_request_humidity_valve;

//VARIABLE OF SYSTEM
//type of sensor for detecting the temperature and humidity in the room
#define DHTTYPE DHT11
DHT sensor_temperature_humidity(SENSOR_TEMPERATURE_HUMIDITY, DHTTYPE);

//TIME

//temperature and humidity
int time_temperature_humidity = 1000;
timer check_time_temperature_humidity = timer(time_temperature_humidity);

// - REGOLATE POWER REQUEST

// - HYSTERESIS LOOP
//temperature
hysteresis_loop temperature_hysteresis_loop = hysteresis_loop(min_power_request_range, max_power_request_range);
//humidity
hysteresis_loop humidity_hysteresis_loop = hysteresis_loop(min_power_request_range, max_power_request_range);

// - PID
int Ti = 10000;
//temperature
pid temperature_pid = pid(min_power_request_range, max_power_request_range, time_temperature_humidity, Ti);
//humidity
pid humidity_pid = pid(min_power_request_range, max_power_request_range, time_temperature_humidity, Ti);

// - ACTUATORS

// - VALVE
//temperature
Servo valve_temperature;
bool safety_valve_room_regolate_temperature = false;
valve valve_room_regolate_temperature = valve(valve_temperature, min_power_request_range, max_power_request_range, TEMPERATURE_VALVE);
//humidity
Servo valve_humidity;
bool safety_valve_room_regolate_humidity = false;
valve valve_room_regolate_humidity = valve(valve_humidity, min_power_request_range, max_power_request_range, HUMIDITY_VALVE);

//LED
//temperature
led room_regolate_temperature = led(min_power_request_range, max_power_request_range, TEMPERATURE_VALVE);
//humidity
led led_room_regolate_humidity = led(min_power_request_range, max_power_request_range, HUMIDITY_VALVE);

//CONTROL LIGTH

//TIME
//if it is less than this time, the first time it is pressed it lights up at the minium_light_percentage
int turn_on_time = 400;
//is the time it takes for the LED to light up at maximum starting from 0
int maximum_light_on_time = 3000;
//ILLUMINATION PERCENTAGE RANGE
int minium_light_percentage = 5;
int max_light_percentage = 100;

button light_control = button(LIGHT_CONTROL ,min_power_request_range ,max_power_request_range);
led light_room = led(min_power_request_range, max_power_request_range, ROOM_LIGHTS);

// - CONTROL SHUTTERS

// BUTTON_ON_SHUTTERS 5
// BUTTON_DOWN_SHUTTERS 6

buttons_shutters buttons_control_shutters = buttons_shutters(BUTTON_ON_SHUTTERS ,BUTTON_DOWN_SHUTTERS);
int direction_motor_shutters;
enum states_motor_shutters {stopped, on_shutters ,down_shutters};
int shutter_opening_closing_time = 8000;
int rolling_shutter_rotation_time = 1000;

// - MOTOR SHUTTERS

motor_shutters motor_shutters_room = motor_shutters(MOTOR_ON_SHUTTERS, MOTOR_DOWN_SHUTTERS, MOTOR_SPEED ,min_power_request_range ,max_power_request_range);



void setup() {
  Serial.begin(9600);
  sensor_temperature_humidity.begin();
}

void loop() {

    // - CONTROL SHUTTERS ROOM

    //control direction
    direction_motor_shutters = buttons_control_shutters.control_rotation();
    motor_shutters_room.control_direction(direction_motor_shutters);
    //control speed
    buttons_control_shutters.control_speed(shutter_opening_closing_time ,rolling_shutter_rotation_time);
    motor_shutters_room.speed(200);

    //- CONTROL LIGHT ROOM

    // light_control.turn_on_time = turn_on_time;
    // light_control.maximum_light_on_time = maximum_light_on_time;
    // light_control.state_in = digitalRead(LIGHT_CONTROL);
    // float power_request_light_room = light_control.light_regolate(minium_light_percentage ,max_light_percentage);
    // light_room.blink(power_request_light_room);

  if (check_time_temperature_humidity.check_time()) {

    // - MEASUREMENTS IN THE ROOM
    //humidity
    float room_humidity = sensor_temperature_humidity.readHumidity();
    //temperature
    float room_temperature = sensor_temperature_humidity.readTemperature();

    // - HYSTERESIS LOOP

    // - POWER REQUEST

    //TEMPERATURE
    //fields
    // temperature_hysteresis_loop.measurement_setpoint = TEMPERATURE_SETPOINT;
    // temperature_hysteresis_loop.measurement_setpoint_range = TEMPERATURE_SETPOINT_RANGE;
    // temperature_hysteresis_loop.action_measurement = ACTION_TEMPERATURE;
    //function
    //power_request_temperature_valve = temperature_hysteresis_loop.find_power_request(room_temperature);

    //HUMIDITY
    //fields
    // humidity_hysteresis_loop.measurement_setpoint = HUMIDITY_SETPOINT;
    // humidity_hysteresis_loop.measurement_setpoint_range = HUMIDITY_SETPOINT_RANGE;
    // humidity_hysteresis_loop.action_measurement = ACTION_HUMIDITY;
    //function
    //power_request_humidity_valve = humidity_hysteresis_loop.find_power_request(room_humidity);

    //- PID

    // - HUMIDITY
    //fields
    // humidity_pid.measurement = room_humidity;
    // humidity_pid.measurement_setpoint = HUMIDITY_SETPOINT;
    // humidity_pid.measurement_setpoint_range = HUMIDITY_SETPOINT_RANGE;
    // humidity_pid.action_measurement = ACTION_HUMIDITY;

    //function
    //power_request_humidity_valve = humidity_pid.find_power_request();

    // - TEMPERATURE
    //fields
    // temperature_pid.measurement = room_temperature;
    // temperature_pid.measurement_setpoint = TEMPERATURE_SETPOINT;
    // temperature_pid.measurement_setpoint_range = TEMPERATURE_SETPOINT_RANGE;
    // temperature_pid.action_measurement = ACTION_TEMPERATURE;

    //function
    //power_request_temperature_valve = temperature_pid.find_power_request();

    // - ACTUATOR

    // - VALVE

    //humidity
    // valve_room_regolate_humidity.safety = safety_valve_room_regolate_humidity;
    // int anlgle_valve_humidity = valve_room_regolate_humidity.spin(power_request_humidity_valve);

    //temperature
    // valve_room_regolate_temperature.safety = safety_valve_room_regolate_temperature;
    // int angle_valve_temperature = valve_room_regolate_temperature.spin(power_request_temperature_valve);

    // - LED

    //temperature
    // room_regolate_temperature.blink(power_request_temperature_valve);
    // room_regolate_temperature.safety = true;

    //humidity
    // led_room_regolate_humidity.blink(power_request_humidity_valve);
    // led_room_regolate_humidity.safety = true;

    // - PID

    // Serial.print("HUMIDITY ROOM : ");
    // Serial.print(room_humidity);
    // Serial.print("\n");
    // Serial.print("POWER REQUEST HUMIDITY VALVE : ");
    // Serial.print(power_request_humidity_valve);
    // Serial.print("\n");
    // Serial.print("ANGLE VALVE HUMIDITY: ");
    // Serial.print(anlgle_valve_humidity);
    // Serial.print("\n");
    // Serial.print("TEMPERATURE ROOM : ");
    // Serial.print(room_temperature);
    // Serial.print("\n");
    // Serial.print("POWER REQUEST TEMPERATURE VALVE : ");
    // Serial.print(power_request_temperature_valve);
    // Serial.print("\n");
    // Serial.print("ANGLE VALVE TEMPERATURE: ");
    // Serial.print(angle_valve_temperature);
    // Serial.print("\n");
    // Serial.print("-----------------------------------");
    // Serial.print("\n");

    //hysteresis loop

    // Serial.print("TEMPERATURE ROOM : ");
    // Serial.print(room_temperature);
    // Serial.print("\n");
    // Serial.print("POWER REQUEST TEMPERATURE VALVE : ");
    // Serial.print(power_request_temperature_valve);
    // Serial.print("\n");
    // Serial.print("-----------------------------------");
    // Serial.print("\n");
    // Serial.print("HUMIDITY ROOM : ");
    // Serial.print(room_humidity);
    // Serial.print("\n");
    // Serial.print("POWER REQUEST HUMIDITY VALVE : ");
    // Serial.print(power_request_humidity_valve);
    // Serial.print("\n");
  }
}