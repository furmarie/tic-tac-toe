#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

enum class Turn {
    Cross,
    Circle,
    None = -1
};

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
