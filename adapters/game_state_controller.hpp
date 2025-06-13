#pragma once

#include <vector>
#include <string>
#include "types.hpp"

extern int splashStartTime;

void drawMenuOptions(const std::vector<std::string>& options, int selectedOption, int startX, int startY, int spacing);
void display();
void idle();
void keyboard(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);
void specialKeysDown(int key, int x, int y);


