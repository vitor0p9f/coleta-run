#include "opengl_controller.hpp"
#include <GL/freeglut_std.h>
#include <cctype>
#include <cstdio>
#include <vector>

bool OpenGLController::keysState[256];
bool OpenGLController::specialKeysState[256];

void OpenGLController::initKeyStates() {
  for (int index = 0; index < 256; index++) {
    keysState[index] = false;
    specialKeysState[index] = false;
  }
}

void OpenGLController::specialKeysDown(int key, int x, int y) {
    specialKeysState[key] = true;
}

void OpenGLController::specialKeysUp(int key, int x, int y) {
    specialKeysState[key] = false;
}

void OpenGLController::keyboardDown(unsigned char key, int x, int y) {
    key = tolower(key);
    keysState[key] = true;
}

void OpenGLController::keyboardUp(unsigned char key, int x, int y) {
    key = tolower(key);
    keysState[key] = false;
}

void OpenGLController::processInput() const {
  for (const Controllable& element : elements) {
    std::vector<ActionPair> action_list = element.getActions();

    for (ActionPair action : action_list) {
      bool is_pressed = false;
      int key = action.key;

      if (key >= 0 && key < 256 and !action.special_key) {
        is_pressed = keysState[tolower(key)];
      }// Check if is a special key (GLUT_KEY_*)
      else if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_END) {
        is_pressed = specialKeysState[key];
      }

      if (is_pressed) {
        action.callback();
      }
    }
  }
}
