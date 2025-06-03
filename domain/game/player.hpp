#pragma once

#include "../interfaces/drawable.hpp"
#include "../interfaces/controllable.hpp"
#include "types.hpp"

struct Player: public Drawable, public Controllable {
  Point coordinate = {0, 0};
  int width = 0;
  int height = 0;
  int step_size = 1;

  Player();

  Player(Point coordinate, int width, int height) : coordinate(coordinate), width(width), height(height){}

  void draw(const IDrawer& drawer) const override {
    drawer.drawPlayer(*this);
  }

  virtual void setStepSize(int step_size) override {
    this->step_size = step_size;
  }
  
  virtual void move(Direction direction) override {
    switch (direction) {
      case UP:
          coordinate.y = coordinate.y + step_size;
        break;

      case DOWN:
          coordinate.y = coordinate.y - step_size;
        break;

      case LEFT:
          coordinate.x = coordinate.x - step_size;
        break;

      case RIGHT:
          coordinate.x = coordinate.x + step_size;
        break;
    }
  }
};
