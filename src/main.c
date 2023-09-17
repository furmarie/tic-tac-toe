#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#include "hot_reload.h"

int main(void) {
    if(!reload_libplug()) {
        return 1;
    }
    InitWindow(500, 500, "TTT");
    SetTargetFPS(30);

    ttt_init(3, 500, 500);

    while(!WindowShouldClose()) {
        BeginDrawing();
        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)) {
            void* state = ttt_pre_reload();
            if(!reload_libplug()) {
                return 1;
            }
            ttt_post_reload(state);
        }
        else if(IsKeyPressed(KEY_R)) {
            ttt_reset();
        }
        ttt_draw(GetFrameTime());
        EndDrawing();
    }

    CloseWindow();
	return 0;
}
