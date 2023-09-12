#include <stdio.h>
#include <stdint.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "ttt.h"

int main(void) {
    InitWindow(500, 500, "TTT");
    SetTargetFPS(30);

    Game game(3);

    while(!WindowShouldClose()) {
        BeginDrawing();
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            game.Clicked(GetMousePosition());
        }
        game.Draw(GetTime());
        EndDrawing();
    }

    CloseWindow();
	return 0;
}
