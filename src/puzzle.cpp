#include "puzzle.h"
#include <raylib.h>
#include <algorithm>
#include <random>
#include <utility>
#include "constants.h"
#include "utils.h"

Puzzle::Puzzle()
    : board_(kGridSize * kGridSize), empty_index_(kGridSize * kGridSize - 1) {
  // Init board
  for (int i = 0; i < board_.size(); ++i) {
    board_[i] = i + 1;
  }
  board_[empty_index_] = 0;

  InitColor();
  ShuffleBoard();
}

// Improved shuffling to ensure puzzles are solvable
void Puzzle::ShuffleBoard() {
  std::random_device rd;
  std::mt19937 g(rd());

  // find the empty tile index
  empty_index_ = std::find(board_.begin(), board_.end(), 0) - board_.begin();

  // Make random valid moves
  const int kShuffleMoves = 1000;
  for (int i = 0; i < kShuffleMoves; ++i) {
    std::vector<int> possible_moves;
    auto [empty_x, empty_y] = GetColRow(empty_index_);

    // Check all four directions
    if (empty_y > 0) possible_moves.push_back(empty_index_ - kGridSize);  // Up
    if (empty_y < kGridSize - 1)
      possible_moves.push_back(empty_index_ + kGridSize);         // Down
    if (empty_x > 0) possible_moves.push_back(empty_index_ - 1);  // Left
    if (empty_x < kGridSize - 1)
      possible_moves.push_back(empty_index_ + 1);  // Right

    // Pick a random direction
    int moveIndex = possible_moves[rand() % possible_moves.size()];

    // Swap tiles
    std::swap(board_[empty_index_], board_[moveIndex]);
    empty_index_ = moveIndex;
  }
}

bool Puzzle::IsSolved() {
  for (int i = 0; i < board_.size() - 1; ++i) {
    if (board_[i] != i + 1) return false;
  }
  return board_.back() == 0;
}

void Puzzle::MoveTile(int mouse_x, int mouse_y) {
  TraceLog(LOG_DEBUG, "**********MoveTile**********");
  int clicked_index = (mouse_y / kTileSize) * kGridSize + (mouse_x / kTileSize);
  if (clicked_index >= kGridSize * kGridSize) return;

  auto [empty_x, empty_y] = GetColRow(empty_index_);
  auto [click_x, click_y] = GetColRow(clicked_index);
  TraceLog(LOG_DEBUG, "mouse click at %d \t(%d, %d)", board_[clicked_index],
           click_x + 1, click_y + 1);
  TraceLog(LOG_DEBUG, "empty pos \t\t(%d, %d)", empty_x + 1, empty_y + 1);

  auto SwapTile = [&](int a, int b) { std::swap(board_[a], board_[b]); };

#define DEBUG_MOVETILE                                              \
  auto [move_tile_x, move_tile_y] = GetColRow(move_index);          \
  TraceLog(LOG_DEBUG, "move tile %d: (%d, %d)", board_[move_index], \
           move_tile_x, move_tile_y);

  // Check if the clicked tile is n the same row or column as the empty tile
  // Horizontal movement (same row, same y)
  if (empty_y == click_y) {
    TraceLog(LOG_DEBUG, "horizontal");
    if (empty_x < click_x) {  // Move tiles left
      for (int x = empty_x + 1; x <= click_x; ++x) {
        int move_index = GetIndex(x, click_y);
        int prev_index = GetIndex(x - 1, click_y);
        DEBUG_MOVETILE
        SwapTile(move_index, prev_index);
      }
    } else if (empty_x > click_x) {  // Move tiles right
      for (int x = empty_x - 1; x >= click_x; --x) {
        int move_index = GetIndex(x, click_y);
        int prev_index = GetIndex(x + 1, click_y);
        DEBUG_MOVETILE
        SwapTile(move_index, prev_index);
      }
    }
    empty_index_ = clicked_index;
  }
  // Vertical movement (same column, same x)
  else if (empty_x == click_x) {
    TraceLog(LOG_DEBUG, "vertical");
    if (empty_y < click_y) {  // Move tiles up
      for (int col = empty_y + 1; col <= click_y; ++col) {
        int move_index = GetIndex(click_x, col);
        int prev_index = GetIndex(click_x, col - 1);
        DEBUG_MOVETILE
        SwapTile(move_index, prev_index);
      }
    } else if (empty_y > click_y) {  // Move tiles down
      for (int col = empty_y - 1; col >= click_y; --col) {
        int move_index = GetIndex(click_x, col);
        int prev_index = GetIndex(click_x, col + 1);
        DEBUG_MOVETILE
        SwapTile(move_index, prev_index);
      }
    }
    empty_index_ = clicked_index;
  }
#undef DEBUG_MOVETILE
}

void Puzzle::Update() {
  if (IsKeyPressed(KEY_H)) {
    show_hint_color_ = !show_hint_color_;
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
    Color color = LIGHTGRAY;
    if (show_hint_color_) color = colors_[board_[i] - 1];
    DrawRectangle(x, y, kTileSize - 5, kTileSize - 5, color);

    // Draw tile number
    std::string numStr = std::to_string(board_[i]);
    int fontSize = 40;
    int textWidth = MeasureText(numStr.c_str(), fontSize);
    DrawText(numStr.c_str(), x + (kTileSize - textWidth) / 2,
             y + (kTileSize - fontSize) / 2, fontSize, WHITE);
  }
}

void Puzzle::InitColor() {
  // clang-format off
  Color hint_colors[10] = {
      BLANK,
      Haxc("#fae4d0"),
      Haxc("#b7fce5"),
      Haxc("#b8e2fc"),
      Haxc("#d5d6fa"),
  };
  // clang-format on

  // Assign colors based on the tile's target position
  colors_.resize(kGridSize * kGridSize);
  for (int k = 1; k < kGridSize; k++) {
    for (int i = k - 1; i < kGridSize; i++) {
      colors_[GetIndex(k - 1, i)] = hint_colors[k];
      colors_[GetIndex(i, k - 1)] = hint_colors[k];
    }
  }
  colors_[kGridSize * kGridSize - 1] = BLANK;
}
