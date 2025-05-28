#pragma once

#include "../../interfaces/drawable.hpp"
#include "../types.hpp"

struct Hallway: public Drawable{
  Point coordinate;
  int width, height, size;

  Hallway() : coordinate(), width(0), height(0), size(0) {}

  Hallway(Point initial_coordinate, int width, int height, int size) :
    coordinate(initial_coordinate),
    width(width),
    height(height),
    size(size) {}
 
  void draw(const IDrawer& drawer) const override{
    drawer.drawHallway(*this);
  }
};
