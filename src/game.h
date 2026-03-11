#ifndef GAME_H
#define GAME_H

#include <gbdk/platform.h>
#include <stdint.h>

#define GRID_SIZE 4

// 游戏状态
typedef enum {
    STATE_LOGO,
    STATE_PLAYING,
    STATE_GAMEOVER
} GameState;

// 游戏数据结构
typedef struct {
    uint16_t grid[GRID_SIZE][GRID_SIZE];  // 4x4 网格，存储 2 的幂次
    uint16_t score;
    uint8_t game_over;
    uint8_t won;
    GameState state;
} Game;

// 方向定义
#define DIR_UP    0
#define DIR_DOWN  1
#define DIR_LEFT  2
#define DIR_RIGHT 3

// 函数声明
void game_init(Game* game) OLDCALL;
uint8_t game_move(Game* game, uint8_t direction) OLDCALL;
void game_spawn(Game* game) OLDCALL;
uint8_t check_game_over(Game* game) OLDCALL;
uint8_t check_win(Game* game) OLDCALL;

#endif
