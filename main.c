#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <raylib.h>
#include <stdint.h>

#define HEX_TO_COLOR(hex) *(Color*)(uint32_t[1]){ hex }

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SCALE 2

#define WIDTH WINDOW_WIDTH / SCALE
#define HEIGHT WINDOW_HEIGHT / SCALE

float random_float() {
    return (float) rand() / (float) RAND_MAX;
}

void allocate_board(bool*** board, size_t width, size_t height) {
    (*board) = calloc(height, sizeof(bool*));

    for (size_t i = 0; i < height; i++) {
        (*board)[i] = calloc(width, sizeof(bool));
    }
}

void free_board(bool*** board, size_t height) {
    for (size_t i = 0; i < height; i++) {
        free((*board)[i]);
    }
    
    free(*board);
}

void populate_board(bool*** board, float rate, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            (*board)[y][x] = random_float() < rate;
        }
    }
}

void draw_board(bool*** board, int width, int height, int scale) {
    for (int y = 0; y < height; y+=scale) {
        for (int x = 0; x < width; x+=scale) {
            if ((*board)[y/scale][x/scale]) {
                for (int i = 0; i < scale; i++) {
                    for (int j = 0; j < scale; j++) {
                        DrawPixel(x+i, y+j, WHITE);
                    }
                }
            }
        }
    }
}

char get_direction_val(bool*** board, int x, int y, int horizontal, int vertical, int width, int height) {
    int val_x = x + horizontal < width ? x + horizontal : 0;
    val_x = val_x == -1 ? width - 1 : val_x;

    int val_y = y + vertical < height ? y + vertical : 0;
    val_y = val_y == -1 ? height - 1 : val_y;

    return (*board)[val_y][val_x];
}

int count_neighbors(bool*** board, int x, int y, int width, int height) {
    int directions[] = {-1, 0, 1};
    int neighbors = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (directions[i] == 0 && directions[j] == 0) {
                continue;
            }

            if (get_direction_val(board, x, y, directions[i], directions[j], width, height)) {
                neighbors++;
            }
        }
    }

    return neighbors;
}

void game_of_life_step(bool*** board, int width, int height) {
    bool** alive_states = calloc(height, sizeof(bool*));
    for (int i = 0; i < height; i++) {
        alive_states[i] = calloc(width, sizeof(bool));
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int neighbor_count = count_neighbors(board, x, y, width, height);

            if ((*board)[y][x]) {
                alive_states[y][x] = neighbor_count >= 2 && neighbor_count <= 3;
            } else {
                alive_states[y][x] = neighbor_count == 3;
            }
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            (*board)[y][x] = alive_states[y][x];
        }
    }

    for (int i = 0; i < height; i++) {
        free(alive_states[i]);
    }
    free(alive_states);
}

void put_glider(bool*** board) {
    (*board)[1][0] = 1;
    (*board)[2][1] = 1;
    (*board)[0][2] = 1;
    (*board)[1][2] = 1;
    (*board)[2][2] = 1;
}

int main(void) {
    srand(time(NULL));

    bool** board = NULL;
    allocate_board(&board, WIDTH, HEIGHT);
    populate_board(&board, 0.3f, WIDTH, HEIGHT);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game of Life");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(HEX_TO_COLOR(0xFF222222));

        draw_board(&board, WINDOW_WIDTH, WINDOW_HEIGHT, SCALE);
        game_of_life_step(&board, WIDTH, HEIGHT);

        EndDrawing();
    }
    
    CloseWindow();
    free_board(&board, HEIGHT);

    return 0;
}