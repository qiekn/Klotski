#pragma once

#include <utility>
#include "constants.h"

inline std::pair<int, int> GetRowCol(int index) {
  return {index / kGridSize, index % kGridSize};
}
