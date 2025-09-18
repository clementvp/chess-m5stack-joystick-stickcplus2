#pragma once
#include <stdint.h>

#define JOY_ADDR 0x38
const int JOY_THRESHOLD = 80;

const int TONE_FREQ = 8000;
const int TONE_DURATION = 20;
const float TEXT_SIZE_SMALL = 1.0;
const float TEXT_SIZE_MEDIUM = 1.5;
const float TEXT_SIZE_LARGE = 2.0;
const int INVALID_MOVE_DELAY = 1000;
const int CHECK_DELAY = 1500;

typedef struct {
  uint8_t level;
  uint32_t node_max;
  uint32_t depth_max;
} difficulty_config_t;

const difficulty_config_t difficulty_levels[15] = {
    {1, 300, 2},    {2, 600, 2},    {3, 1000, 2},   {4, 1500, 3},
    {5, 2500, 3},   {6, 4000, 3},   {7, 6000, 4},   {8, 8500, 4},
    {9, 12000, 4},  {10, 16000, 5}, {11, 22000, 5}, {12, 30000, 5},
    {13, 40000, 6}, {14, 55000, 6}, {15, 75000, 6}};

extern const char *playerRow[8];
extern const char *playerCol[8];

const int max_moves = 181;