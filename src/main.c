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

// 绘制 Logo 界面
void draw_logo() {
    uint8_t i;
    cls();
    
    // 顶部空几行
    for (i = 0; i < 3; i++) {
        gotoxy(0, i);
        printf(" ");
    }
    
    // 大标题 2048
    gotoxy(5, 4);
    printf("2048");
    
    gotoxy(5, 6);
    printf("GAME");
    
    // 说明文字
    gotoxy(2, 10);
    printf("Press A to Start");
    
    gotoxy(1, 14);
    printf("Arrows:Move");
    gotoxy(1, 15);
    printf("A:Reset");
}

// 绘制游戏界面
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
    }
}

// 绘制 GameOver 界面
void draw_gameover(Game* game) {
    uint8_t i;
    cls();
    
    // 顶部空几行
    for (i = 0; i < 3; i++) {
        gotoxy(0, i);
        printf(" ");
    }
    
    // GameOver 标题
    gotoxy(3, 4);
    printf("GAME OVER");
    
    // 最终分数
    gotoxy(4, 7);
    printf("Score:");
    gotoxy(11, 7);
    print_num3(game->score);
    
    // 说明文字
    gotoxy(2, 11);
    printf("Press A to Retry");
    
    gotoxy(1, 15);
    printf("A:Restart");
}

void main(void) {
    Game game;
    uint8_t last_keys = 0;
    uint8_t keys;

    initrand(sys_time);
    
    // 初始状态为 Logo 界面
    game.state = STATE_LOGO;
    draw_logo();

    while (1) {
        keys = joypad();

        if (keys && !last_keys) {
            if (game.state == STATE_LOGO) {
                // Logo 界面按 A 开始游戏
                if (keys & J_A) {
                    game_init(&game);
                    game.state = STATE_PLAYING;
                    draw_grid(&game);
                }
            } else if (game.state == STATE_PLAYING) {
                // 游戏进行中
                if (keys & J_A) {
                    game_init(&game);
                    game.state = STATE_PLAYING;
                    draw_grid(&game);
                } else {
                    uint8_t moved = 0;

                    if (keys & J_UP) moved = game_move(&game, DIR_UP);
                    else if (keys & J_DOWN) moved = game_move(&game, DIR_DOWN);
                    else if (keys & J_LEFT) moved = game_move(&game, DIR_LEFT);
                    else if (keys & J_RIGHT) moved = game_move(&game, DIR_RIGHT);

                    if (moved) draw_grid(&game);
                    
                    // 检查游戏结束
                    if (game.game_over) {
                        game.state = STATE_GAMEOVER;
                        draw_gameover(&game);
                    }
                }
            } else if (game.state == STATE_GAMEOVER) {
                // GameOver 界面按 A 重新开始
                if (keys & J_A) {
                    game_init(&game);
                    game.state = STATE_PLAYING;
                    draw_grid(&game);
                }
            }
        }

        last_keys = keys;
        delay(100);
    }
}
