#include "square_utils.h"
mcumax_square get_square(char *s) {
  mcumax_square rank = s[0] - 'a';
  if (rank > 7)
    return MCUMAX_SQUARE_INVALID;

  mcumax_square file = '8' - s[1];
  if (file > 7)
    return MCUMAX_SQUARE_INVALID;

  return 0x10 * file + rank;
}

String get_square_str(int row, int col) {
  String square = "";
  square += playerRow[row];
  square += playerCol[col];
  return square;
}