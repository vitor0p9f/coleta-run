#pragma once

#include "types.hpp"
#include "../interfaces/drawable.hpp"

struct TrashBag: public Drawable{
  Category category;
  TrashBag();

  TrashBag(
    Point coordinate, 
    int width, 
    int height, 
    Category category
  ) : Drawable(coordinate, width, height), category(category) {}

  void draw(const IDrawer& drawer) const override{
    drawer.drawTrashBag(*this);
  }
};

