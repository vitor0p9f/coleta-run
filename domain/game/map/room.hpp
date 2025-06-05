#pragma once

#include "../types.hpp"
#include "../../interfaces/drawable.hpp"

struct Room: public Drawable{
  Point center;
  
  Room() : Drawable({0, 0}, 0, 0){};

  Room(
    Point coordinate, 
    int width, int height
  )  : Drawable(coordinate, width, height), center(
    (coordinate.x + width / 2), 
    (coordinate.y + height / 2) 
  ){}

  void draw(const IDrawer& drawer) const override{
    drawer.drawRoom(*this);
  }
};
