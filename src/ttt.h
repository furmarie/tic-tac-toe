#include <raylib.h>
#include <stdlib.h>

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
    Game(int size);
    ~Game();
    void Draw(float);
    void Clicked(Vector2);

private:
    Turn m_turn;
    int m_size;
    Turn* m_state = nullptr;
    bool m_gameOver = false;
    Winner m_winner;
    Color m_clearColor = {120, 120, 120};

    void m_CheckWinner();
    void m_NextTurn();
    void m_EndGame(Winner);
};
