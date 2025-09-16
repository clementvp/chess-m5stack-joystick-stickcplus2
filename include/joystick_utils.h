#pragma once
#include <stdint.h>

void joystick_update();
bool joystick_isUp();
bool joystick_isDown();
bool joystick_isRight();
bool joystick_isLeft();
bool joystick_isButtonPressed();
void joystick_save_prev();

extern int8_t joystick_x_data;
extern int8_t joystick_y_data;
extern int8_t joystick_button_data;
