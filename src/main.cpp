
#include "M5StickCPlus2.h"
#include "Wire.h"
#include "ai_move.h"
#include "chooseColor.h"
#include "chooseDifficulty.h"
#include "constants.h"
#include "gameover.h"
#include "inGame.h"
#include "initGame.h"
#include "joystick_utils.h"
#include "player_move.h"
#include "square_utils.h"
#include "state.h"
#include <Arduino.h>

int currentIndexDifficulty = 0;
int mcu_nodes = difficulty_levels[0].node_max;
int mcu_depth = difficulty_levels[0].depth_max;
State currentState = INIT;
int selectedColor = MCUMAX_BOARD_WHITE;
PlayerMoveSelection currentPlayerMoveSelection = FROMROW;
int fromRow = 0;
int fromCol = 0;
int toRow = 0;
int toCol = 0;
mcumax_move move;

void setup() {
  auto cfg = M5.config();
  StickCP2.begin(cfg);
  Wire.begin(0, 26, 100000UL);
  StickCP2.Display.setRotation(3);
  StickCP2.Display.setTextColor(WHITE);
  StickCP2.Display.setTextDatum(middle_center);
}

void loop() {
  StickCP2.update();
  joystick_update();

  if (StickCP2.BtnA.wasDoubleClicked()) {
    ESP.restart();
  }

  switch (currentState) {
  case INIT:
    initGame();
    break;
  case CHOOSECOLOR:
    chooseColor();
    break;
  case CHOOSEDIFFICULTY:
    chooseDifficulty();
    break;
  case INGAME:
    inGame();
    break;
  case PLAYERMOVE:
    playerMove();
    break;
  case AIMOVE:
    AiMove();
    break;
  case GAMEOVER:
    gameOver();
    break;
  }

  joystick_save_prev();
}
