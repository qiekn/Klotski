#include "puzzle.h"
#include <raylib.h>
#include <algorithm>
#include <random>
#include "constants.h"
#include "utils.h"

Puzzle::Puzzle()
    : board_(kGridSize * kGridSize), empty_index_(kGridSize * kGridSize - 1) {
  for (int i = 0; i < board_.size(); ++i) {
    board_[i] = i + 1;
  }
  board_[empty_index_] = 0;
  ShuffleBoard();
}

void Puzzle::ShuffleBoard() {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(board_.begin(), board_.end(), g);

  // find the empty tile index
  empty_index_ = std::find(board_.begin(), board_.end(), 0) - board_.begin();
}

bool Puzzle::IsSolved() {
  for (int i = 0; i < board_.size() - 1; ++i) {
    if (board_[i] != i + 1) return false;
  }
  return board_.back() == 0;
}

void Puzzle::MoveTile(int mouse_x, int mouse_y) {
  int clicked_index = (mouse_y / kTileSize) * kGridSize + (mouse_x / kTileSize);

  TraceLog(LOG_DEBUG, "clicked_index = %d", clicked_index);
  auto [empty_row, empty_col] = GetRowCol(empty_index_);
  auto [clicked_row, clicked_col] = GetRowCol(clicked_index);
  TraceLog(LOG_DEBUG, "mouse click at %d %d", clicked_row, clicked_col);

  // Check if the clicked tile is adjacent to the empty tile
  if ((std::abs(empty_row - clicked_row) == 1 && empty_col == clicked_col) ||
      (std::abs(empty_col - clicked_col) == 1 && empty_row == clicked_row)) {
    // Swap tiles
    std::swap(board_[empty_index_], board_[clicked_index]);
    empty_index_ = clicked_index;
  }
}

void Puzzle::Draw() {
  for (int i = 0; i < board_.size(); ++i) {
    if (board_[i] == 0) continue;  // Skip empty tile

    int row = i / kGridSize;
    int col = i % kGridSize;
    int x = col * kTileSize;
    int y = row * kTileSize;

    // Draw tile rectangle
    DrawRectangle(x, y, kTileSize - 5, kTileSize - 5, LIGHTGRAY);

    // Draw tile number
    std::string numStr = std::to_string(board_[i]);
    int fontSize = 40;
    int textWidth = MeasureText(numStr.c_str(), fontSize);
    DrawText(numStr.c_str(), x + (kTileSize - textWidth) / 2,
             y + (kTileSize - fontSize) / 2, fontSize, DARKGRAY);
  }
}
