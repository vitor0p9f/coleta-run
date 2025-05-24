#pragma once

#include "../../interfaces/drawable.hpp"
#include "../types.hpp"
#include <random>

class Room : public Drawable{
  public:
    Coordinate coordinate, center;
    int width, height;

    explicit Room(Area area);

    void draw(const IDrawer& drawer) const override;

  private: 
    void generate(Area area);
};
