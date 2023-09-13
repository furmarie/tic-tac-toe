#include <stdio.h>
#include <stdint.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "ttt.h"

int main(void) {
    const int windowWidth = 500, windowHeight = 500;
    InitWindow(windowWidth, windowHeight, "TTT");
    SetTargetFPS(30);

    Game game(3, windowWidth, windowHeight);

    while(!WindowShouldClose()) {
        BeginDrawing();
        if(IsKeyDown(KEY_R)) {
            game.Reset();
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            game.Clicked(GetMousePosition());
        }
        game.Draw(GetFrameTime());
        EndDrawing();
    }

    CloseWindow();
	return 0;
}
