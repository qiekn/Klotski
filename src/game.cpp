#include "game.h"
#include <raylib.h>
#include "constants.h"

void Game::Run() {
  Init();
  while (!WindowShouldClose()) {
    Update();

    BeginDrawing();
    Draw();
    EndDrawing();
  }
}

void Game::Init() {
  SetTraceLogLevel(LOG_DEBUG);

  InitWindow(kScreenWidth, kScreenHeight, "game");
  SetTargetFPS(60);
}

void Game::Update() {
  // TODO: handle mouse input <2025-05-12 22:04, @qiekn> //
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse_pos = GetMousePosition();
    puzzle.MoveTile(mouse_pos.x, mouse_pos.y);
  }
}

void Game::Draw() {
  ClearBackground(RAYWHITE);
  puzzle.Draw();
}
