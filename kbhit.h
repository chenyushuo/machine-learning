#ifndef KBHIT_H
#define KBHIT_H

#include <stdio.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>

void InitKeyboard();
void CloseKeyboard();
bool CheckKeyboard();

#endif
