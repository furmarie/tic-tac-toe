#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "ttt.h"

#include <rlgl.h>

#define GLSL_VERSION 330

typedef enum Turn {
    Turn_Cross,
    Turn_Circle,
    Turn_None = -1
} Turn;

typedef enum Winner {
    Winner_Cross,
    Winner_Circle,
    Winner_Drawn
} Winner;

typedef struct {
    int m_size;
    float runTime;
    bool m_gameOver;
    enum Turn m_turn;
    enum Turn* m_state;
    Winner m_winner;

    int win_start, win_end;

    Shader confetti;
    int confetti_res_loc;
    int confetti_time_loc;
} ttt_state;

ttt_state* m_state = NULL;

#define ind_to_coord(ind, x, y)        \
    int x = (ind) / (m_state->m_size); \
    int y = (ind) % (m_state->m_size);

#define coord_to_ind(x, y) ((x) * m_state->m_size + (y))

// Returns the center of cell with top left coords (x, y) and cell size cellSize
Vector2 cell_center(int x, int y, int cellSize) {
    return CLITERAL(Vector2) { x + cellSize / 2, y + cellSize / 2 };
}

void ttt_init(int size, int width, int height) {
    m_state = (ttt_state*)malloc(sizeof(*m_state));
    memset(m_state, 0, sizeof(*m_state));
    m_state->m_size = size;
    m_state->m_turn = Turn_Cross;

    m_state->m_state = (Turn*)malloc(size * size * sizeof(Turn));
    for (int i = 0; i < size * size; i++) {
        m_state->m_state[i] = Turn_None;
    }
    assert(m_state->m_state != NULL && "Unable to allocate memory");

    m_state->confetti = LoadShader(0, TextFormat("./resources/shaders/glsl%d/confetti.fs", GLSL_VERSION));
    m_state->confetti_time_loc = GetShaderLocation(m_state->confetti, "time");
    m_state->confetti_res_loc = GetShaderLocation(m_state->confetti, "resolution");
}

void ttt_reset() {
    m_state->runTime = 0;
    for (int i = 0; i < m_state->m_size * m_state->m_size; i++) {
        m_state->m_state[i] = Turn_None;
    }
    m_state->m_turn = Turn_Cross;
    m_state->m_gameOver = false;
}

bool walk(int x, int y, int dx, int dy) {
    Turn* state = m_state->m_state;
    int size = m_state->m_size;
    int cnt = 1;
    while (true) {
        int nx = x + dx, ny = y + dy;
        if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
            int currIndex = x * size + y;
            int nextIndex = nx * size + ny;
            if (state[currIndex] != Turn_None && state[currIndex] == state[nextIndex]) {
                x = nx;
                y = ny;
                cnt++;
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
    return cnt == size;
};

void ttt_end_game(Winner winner) {
    if (m_state->m_gameOver) {
        return;
    }
    printf("GAME ENDED: %d ", winner);
    switch (winner) {
    case Winner_Circle: {
        printf("Circle \n");
        break;
    };
    case Winner_Cross: {
        printf("Cross \n");
        break;
    };
    default:
        printf("Draw \n");
    }
    m_state->m_winner = winner;
    m_state->m_gameOver = true;
    m_state->runTime = 0;
}

void ttt_check_winner() {
    int dx[ ] = { 1, 1, 0, -1 }, dy[ ] = { 0, 1, 1, 1 };
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < m_state->m_size; i++) {
            bool gameWon = walk(i, 0, dx[k], dy[k]);
            if (gameWon == true) {
                int ind = coord_to_ind(i, 0);
                m_state->win_start = ind;
                m_state->win_end = coord_to_ind(i + dx[k] * 2, dy[k] * 2);
                if (m_state->m_state[ind] == Turn_Circle) {
                    ttt_end_game(Winner_Circle);
                }
                else {
                    ttt_end_game(Winner_Cross);
                }
            }
            gameWon = walk(0, i, dx[k], dy[k]);
            if (gameWon == true) {
                int ind = coord_to_ind(0, i);
                m_state->win_start = ind;
                m_state->win_end = coord_to_ind(dx[k] * 2, i + dy[k] * 2);
                if (m_state->m_state[ind] == Turn_Circle) {
                    ttt_end_game(Winner_Circle);
                }
                else {
                    ttt_end_game(Winner_Cross);
                }
            }
        }
    }

    if (!m_state->m_gameOver) {
        bool draw = true;
        for (int i = 0; i < m_state->m_size * m_state->m_size; i++) {
            draw &= m_state->m_state[i] != Turn_None;
        }
        if (draw) {
            ttt_end_game(Winner_Drawn);
        }
    }
}

