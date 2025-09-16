#pragma once
#include <stdint.h>

// Adresse du joystick et seuil
#define JOY_ADDR 0x38
const int JOY_THRESHOLD = 50;

// Affichage et son
const int TONE_FREQ = 8000;
const int TONE_DURATION = 20;
const float TEXT_SIZE_SMALL = 1.0;
const float TEXT_SIZE_MEDIUM = 1.5;
const float TEXT_SIZE_LARGE = 2.0;
const int INVALID_MOVE_DELAY = 1000;
const int CHECK_DELAY = 1500;

// Structure de configuration de la difficulté
typedef struct {
  uint8_t level;      // Niveau 1-10
  uint32_t node_max;  // Nombre max de nœuds
  uint32_t depth_max; // Profondeur max
} difficulty_config_t;

const difficulty_config_t difficulty_levels[15] = {
    {1, 300, 2},    // ~400-500 ELO
    {2, 600, 2},    // ~500-650 ELO
    {3, 1000, 2},   // ~650-800 ELO
    {4, 1500, 3},   // ~800-950 ELO
    {5, 2500, 3},   // ~950-1100 ELO
    {6, 4000, 3},   // ~1100-1250 ELO
    {7, 6000, 4},   // ~1250-1400 ELO
    {8, 8500, 4},   // ~1400-1550 ELO
    {9, 12000, 4},  // ~1550-1700 ELO
    {10, 16000, 5}, // ~1700-1850 ELO
    {11, 22000, 5}, // ~1850-2000 ELO
    {12, 30000, 5}, // ~2000-2150 ELO
    {13, 40000, 6}, // ~2150-2300 ELO
    {14, 55000, 6}, // ~2300-2450 ELO
    {15, 75000, 6}  // ~2450-2600 ELO
};

extern const char *playerRow[8];
extern const char *playerCol[8];

const int max_moves = 181;