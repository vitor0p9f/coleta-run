#pragma once

#include "../../interfaces/drawable.hpp"
#include "../types.hpp"

struct Hallway: public Drawable{
  int size;

  Hallway();

  Hallway(
      Point coordinate, 
      int width, 
      int height, 
      int size
  ) : Drawable(coordinate, width, height), size(size){} 
  
  void draw(const IDrawer& drawer) const override{
    drawer.drawHallway(*this);
  }
};
