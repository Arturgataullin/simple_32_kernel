#include <stdint.h>
#include "screen.h"
#include "keys.h"
#include "pit.h"

static int paddle_y = 5, paddle_x = 2;
static int ball_x = 40, ball_y = 12;
static int bat_size = 3;
static int vx = 1, vy = 1;


static void draw(void) {
    screen_clear(0x07, ' ');
    // рамка 
    for (int x = 0; x < VGA_W; x++) {
        screen_put(x, 0, 0x0F, '#');
        screen_put(x, VGA_H - 1, 0x0F, '#');
    }
    for (int y = 0; y < VGA_W; y++) {
        screen_put(y, 0, 0x0F, '#');
        screen_put(y, VGA_W - 1, 0x0F, '#');
    }
    // ракетка
    for (int i = 0; i < bat_size; i++) {
        screen_put(paddle_y + i, paddle_x, 0x0A, '|');
    }
    // мяч
    screen_put(ball_x, ball_y, 0x0C, 'o');

    screen_present();
} 


static void update(void) {
    // управление: W=0x11, S=0x1F (set 1 scancodes)
    if (key_down(0x11) && paddle_y > 1) paddle_y--;
    if (key_down(0x1F) && paddle_y < VGA_H - 1 - 3) paddle_y++;

    // движение мяча
    ball_x += vx;
    ball_y += vy;

    // отскок от стен
    if (ball_y <= 1) { ball_y = 1; vy = -vy; }
    if (ball_y >= VGA_H-2) { ball_y = VGA_H-2; vy = -vy; }

    // отскок от правой стены
    if (ball_x >= VGA_W-2) { ball_x = VGA_W-2; vx = -vx; }

    // столкновение с ракеткой (слева)
    if (ball_x == 3) {
        if (ball_y >= paddle_y && ball_y < paddle_y + 3) {
            vx = 1; // отскок вправо
        }
    }

    // проигрыш: улетел влево
    if (ball_x <= 1) {
        ball_x = 40; ball_y = 12;
        vx = 1; vy = 1;
    }
}

void pong_run(void) {
    uint32_t last = pit_ticks();
    for (;;) {
        uint32_t now = pit_ticks();
        if (now != last) {
            last = now;
            update();
            draw();
        }
        __asm__ volatile("hlt");
    }
}