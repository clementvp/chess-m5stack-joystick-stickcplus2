#include "joystick_utils.h"
#include "constants.h"
#include <Wire.h>

#define JOY_ADDR 0x38

int8_t joystick_x_data = 0, joystick_y_data = 0, joystick_button_data = 1;
static int8_t prev_x = 0, prev_y = 0, prev_button = 1;

void joystick_update() {
  Wire.beginTransmission(JOY_ADDR);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(JOY_ADDR, 3);

  if (Wire.available()) {
    joystick_x_data = Wire.read();
    joystick_y_data = Wire.read();
    joystick_button_data = Wire.read();
  }
}

bool joystick_isUp() {
  return (joystick_x_data < -JOY_THRESHOLD && prev_x >= -JOY_THRESHOLD);
}
bool joystick_isDown() {
  return (joystick_x_data > JOY_THRESHOLD && prev_x <= JOY_THRESHOLD);
}
bool joystick_isRight() {
  return (joystick_y_data > JOY_THRESHOLD && prev_y <= JOY_THRESHOLD);
}
bool joystick_isLeft() {
  return (joystick_y_data < -JOY_THRESHOLD && prev_y >= -JOY_THRESHOLD);
}
bool joystick_isButtonPressed() {
  return (joystick_button_data == 0 && prev_button == 1);
}

void joystick_save_prev() {
  prev_x = joystick_x_data;
  prev_y = joystick_y_data;
  prev_button = joystick_button_data;
}
