#pragma once

#include <utility>
#include "constants.h"

// get grid position {x, y}
inline std::pair<int, int> GetColRow(int index) {
  return {index % kGridSize, index / kGridSize};
}

inline int GetIndex(int col, int row) { return row * kGridSize + col; }
