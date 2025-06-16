#pragma once

#include "drawer.hpp"
#include "../game/types.hpp"

class Drawable{
  public:
    Drawable(Point coordinate, int width, int height) : coordinate(coordinate), width(width), height(height){}
    
    virtual ~Drawable() = default;

    virtual void draw(const IDrawer& drawer) const = 0;

    int getWidth() const {
      return width;
    }

    int getHeight() const {
      return height;
    }

    Point getCoordinate() const {
      return coordinate;
    }

    void setCoordinate(Point coordinate){
      this->coordinate = coordinate;
    }

  protected:
    Point coordinate;
    int width;
    int height;
};
