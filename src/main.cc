#include <stdio.h>
#include <stdint.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "ttt_base.h"


#include "hot_reload.h"

int main(void) {
    InitWindow(500, 500, "TTT");
    SetTargetFPS(30);

    GameBase* game;
    reload_libplug(game);
    printf("HEREREREE\n");
    game->Init(3, 500, 500);


    while(!WindowShouldClose()) {
        BeginDrawing();
        if(IsKeyDown(KEY_R)) {
            void* state = game->PreReload();
            delete game;
            if(!reload_libplug(game)) {
                return 1;
            }
            game->PostReload(state);
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            game->Clicked(GetMousePosition());
        }
        game->Draw(GetFrameTime());
        EndDrawing();
    }

    CloseWindow();
	return 0;
}
