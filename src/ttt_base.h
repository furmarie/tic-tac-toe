#pragma once
#include "raylib.h"

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

class GameBase {
public:
    virtual void Init(int, int, int) = 0;
    virtual void Draw(float) = 0;
    virtual void Clicked(Vector2) = 0;
    virtual void Reset() = 0;
    virtual void* PreReload() = 0;
    virtual void PostReload(void*) = 0;

// private:
//     Turn m_turn;
//     int m_size;
//     Turn* m_state = nullptr;
//     bool m_gameOver = false;
//     Winner m_winner;
//     Color m_clearColor = {120, 120, 120};

//     void m_CheckWinner();
//     void m_NextTurn();
//     void m_EndGame(Winner);
};

using Base_creator_t = GameBase *(*)();
