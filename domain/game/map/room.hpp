#pragma once

#include "../types.hpp"
#include "../../interfaces/drawable.hpp"

struct Room: public Drawable{
  Point coordinate;
  Point center;
  int width, height;

  Room() : coordinate(), center(), width(0), height(0) {}

  Room(Point initial_coordinate, int width, int height) : 
    coordinate(initial_coordinate),
    center( (initial_coordinate.x + width / 2), (initial_coordinate.y + height / 2) ),
    width(width),
    height(height){}

  void draw(const IDrawer& drawer) const override{
    drawer.drawRoom(*this);
  }
};
