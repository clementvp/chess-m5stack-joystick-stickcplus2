#include "inGame.h"
#include "constants.h"
#include "state.h"

extern ColorSelection selectedColor;
extern State currentState;

void inGame() {
  if (selectedColor == MCUMAX_BOARD_WHITE) {
    currentState = PLAYERMOVE;
  } else {
    currentState = AIMOVE;
  }
}
