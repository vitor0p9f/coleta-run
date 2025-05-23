#pragma once

#include "drawer.hpp"

class Drawable{
  public:
    virtual ~Drawable() = default;

    virtual void draw(const IDrawer& drawer) const = 0;
};
