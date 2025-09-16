#include "move_utils.h"

const int MOVE_STRING_OFFSET = 60;

void print_move(mcumax_move move) {
  String fromStr = String((char)('a' + (move.from & 0x07))) +
                   String((char)('1' + 7 - ((move.from & 0x70) >> 4)));
  String toStr = String((char)('a' + (move.to & 0x07))) +
                 String((char)('1' + 7 - ((move.to & 0x70) >> 4)));
  StickCP2.Display.drawString("Ai move:", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 5);
  StickCP2.Display.drawString("->", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.drawString(fromStr,
                              StickCP2.Display.width() / 2 - MOVE_STRING_OFFSET,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.drawString(toStr,
                              StickCP2.Display.width() / 2 + MOVE_STRING_OFFSET,
                              StickCP2.Display.height() / 2);
}