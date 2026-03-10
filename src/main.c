// 2048 Game for Game Boy
#include <stdio.h>
#include <rand.h>
#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
#include "game.h"

// 打印数字，固定 3 字符宽度，右对齐
void print_num3(uint16_t n) {
    if (n >= 100) {
        putchar('0' + (n / 100));
        putchar('0' + ((n % 100) / 10));
        putchar('0' + (n % 10));
    } else if (n >= 10) {
        putchar(' ');
        putchar('0' + (n / 10));
        putchar('0' + (n % 10));
    } else if (n > 0) {
        putchar(' ');
        putchar(' ');
        putchar('0' + n);
    } else {
        putchar(' ');
        putchar('.');
        putchar(' ');
    }
}

void draw_grid(Game* game) {
    uint8_t i, j;
    
    cls();
    
    gotoxy(6, 0);
    printf("2048 GAME");
    
    for (i = 0; i < 4; i++) {
        gotoxy(0, 2 + i * 2);
        printf("+---+---+---+---+");
        gotoxy(0, 3 + i * 2);
        for (j = 0; j < 4; j++) {
            printf("|");
            print_num3(game->grid[i][j]);
        }
        printf("|");
    }
    
    gotoxy(0, 10);
    printf("+---+---+---+---+");
    
    gotoxy(0, 11);
    printf("Score: ");
    print_num3(game->score);
    
    gotoxy(0, 13);
    printf("Arrows:Move");
    gotoxy(0, 14);
    printf("A:Reset");
    
    if (game->won) {
        gotoxy(0, 16);
        printf("WIN! 2048!");
    } else if (game->game_over) {
        gotoxy(0, 16);
        printf("GAME OVER!");
    }
}

void main(void) {
    Game game;
    uint8_t last_keys = 0;
    uint8_t keys;
    
    initrand(sys_time);
    game_init(&game);
    draw_grid(&game);
    
    while (1) {
        keys = joypad();
        
        if (keys && !last_keys) {
            // A 键随时可以重置
            if (keys & J_A) {
                game_init(&game);
                draw_grid(&game);
            } else {
                uint8_t moved = 0;
                
                if (keys & J_UP) moved = game_move(&game, DIR_UP);
                else if (keys & J_DOWN) moved = game_move(&game, DIR_DOWN);
                else if (keys & J_LEFT) moved = game_move(&game, DIR_LEFT);
                else if (keys & J_RIGHT) moved = game_move(&game, DIR_RIGHT);
                
                if (moved) draw_grid(&game);
            }
        }
        
        last_keys = keys;
        delay(100);
    }
}
