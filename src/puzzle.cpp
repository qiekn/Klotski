#include "puzzle.h"
#include <raylib.h>
#include <algorithm>
#include <random>
#include <utility>
#include "constants.h"
#include "utils.h"

Puzzle::Puzzle()
    : board_(kTileCount * kTileCount),
      empty_index_(kTileCount * kTileCount - 1) {
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
    if (empty_y > 0) possible_moves.push_back(empty_index_ - kTileCount);  // Up
    if (empty_y < kTileCount - 1)
      possible_moves.push_back(empty_index_ + kTileCount);        // Down
    if (empty_x > 0) possible_moves.push_back(empty_index_ - 1);  // Left
    if (empty_x < kTileCount - 1)
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
  int clicked_index =
      (mouse_y / kTileSize) * kTileCount + (mouse_x / kTileSize);
  if (clicked_index >= kTileCount * kTileCount) return;

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
  video_.Update();

  // hold <tab> to show hint
  if (IsKeyDown(KEY_TAB)) {
    show_hint_ = true;
  } else {
    show_hint_ = false;
  }
  // press <h> to toggle color hint
  if (IsKeyPressed(KEY_H)) {
    show_hint_color_ = !show_hint_color_;
  }
}

void Puzzle::Draw() {
  for (int i = 0; i < board_.size(); ++i) {
    if (board_[i] == 0) continue;  // Skip empty tile

    int row = i / kTileCount;
    int col = i % kTileCount;
    int x = col * kTileSize;
    int y = row * kTileSize;

    // Draw Video
    if (draw_video_) {
      video_.Draw(board_[i], x, y);
    }

    if (show_hint_) {
      // Draw tile rectangle background
      Color color = Haxc("#f5f5f5", 150);  // default color
      if (show_hint_color_) color = colors_[board_[i] - 1];
      DrawRectangle(x, y, kTileSize - kBorder, kTileSize - kBorder, color);

      // Draw tile number
      std::string numStr = std::to_string(board_[i]);
      int fontSize = 40;
      int textWidth = MeasureText(numStr.c_str(), fontSize);
      DrawText(numStr.c_str(), x + (kTileSize - textWidth) / 2,
               y + (kTileSize - fontSize) / 2, fontSize, WHITE);
    }
  }
}

void Puzzle::InitColor() {
  // clang-format off
  Color hint_colors[10] = {
      BLANK,
      Haxc("#fae4d0", 150),
      Haxc("#b7fce5", 150),
      Haxc("#b8e2fc", 150),
      Haxc("#d5d6fa", 150),
  };
  // clang-format on

  // Assign colors based on the tile's target position
  colors_.resize(kTileCount * kTileCount);
  for (int k = 1; k < kTileCount; k++) {
    for (int i = k - 1; i < kTileCount; i++) {
      colors_[GetIndex(k - 1, i)] = hint_colors[k];
      colors_[GetIndex(i, k - 1)] = hint_colors[k];
    }
  }
  colors_[kTileCount * kTileCount - 1] = BLANK;
}
