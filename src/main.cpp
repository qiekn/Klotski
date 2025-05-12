#include <raylib.h>

const int kScreenWidth = 800;
const int kSreenHeight = 450;

int main() {

  SetTraceLogLevel(LOG_WARNING);

  InitWindow(kScreenWidth, kSreenHeight, "game");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Update

    // Draw
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
