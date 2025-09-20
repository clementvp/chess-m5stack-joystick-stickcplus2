

#include "player_move.h"
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
extern int fromRow;
extern int fromCol;
extern int toRow;
extern int toCol;
extern int mcu_nodes;
extern int mcu_depth;

extern PlayerMoveSelection currentPlayerMoveSelection;

void playerMove() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  if (mcumax_is_in_checkmate(selectedColor)) {
    StickCP2.Display.drawString("Player in Checkmate!",
                                StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
    delay(CHECK_DELAY);
    StickCP2.Display.clear();
    currentState = GAMEOVER;
    return;
  }
  if (mcumax_is_in_check(selectedColor)) {
    StickCP2.Display.drawString("Player in Check!",
                                StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
    delay(CHECK_DELAY);
    StickCP2.Display.clear();
  }
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString("Player Move", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 5);
  StickCP2.Display.setTextSize(TEXT_SIZE_MEDIUM);
  StickCP2.Display.drawString("->", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString(String(playerRow[fromRow]),
                              StickCP2.Display.width() / 2 - 60,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.drawString(String(playerCol[fromCol]),
                              StickCP2.Display.width() / 2 - 30,
                              StickCP2.Display.height() / 2);

  StickCP2.Display.drawString(String(playerRow[toRow]),
                              StickCP2.Display.width() / 2 + 30,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.drawString(String(playerCol[toCol]),
                              StickCP2.Display.width() / 2 + 60,
                              StickCP2.Display.height() / 2);

  if (currentPlayerMoveSelection == FROMROW) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 2 - 75,
                              StickCP2.Display.height() / 2 - 15, 30, 30,
                              WHITE);
  }
  if (currentPlayerMoveSelection == FROMCOL) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 2 - 45,
                              StickCP2.Display.height() / 2 - 15, 30, 30,
                              WHITE);
  }
  if (currentPlayerMoveSelection == TOROW) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 2 + 15,
                              StickCP2.Display.height() / 2 - 15, 30, 30,
                              WHITE);
  }
  if (currentPlayerMoveSelection == TOCOL) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 2 + 45,
                              StickCP2.Display.height() / 2 - 15, 30, 30,
                              WHITE);
  }

  if (joystick_isRight()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    if (currentPlayerMoveSelection == FROMROW) {
      currentPlayerMoveSelection = FROMCOL;
    } else if (currentPlayerMoveSelection == FROMCOL) {
      currentPlayerMoveSelection = TOROW;
    } else if (currentPlayerMoveSelection == TOROW) {
      currentPlayerMoveSelection = TOCOL;
    }
    StickCP2.Display.clear();
  }
  if (joystick_isLeft()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    if (currentPlayerMoveSelection == TOCOL) {
      currentPlayerMoveSelection = TOROW;
    } else if (currentPlayerMoveSelection == TOROW) {
      currentPlayerMoveSelection = FROMCOL;
    } else if (currentPlayerMoveSelection == FROMCOL) {
      currentPlayerMoveSelection = FROMROW;
    }
    StickCP2.Display.clear();
  }
  if (joystick_isUp()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    if (currentPlayerMoveSelection == FROMROW) {
      if (fromRow < 7)
        fromRow++;
    } else if (currentPlayerMoveSelection == FROMCOL) {
      if (fromCol < 7)
        fromCol++;
    } else if (currentPlayerMoveSelection == TOROW) {
      if (toRow < 7)
        toRow++;
    } else if (currentPlayerMoveSelection == TOCOL) {
      if (toCol < 7)
        toCol++;
    }
    StickCP2.Display.clear();
  }
  if (joystick_isDown()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    if (currentPlayerMoveSelection == FROMROW) {
      if (fromRow > 0)
        fromRow--;
    } else if (currentPlayerMoveSelection == FROMCOL) {
      if (fromCol > 0)
        fromCol--;
    } else if (currentPlayerMoveSelection == TOROW) {
      if (toRow > 0)
        toRow--;
    } else if (currentPlayerMoveSelection == TOCOL) {
      if (toCol > 0)
        toCol--;
    }
    StickCP2.Display.clear();
  }

  if (joystick_isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    String fromSquareStr = get_square_str(fromRow, fromCol);
    String toSquareStr = get_square_str(toRow, toCol);
    mcumax_move move = (mcumax_move){
        get_square((char *)fromSquareStr.c_str()),
        get_square((char *)toSquareStr.c_str()),
    };
    mcumax_move valid_moves[max_moves];
    uint32_t valid_moves_num =
        mcumax_search_valid_moves(valid_moves, max_moves);
    bool is_valid_move = false;
    for (uint32_t i = 0; i < valid_moves_num; i++)
      if ((valid_moves[i].from == move.from) && (valid_moves[i].to == move.to))
        is_valid_move = true;

    if (is_valid_move) {
      StickCP2.Display.clear();
      mcumax_play_move(move);
      fromRow = 0;
      fromCol = 0;
      toRow = 0;
      toCol = 0;
      currentPlayerMoveSelection = FROMROW;
      currentState = AIMOVE;
    } else {
      StickCP2.Display.clear();
      StickCP2.Display.drawString("Invalid Move", StickCP2.Display.width() / 2,
                                  StickCP2.Display.height() / 2);
      delay(INVALID_MOVE_DELAY);
      currentState = PLAYERMOVE;
      StickCP2.Display.clear();
    }
  }
}
