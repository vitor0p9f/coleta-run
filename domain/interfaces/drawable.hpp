#pragma once

#include "drawer.hpp"
#include "../game/types.hpp"

class Drawable{
  public:
    Point coordinate;
    int width;
    int height;

    Drawable(Point coordinate, int width, int height) : coordinate(coordinate), width(width), height(height){}
    
    virtual ~Drawable() = default;

    virtual void draw(const IDrawer& drawer) const = 0;
};
