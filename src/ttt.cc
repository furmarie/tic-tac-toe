#include "ttt.h"
#include <cstring>
#include <assert.h>
#include <stdio.h>

#include <rlgl.h>

#define GLSL_VERSION 330

void Game::Init(int size, int width, int height) {
    printf("Game::Init\n");
    m_state.m_size = size;
    m_state.m_turn = Turn::Cross;

    m_state.m_state = (Turn*)malloc(size * size * sizeof(Turn));
    for (int i = 0; i < size * size; i++) {
        m_state.m_state[i] = Turn::None;
    }
    assert(m_state.m_state != nullptr && "Unable to allocate memory");

    m_clearColor = {140, 145, 120};

    // m_state.confetti = LoadShader(0, TextFormat("./resources/shaders/glsl%d/confetti.fs", GLSL_VERSION));
    // m_state.confetti_time_loc = GetShaderLocation(m_state.confetti, "time");
    // m_state.confetti_res_loc = GetShaderLocation(m_state.confetti, "resolution");

    m_state.screen = LoadRenderTexture(width, height);
}

Game::~Game() {
    UnloadShader(m_state.confetti);
    free(m_state.m_state);
}

void Game::Reset() {
    for (int i = 0; i < m_state.m_size * m_state.m_size; i++) {
        m_state.m_state[i] = Turn::None;
    }
    m_state.m_turn = Turn::Cross;
    m_state.m_gameOver = false;
}

void Game::m_NextTurn() {
    m_state.m_turn = m_state.m_turn == Turn::Circle ? Turn::Cross : Turn::Circle;
}

void Game::m_CheckWinner() {
    auto walk = [this](int x, int y, int dx, int dy) {
        Turn* state = this->m_state.m_state;
        int size = this->m_state.m_size;
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
                } else {
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
        for (int i = 0; i < m_state.m_size; i++) {
            bool gameWon = walk(i, 0, dx[k], dy[k]);
            if (gameWon == true) {
                if (m_state.m_state[i * m_state.m_size + 0] == Turn::Circle) {
                    m_EndGame(Winner::Circle);
                } else {
                    m_EndGame(Winner::Cross);
                }
            }
        }
        for (int j = 0; j < m_state.m_size; j++) {
            bool gameWon = walk(0, j, dx[k], dy[k]);
            if (gameWon == true) {
                if (m_state.m_state[0 + j] == Turn::Circle) {
                    m_EndGame(Winner::Circle);
                } else {
                    m_EndGame(Winner::Cross);
                }
            }
        }
    }

    if (!m_state.m_gameOver) {
        bool draw = true;
        for (int i = 0; i < m_state.m_size * m_state.m_size; i++) {
            draw &= m_state.m_state[i] != Turn::None;
        }
        if (draw) {
            m_EndGame(Winner::Drawn);
        }
    }
}

void Game::m_EndGame(Winner winner) {
    if (m_state.m_gameOver) {
        return;
    }
    printf("GAME ENDED: %d ", winner);
    switch (winner) {
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
    m_state.m_winner = winner;
    m_state.m_gameOver = true;
}

void Game::Clicked(Vector2 pos) {
    if (m_state.m_gameOver) {
        return;
    }
    int cellSize = (float)GetScreenWidth() / m_state.m_size;
    int x = pos.x;
    int y = pos.y;
    for (int i = 0; i < m_state.m_size; i++) {
        for (int j = 0; j < m_state.m_size; j++) {
            float cell_x = i * cellSize, cell_y = j * cellSize;
            int ind = i * m_state.m_size + j;
            if (x >= cell_x and x < cell_x + cellSize and y >= cell_y and
                y <= cell_y + cellSize and m_state.m_state[ind] == Turn::None) {
                m_state.m_state[ind] = m_state.m_turn;
                m_NextTurn();
            }
        }
    }
    m_CheckWinner();
}

void Game::Draw(float dt) {
    float cellSize = (float)GetScreenWidth() / m_state.m_size;
    Color lineCol = RED;

    if(m_state.m_gameOver and m_state.m_winner != Winner::Drawn) {
        // runTime += dt * 0.5;
        // SetShaderValue(m_state.confetti, m_state.confetti_time_loc, &runTime, SHADER_UNIFORM_FLOAT);

        // float res[2] = {(float)GetScreenWidth(), (float)GetScreenHeight()};
        // SetShaderValue(m_state.confetti, m_state.confetti_res_loc, res, SHADER_UNIFORM_VEC2);

        // // Texture2D texture = { rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
        // BeginShaderMode(m_state.confetti);
        //     // DrawTexture(texture, 5, 5, RED);
        //     DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 0});
        // EndShaderMode();
    }
    else {
        ClearBackground(m_clearColor);
    }

    for (int i = 0; i < m_state.m_size; i++) {
        for (int j = 0; j < m_state.m_size; j++) {
            float cell_x = i * cellSize, cell_y = j * cellSize;
            Rectangle rec = {cell_x, cell_y, cellSize, cellSize};

            DrawRectangleLinesEx(rec, 5, lineCol);

            int ind = i * m_state.m_size + j;
            if (m_state.m_state[ind] == Turn::Circle) {
                Vector2 center = {cell_x + cellSize / 2, cell_y + cellSize / 2};
                float outerRadius = cellSize * 0.8 * 0.5;
                float innerRadius = cellSize * 0.7 * 0.5;
                DrawRing(center, innerRadius, outerRadius, 0, 360, 1, PURPLE);
            } else if (m_state.m_state[ind] == Turn::Cross) {
                Vector2 pos1 = {(float)(cell_x + 0.2 * cellSize),
                                (float)(cell_y + 0.2 * cellSize)};
                Vector2 pos2 = {(float)(pos1.x + 0.6 * cellSize),
                                (float)(pos1.y + 0.6 * cellSize)};
                Vector2 pos3 = {pos2.x, pos1.y};
                Vector2 pos4 = {pos1.x, pos2.y};
                Color lineCol = {0xE1, 0xFF, 0x2F, 0xFF};
                DrawLineEx(pos1, pos2, 5, lineCol);
                DrawLineEx(pos3, pos4, 5, lineCol);
            }
        }
    }
}

void* Game::PreReload() {
    state* ptr = new state(m_state);
    int size = m_state.m_size;
    ptr->m_state = (Turn*)malloc(sizeof(Turn) * size * size);
    memcpy(ptr->m_state, m_state.m_state, sizeof(Turn) * size * size);
    return static_cast<void*>(ptr);
}

void Game::PostReload(void* p) {
    state* ptr = static_cast<state*>(p);
    m_state = *ptr;
    free(ptr);
}
