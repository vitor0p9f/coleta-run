#pragma once

#include "types.hpp"

extern int splashStartTime;

void display();
void idle();
void keyboard(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);
void specialKeysDown(int key, int x, int y);

