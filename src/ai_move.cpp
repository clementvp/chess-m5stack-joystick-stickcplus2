
#include "ai_move.h"
#include "M5StickCPlus2.h"
#include "constants.h"
#include "joystick_utils.h"
#include "move_utils.h"
#include "square_utils.h"
#include "state.h"
#include <Arduino.h>
#include <mcu-max.h>

extern State currentState;
extern int selectedColor;
extern int mcu_nodes;
extern int mcu_depth;

bool checkDisplayed = false;

void AiMove() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  static bool moveDisplayed = false;
  if (mcumax_is_in_check(selectedColor == MCUMAX_BOARD_WHITE
                             ? MCUMAX_BOARD_BLACK
                             : MCUMAX_BOARD_WHITE) &&
      !checkDisplayed) {
    StickCP2.Display.drawString("AI in check", StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
    delay(CHECK_DELAY);
    StickCP2.Display.clear();
    checkDisplayed = true;
  }
  if (!moveDisplayed) {
    StickCP2.Display.clear();
    StickCP2.Display.drawString("AI is thinking...",
                                StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 3);

    mcumax_move move = mcumax_search_best_move(mcu_nodes, mcu_depth);
    if (move.from == MCUMAX_SQUARE_INVALID) {
      currentState = GAMEOVER;
      return;
    }
    mcumax_play_move(move);
    StickCP2.Display.clear();
    print_move(move);
    moveDisplayed = true;
  }
  if (joystick_isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    moveDisplayed = false;
    checkDisplayed = false;
    currentState = PLAYERMOVE;
  }
}
