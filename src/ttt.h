#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

#include "ttt_base.h"

class Game: public GameBase {
public:
    Game() {
        printf("MADE GAME 6 !!! ");
    }
    ~Game();
    void Init(int size, int width, int height) override;
    void Draw(float) override;
    void Clicked(Vector2) override;
    void Reset() override;
    void* PreReload() override;
    void PostReload(void*) override;

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
        RenderTexture2D screen;
    } state;
    float runTime;
    state m_state;
    Color m_clearColor;
    void m_CheckWinner();
    void m_NextTurn();
    void m_EndGame(Winner);
};

extern "C" {
GameBase* factory(void) {
    printf("Fact 3 \n");
    return static_cast<GameBase*>(new Game);
}
}