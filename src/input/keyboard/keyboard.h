#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

char scan2char(unsigned char scancode, bool shift);
unsigned char read_keyboard();

#endif