#pragma once

#include <cstdio>
#include <functional>
#include <vector>

enum Direction{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

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

class Controllable{
  public:
    virtual ~Controllable() = default;

    virtual void setStepSize(int step_size) = 0;
    virtual void move(Direction direction) = 0;

    const std::vector<ActionPair>& getActions() const {
      return action_list;
    }

    void bindKeys(const std::vector<Control>& control_list) {
      for (Control control : control_list) {
        std::function<void()> action = [control, this]() {
          move(control.direction);
        };

        action_list.emplace_back(ActionPair{control.key, action, control.special_key});
      }
    }

  protected:
    std::vector<ActionPair> action_list = {};
    std::vector<Control> control_list = {};
};
