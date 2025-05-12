#pragma once

#include <utility>
#include "constants.h"

// get grid position {x, y}
inline std::pair<int, int> GetColRow(int index) {
  return {index % kGridSize, index / kGridSize};
}

inline std::pair<int, int> GetXY(int index) { return GetColRow(index); }

inline int GetIndex(int col, int row) { return row * kGridSize + col; }
