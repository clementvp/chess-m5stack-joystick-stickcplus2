#include "inGame.h"
#include "constants.h"
#include "mcu-max.h"
#include "state.h"

extern int selectedColor;
extern State currentState;

void inGame() {
  if (selectedColor == MCUMAX_BOARD_WHITE) {
    currentState = PLAYERMOVE;
  } else {
    currentState = AIMOVE;
  }
}
