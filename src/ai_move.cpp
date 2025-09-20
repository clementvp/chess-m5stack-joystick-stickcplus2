
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

void AiMove() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  static bool moveDisplayed = false;
  if (!moveDisplayed) {
    if (mcumax_is_in_checkmate(selectedColor == MCUMAX_BOARD_WHITE
                                   ? MCUMAX_BOARD_BLACK
                                   : MCUMAX_BOARD_WHITE)) {
      StickCP2.Display.drawString("AI in Checkmate!",
                                  StickCP2.Display.width() / 2,
                                  StickCP2.Display.height() / 2);
      // Attendre jusqu'à ce que le bouton soit pressé pour l'échec et mat aussi
      while (!joystick_isButtonPressed()) {
        StickCP2.update();
        joystick_update();
        delay(50);
      }
      StickCP2.Display.clear();
      currentState = GAMEOVER;
    }
    if (mcumax_is_in_check(selectedColor == MCUMAX_BOARD_WHITE
                               ? MCUMAX_BOARD_BLACK
                               : MCUMAX_BOARD_WHITE)) {
      StickCP2.Display.drawString("Ai in Check!", StickCP2.Display.width() / 2,
                                  StickCP2.Display.height() / 2);

      while (!joystick_isButtonPressed()) {
        StickCP2.update();
        joystick_update();
        delay(50);
      }
      StickCP2.Display.clear();
    }
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
    currentState = PLAYERMOVE;
    moveDisplayed = false;
  }
}
