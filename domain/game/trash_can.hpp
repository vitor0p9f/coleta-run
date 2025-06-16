#pragma once

#include "types.hpp"
#include "../interfaces/drawable.hpp"

struct TrashCan: public Drawable{
  Category category;

  TrashCan();

  TrashCan(Point coordinate, int width, int height, Category category) : Drawable(coordinate, width, height), category(category) {}

  void draw(const IDrawer& drawer) const override{
    drawer.drawTrashCan(*this);
  }
};

