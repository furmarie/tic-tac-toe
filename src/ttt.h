#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

#define LIST_OF_FUNCS                   \
    FUNC(ttt_init, void, int, int, int) \
    FUNC(ttt_pre_reload, void*, void)  \
    FUNC(ttt_post_reload, void, void*) \
    FUNC(ttt_draw, void, float) \
    FUNC(ttt_reset, void, void)

<<<<<<< HEAD
enum class Winner {
    Cross,
    Circle, 
    Drawn
};

class Game {
public:

    Game(int size, int width, int height);
    ~Game();
    void Draw(float);
    void Clicked(Vector2);
    void Reset();
    void* PreReload();
    void PostReload(void*);

private:
    typedef struct {
        int m_size;
        bool m_gameOver = false;
        Turn m_turn;
        Turn* m_state = nullptr;
        Winner m_winner;
        Shader confetti;
        int confetti_res_loc;
        int confetti_time_loc;
    } state;
    float runTime;
    state m_state;
    Color m_clearColor;
    void m_CheckWinner();
    void m_NextTurn();
    void m_EndGame(Winner);
};
=======
#define FUNC(func, ret, ...) typedef ret func##_t(__VA_ARGS__);
LIST_OF_FUNCS
#undef FUNC
>>>>>>> hot-reload-attempt
