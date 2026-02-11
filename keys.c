#include "include/keys.h"

static volatile uint8_t down[256]; // массив со значениями нажата(1)/отпущена(0) для каждой клавиши

void keys_on_scancode(uint8_t sc) { 
    if (sc & 0x80) down[sc & 0x7F] = 0;        // scnacode - 8 bit; 0-6 info bits, 7 bit - press(1) or release(0)
    else down[sc] = 1;
}

int key_down(uint8_t sc) {
    return down[sc] != 0;
}