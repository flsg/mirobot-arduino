#ifndef Mirobot_h
#define Mirobot_h

#include "Arduino.h"
#include "lib/SerialWebSocket.h"
#include "lib/HotStepper.h"
#include "lib/CmdProcessor.h"
#include "lib/ArduinoJson/ArduinoJson.h"
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define SERIAL_BUFFER_LENGTH 180

// The steppers have a gear ratio of 1:63.7 and have 32 steps per turn. 32 x 63.7 = 2038.4
#define STEPS_PER_TURN    2048.0f

#define CIRCUMFERENCE_MM_V2  256.0f
#define WHEEL_DISTANCE_V2    120.0f

#define STEPS_PER_MM_V1      STEPS_PER_TURN / CIRCUMFERENCE_MM_V1
#define STEPS_PER_DEGREE_V1  ((WHEEL_DISTANCE_V1 * 3.1416) / 360) * STEPS_PER_MM_V1
#define STEPS_PER_MM_V2      STEPS_PER_TURN / CIRCUMFERENCE_MM_V2
#define STEPS_PER_DEGREE_V2  ((WHEEL_DISTANCE_V2 * 3.1416) / 360) * STEPS_PER_MM_V2

#define STATUS_LED 13

#define MIROBOT_SUB_VERSION "0.9MOD"

#define EEPROM_OFFSET 0
#define MAGIC_BYTE_1 0xF0
#define MAGIC_BYTE_2 0x0D
#define SETTINGS_VERSION 1

#define SERVO_PIN 3
#define SERVO_PULSES 15

#define SPEAKER_PIN 9
#define NOTE_C4  262

#define LEFT_LINE_SENSOR  A0
#define RIGHT_LINE_SENSOR A1

#define LEFT_COLLIDE_SENSOR  A3
#define RIGHT_COLLIDE_SENSOR A2

typedef enum {NONE=0, RIGHT, LEFT, BOTH} collideState_t;
typedef enum {NORMAL, RIGHT_REVERSE, RIGHT_TURN, LEFT_REVERSE, LEFT_TURN} collideStatus_t;

struct Settings {
  uint8_t      settingsVersion;
  unsigned int slackCalibration;
  float        moveCalibration;
  float        turnCalibration;
};

class Mirobot {
  public:
    Mirobot();
    void begin();
    void begin(unsigned char);
    void enableSerial();
    void forward(int distance);
    void back(int distance);
    void right(int angle);
    void left(int angle);
    void servo(int angle);
    void rgb(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
    uint8_t lineSensor(uint8_t sensor);
    void pause();
    void resume();
    void stop();
    void follow();
    int  followState();
    void collide();
    collideState_t collideState();
    void beep(int);
    boolean ready();
    void loop();
    void calibrateSlack(unsigned int);
    void calibrateMove(float);
    void calibrateTurn(float);
    char hwVersion;
    char versionStr[9];
    Settings settings;
    boolean blocking;
    boolean collideNotify;
    boolean followNotify;
  private:
  void wait();
    void waitAngle(int angle);
    void followHandler();
    void collideHandler();
    void ledHandler();
    void servoHandler(int angle);
    void autoHandler();
    void sensorNotifier();
    void checkState();
    void initSettings();
    void saveSettings();
    void checkReady();
    void version(char);
    void initCmds();
    void _version(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _ping(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _uptime(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _pause(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _resume(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _stop(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _collideState(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _collideNotify(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _followState(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _followNotify(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _slackCalibration(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _moveCalibration(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _turnCalibration(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _calibrateMove(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _calibrateTurn(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _forward(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _back(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _right(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _left(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _servo(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _rgb(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _follow(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _collide(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _beep(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    void _calibrateSlack(ArduinoJson::JsonObject &, ArduinoJson::JsonObject &);
    char lastCollideState;
    int lastFollowState;
    collideStatus_t _collideStatus;
    unsigned long lastLedChange;
    Mirobot& self() { return *this; }
    void takeUpSlack(byte, byte);
    void calibrateHandler();
    boolean paused;
    boolean following;
    boolean colliding;
    float steps_per_mm;
    float steps_per_degree;
    unsigned char servo_pulses_left;
    unsigned long next_servo_pulse;
    long beepComplete;
    boolean calibratingSlack;
    bool serialEnabled = false;
    unsigned long last_char;
    char serial_buffer[SERIAL_BUFFER_LENGTH];
    int serial_buffer_pos;
    void serialHandler();
};

#endif
