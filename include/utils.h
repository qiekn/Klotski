#pragma once

#include <raylib.h>
#include <utility>
#include "constants.h"

// get grid position {x, y}
inline std::pair<int, int> GetColRow(int index) {
  return {index % kTileCount, index / kTileCount};
}

inline std::pair<int, int> GetXY(int index) { return GetColRow(index); }

inline int GetIndex(int col, int row) { return row * kTileCount + col; }

Color Haxc(const std::string& color, int alpha = 255);
