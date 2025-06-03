#pragma once

enum Direction{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Controllable{
  public:
    virtual ~Controllable() = default;

    virtual void setStepSize(int step_size) = 0;
    virtual void move(Direction direction) = 0;
};
