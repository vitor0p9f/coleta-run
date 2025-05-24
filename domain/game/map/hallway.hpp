#pragma once

#include "../../interfaces/drawable.hpp"
#include "../types.hpp"

class Hallway : public Drawable{
  public:
    Coordinate start, end;
    int width;

    Hallway(Coordinate start, Coordinate end, int width);

    void draw(const IDrawer& drawer) const override;
};
