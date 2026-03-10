#include "game.h"
#include <rand.h>
#include <string.h>

// 获取方块的幂次值 (用于显示不同图案)
uint8_t get_power(uint16_t value) OLDCALL {
    uint8_t p = 0;
    while (value > 1) {
        value >>= 1;
        p++;
    }
    return p;
}

// 初始化游戏
void game_init(Game* game) OLDCALL {
    memset(game->grid, 0, sizeof(game->grid));
    game->score = 0;
    game->game_over = 0;
    game->won = 0;
    
    // 初始生成两个方块
    game_spawn(game);
    game_spawn(game);
}

// 在空位置生成新方块 (2 或 4)
void game_spawn(Game* game) OLDCALL {
    uint8_t empty[16];
    uint8_t count = 0;
    
    // 收集所有空位
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        for (uint8_t j = 0; j < GRID_SIZE; j++) {
            if (game->grid[i][j] == 0) {
                empty[count++] = i * GRID_SIZE + j;
            }
        }
    }
    
    if (count == 0) return;
    
    // 随机选择一个空位
    uint8_t idx = empty[randw() % count];
    uint8_t row = idx / GRID_SIZE;
    uint8_t col = idx % GRID_SIZE;
    
    // 90% 概率生成 2，10% 概率生成 4
    game->grid[row][col] = (randw() % 10 < 9) ? 2 : 4;
}

// 压缩一行 (向左移动)
uint8_t compress_line(uint16_t* line, uint16_t* score_add) OLDCALL {
    uint16_t new_line[GRID_SIZE] = {0, 0, 0, 0};
    uint8_t pos = 0;
    uint8_t moved = 0;
    
    // 收集非零元素
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        if (line[i] != 0) {
            new_line[pos++] = line[i];
        }
    }
    
    // 合并相同元素
    for (uint8_t i = 0; i < pos - 1; i++) {
        if (new_line[i] == new_line[i + 1]) {
            new_line[i] *= 2;
            *score_add += new_line[i];
            new_line[i + 1] = 0;
            moved = 1;
        }
    }
    
    // 再次压缩
    pos = 0;
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        if (new_line[i] != 0) {
            line[pos++] = new_line[i];
        }
    }
    while (pos < GRID_SIZE) {
        line[pos++] = 0;
    }
    
    // 检查是否有移动
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        if (line[i] != (i < pos ? new_line[i] : 0)) {
            // 这个检查不太准确，简化处理
        }
    }
    
    return moved || *score_add > 0;
}

// 移动并合并，返回是否有变化
uint8_t game_move(Game* game, uint8_t direction) OLDCALL {
    uint8_t changed = 0;
    uint16_t score_add = 0;
    uint16_t old_grid[GRID_SIZE][GRID_SIZE];
    
    // 保存旧状态
    memcpy(old_grid, game->grid, sizeof(game->grid));
    
    switch (direction) {
        case DIR_LEFT:
            for (uint8_t i = 0; i < GRID_SIZE; i++) {
                uint16_t line[GRID_SIZE];
                uint16_t add = 0;
                memcpy(line, game->grid[i], sizeof(line));
                compress_line(line, &add);
                memcpy(game->grid[i], line, sizeof(line));
                score_add += add;
            }
            break;
            
        case DIR_RIGHT:
            for (uint8_t i = 0; i < GRID_SIZE; i++) {
                uint16_t line[GRID_SIZE];
                uint16_t add = 0;
                // 反转行
                for (uint8_t j = 0; j < GRID_SIZE; j++) {
                    line[j] = game->grid[i][GRID_SIZE - 1 - j];
                }
                compress_line(line, &add);
                // 反转回来
                for (uint8_t j = 0; j < GRID_SIZE; j++) {
                    game->grid[i][j] = line[GRID_SIZE - 1 - j];
                }
                score_add += add;
            }
            break;
            
        case DIR_UP:
            for (uint8_t j = 0; j < GRID_SIZE; j++) {
                uint16_t line[GRID_SIZE];
                uint16_t add = 0;
                // 提取列
                for (uint8_t i = 0; i < GRID_SIZE; i++) {
                    line[i] = game->grid[i][j];
                }
                compress_line(line, &add);
                // 写回列
                for (uint8_t i = 0; i < GRID_SIZE; i++) {
                    game->grid[i][j] = line[i];
                }
                score_add += add;
            }
            break;
            
        case DIR_DOWN:
            for (uint8_t j = 0; j < GRID_SIZE; j++) {
                uint16_t line[GRID_SIZE];
                uint16_t add = 0;
                // 提取列并反转
                for (uint8_t i = 0; i < GRID_SIZE; i++) {
                    line[i] = game->grid[GRID_SIZE - 1 - i][j];
                }
                compress_line(line, &add);
                // 反转回来写回
                for (uint8_t i = 0; i < GRID_SIZE; i++) {
                    game->grid[i][j] = line[GRID_SIZE - 1 - i];
                }
                score_add += add;
            }
            break;
    }
    
    game->score += score_add;
    
    // 检查是否有变化
    changed = (memcmp(old_grid, game->grid, sizeof(game->grid)) != 0);
    
    if (changed) {
        game_spawn(game);
        game->game_over = check_game_over(game);
        if (check_win(game)) {
            game->won = 1;
        }
    }
    
    return changed;
}

// 检查游戏结束
uint8_t check_game_over(Game* game) OLDCALL {
    // 检查是否有空位
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        for (uint8_t j = 0; j < GRID_SIZE; j++) {
            if (game->grid[i][j] == 0) {
                return 0;
            }
        }
    }
    
    // 检查是否有可合并的相邻方块
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        for (uint8_t j = 0; j < GRID_SIZE; j++) {
            uint16_t val = game->grid[i][j];
            // 检查右边
            if (j < GRID_SIZE - 1 && game->grid[i][j + 1] == val) {
                return 0;
            }
            // 检查下边
            if (i < GRID_SIZE - 1 && game->grid[i + 1][j] == val) {
                return 0;
            }
        }
    }
    
    return 1;
}

// 检查是否达到 2048
uint8_t check_win(Game* game) OLDCALL {
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        for (uint8_t j = 0; j < GRID_SIZE; j++) {
            if (game->grid[i][j] == 2048) {
                return 1;
            }
        }
    }
    return 0;
}
