#include "opengl_controller.hpp"
#include "../domain/interfaces/controllable.hpp"
#include <GL/freeglut_std.h>
#include <cctype>
#include <cstdio>
#include <vector>

bool OpenGLController::keysState[256];
bool OpenGLController::specialKeysState[256];

void OpenGLController::bindKeys(const std::vector<Control> controls) {
  for (Control control : controls) {
    std::function<void()> action = [control, this]() {
      element.move(control.direction);
    };

    actions.emplace_back(ActionPair{control.key, action, control.special_key});
  }
}

std::vector<ActionPair>& OpenGLController::getActions(){
  return actions;
}

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
  for (ActionPair action : actions) { 
    bool is_pressed = false;
    int key = action.key;

    if (key >= 0 && key < 256 and !action.special_key) {
      is_pressed = keysState[tolower(key)];
    }// Verifica se a tecla é uma tecla especial (GLUT_KEY_*)
    else if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_END) {
      is_pressed = specialKeysState[key];
    }

    if (is_pressed) {
      action.callback();
    }
  }
}
