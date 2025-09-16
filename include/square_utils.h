#pragma once
#include "constants.h"
#include <Arduino.h>
#include <mcu-max.h>
#include <stdint.h>

mcumax_square get_square(char *s);
String get_square_str(int row, int col);