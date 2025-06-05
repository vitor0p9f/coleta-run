#pragma once

#include "../domain/interfaces/controller.hpp"
#include <functional>
#include <vector>

struct Control {
  int key;
  Direction direction;
  bool special_key;

  Control();

  Control(
      int key, 
      Direction direction, 
      bool special_key
  ) : key(key), direction(direction), special_key(special_key){};
};

struct ActionPair {
  int key;
  std::function<void()> callback;
  bool special_key;

  ActionPair();

  ActionPair(
      int key, 
      std::function<void()> callback, 
      bool special_key
  ) : key(key), callback(callback), special_key(special_key){};
};

class OpenGLController: public IController {
  public:
    explicit OpenGLController(Controllable& element) : IController(element){
      initKeyStates();
    };

    virtual void bindKeys(std::vector<Control> controls) override;
    std::vector<ActionPair>& getActions();
    void static specialKeysDown(int key, int x, int y);
    void static specialKeysUp(int key, int x, int y);
    void static keyboardDown(unsigned char key, int x, int y);
    void static keyboardUp(unsigned char key, int x, int y);
    void processInput() const;

  private:
    std::vector<ActionPair> actions = {};
    static bool keysState[256];
    static bool specialKeysState[256];

    void initKeyStates();
};
