#pragma once

#include "../domain/interfaces/controller.hpp"

class OpenGLController: public IController {
  public:
    OpenGLController(){
      initKeyStates();
    };

    void static specialKeysDown(int key, int x, int y);
    void static specialKeysUp(int key, int x, int y);
    void static keyboardDown(unsigned char key, int x, int y);
    void static keyboardUp(unsigned char key, int x, int y);
    void processInput() const;

  private:
    static bool keysState[256];
    static bool specialKeysState[256];

    void initKeyStates();
};