void ttt_clicked(Vector2 pos) {
    if (m_state->m_gameOver) {
        return;
    }
    int cellSize = (float)GetScreenWidth() / m_state->m_size;
    int x = pos.x;
    int y = pos.y;
    for (int i = 0; i < m_state->m_size; i++) {
        for (int j = 0; j < m_state->m_size; j++) {
            float cell_x = i * cellSize, cell_y = j * cellSize;
            int ind = i * m_state->m_size + j;
            if (x >= cell_x && x < cell_x + cellSize && y >= cell_y &&
                y <= cell_y + cellSize && m_state->m_state[ind] == Turn_None) {
                m_state->m_state[ind] = m_state->m_turn;
                m_state->m_turn = m_state->m_turn == Turn_Circle ? Turn_Cross : Turn_Circle;
            }
        }
    }
    ttt_check_winner();
}

void ttt_draw(float dt) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ttt_clicked(GetMousePosition());
    }

    float cellSize = (float)GetScreenWidth() / m_state->m_size;
    Color lineCol = PINK;

    Color clear_color = { 120, 130, 130, 255 };

    if (m_state->m_gameOver && m_state->m_winner != Winner_Drawn) {
        m_state->runTime += dt * 0.5;
        SetShaderValue(m_state->confetti, m_state->confetti_time_loc, &m_state->runTime, SHADER_UNIFORM_FLOAT);

        float res[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
        SetShaderValue(m_state->confetti, m_state->confetti_res_loc, res, SHADER_UNIFORM_VEC2);

        // Texture2D texture = { rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
        BeginShaderMode(m_state->confetti);
        // DrawTexture(texture, 5, 5, RED);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), CLITERAL(Color){0, 0, 0, 0});
        EndShaderMode();
    }
    else {
        ClearBackground(clear_color);
    }
    for (int i = 0; i < m_state->m_size; i++) {
        for (int j = 0; j < m_state->m_size; j++) {
            float cell_x = i * cellSize, cell_y = j * cellSize;
            Rectangle rec = { cell_x, cell_y, cellSize, cellSize };

            DrawRectangleLinesEx(rec, 5, lineCol);

            int ind = coord_to_ind(i, j);
            if (m_state->m_state[ind] == Turn_Circle) {
                Vector2 center = cell_center(cell_x, cell_y, cellSize);
                float outerRadius = cellSize * 0.8 * 0.5;
                float innerRadius = cellSize * 0.7 * 0.5;
                DrawRing(center, innerRadius, outerRadius, 0, 360, 1, PURPLE);
            }
            else if (m_state->m_state[ind] == Turn_Cross) {
                Vector2 pos1 = { (float)(cell_x + 0.2 * cellSize),
                                (float)(cell_y + 0.2 * cellSize) };
                Vector2 pos2 = { (float)(pos1.x + 0.6 * cellSize),
                                (float)(pos1.y + 0.6 * cellSize) };
                Vector2 pos3 = { pos2.x, pos1.y };
                Vector2 pos4 = { pos1.x, pos2.y };
                Color lineCol = { 0xE1, 0xFF, 0x2F, 0xFF };
                DrawLineEx(pos1, pos2, 5, lineCol);
                DrawLineEx(pos3, pos4, 5, lineCol);
            }
        }
    }

    if (m_state->m_gameOver && m_state->m_winner != Winner_Drawn) {
        int start_i = m_state->win_start, end_i = m_state->win_end;
        ind_to_coord(start_i, start_x, start_y);
        ind_to_coord(end_i, end_x, end_y);
        Vector2 start_pos = cell_center(start_x * cellSize, start_y * cellSize, cellSize);
        Vector2 end_pos = cell_center(end_x * cellSize, end_y * cellSize, cellSize);
        DrawLineEx(start_pos, end_pos, 10, WHITE);
    }
}

void* ttt_pre_reload() {
    UnloadShader(m_state->confetti);
    ttt_state* ptr = (ttt_state*)malloc(sizeof(*m_state));
    memcpy(ptr, m_state, sizeof(*m_state));
    int sz = m_state->m_size;
    ptr->m_state = (Turn*)malloc(sizeof(Turn*) * sz * sz);
    memcpy(ptr->m_state, m_state->m_state, sizeof(Turn*) * sz * sz);
    free(m_state->m_state);
    free(m_state);
    return ptr;
}

void ttt_post_reload(void* ptr) {
    m_state = (ttt_state*)malloc(sizeof(*m_state));
    memcpy(m_state, ptr, sizeof(*m_state));
    m_state->confetti = LoadShader(0, TextFormat("./resources/shaders/glsl%d/confetti.fs", GLSL_VERSION));
    m_state->confetti_time_loc = GetShaderLocation(m_state->confetti, "time");
    m_state->confetti_res_loc = GetShaderLocation(m_state->confetti, "resolution");
    free(ptr);
}
