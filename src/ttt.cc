#include "ttt.h"
#include <assert.h>
#include <stdio.h>

Game::Game(int size) {
    m_size = size;
    m_turn = Turn::Cross;

    m_state = (Turn*)malloc(size * size * sizeof(Turn));
    for(int i = 0; i < size * size; i++) {
        m_state[i] = Turn::None;
    }
    assert(m_state != nullptr && "Unable to allocate memory");
}

Game::~Game() {
    free(m_state);
}

void Game::m_NextTurn() {
    m_turn = m_turn == Turn::Circle ? Turn::Cross : Turn::Circle;
}

void Game::m_CheckWinner() {
    auto walk = [this](int x, int y, int dx, int dy) {
        Turn* state = this->m_state;
        int size = this->m_size;
        int cnt = 1;
        while (true) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 and nx < size and ny >= 0 and ny < size) {
                int currIndex = x * size + y;
                int nextIndex = nx * size + ny;
                if (state[currIndex] != Turn::None and state[currIndex] == state[nextIndex]) {
                    x = nx;
                    y = ny;
                    cnt++;
                }
                else {
                    break;
                }
            } else {
                break;
            }
        }
        return cnt == size;
    };

    int dx[] = {1, 1, 0, -1}, dy[] = {0, 1, 1, 1};
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < m_size; i++) {
            bool gameWon = walk(i, 0, dx[k], dy[k]);
            if (gameWon == true) {
                if(m_state[i * m_size + 0] == Turn::Circle) {
                    m_EndGame(Winner::Circle);
                } 
                else {
                    m_EndGame(Winner::Cross);
                }
            }
        }
    }

    bool draw = true;
    for (int i = 0; i < m_size * m_size; i++) {
        draw &= m_state[i] != Turn::None;
    }
    if(draw) {
        m_EndGame(Winner::Drawn);
    }
}

void Game::m_EndGame(Winner winner) {
    printf("GAME ENDED: %d ", winner);
    switch(winner) {
        case Winner::Circle: {
            printf("Circle \n");
            break;
        };
        case Winner::Cross: {
            printf("Cross \n");
            break;
        };
        default:
            printf("Draw \n");
    }
    m_winner = winner;
    m_gameOver = true;
}

void Game::Clicked(Vector2 pos) {
    if (m_gameOver) {
        return;
    }
    int cellSize = (float)GetScreenWidth() / m_size;
    int x = pos.x;
    int y = pos.y;
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            float cell_x = i * cellSize, cell_y = j * cellSize;
            int ind = i * m_size + j;
            if (x >= cell_x and x < cell_x + cellSize and y >= cell_y and
                y <= cell_y + cellSize and m_state[ind] == Turn::None) {
                m_state[ind] = m_turn;
                m_NextTurn();
            }
        }
    }
    m_CheckWinner();
}

void Game::Draw(float dt) {
    ClearBackground(m_clearColor);
    float cellSize = (float)GetScreenWidth() / m_size;
    Color lineCol = RED;
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            float cell_x = i * cellSize, cell_y = j * cellSize;
            Rectangle rec = {cell_x, cell_y, cellSize, cellSize};

            DrawRectangleLinesEx(rec, 5, lineCol);

            int ind = i * m_size + j;
            if (m_state[ind] == Turn::Circle) {
                Vector2 center = {cell_x + cellSize / 2, cell_y + cellSize / 2};
                float outerRadius = cellSize * 0.8 * 0.5;
                float innerRadius = cellSize * 0.7 * 0.5;
                DrawRing(center, innerRadius, outerRadius, 0, 360, 1, lineCol);
            } else if (m_state[ind] == Turn::Cross) {
                Vector2 pos1 = {(float)(cell_x + 0.2 * cellSize),
                                (float)(cell_y + 0.2 * cellSize)};
                Vector2 pos2 = {(float)(pos1.x + 0.6 * cellSize),
                                (float)(pos1.y + 0.6 * cellSize)};
                Vector2 pos3 = {pos2.x, pos1.y};
                Vector2 pos4 = {pos1.x, pos2.y};
                DrawLineEx(pos1, pos2, 5, lineCol);
                DrawLineEx(pos3, pos4, 5, lineCol);
            }
        }
    }
}
