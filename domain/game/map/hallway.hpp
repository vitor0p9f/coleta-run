#pragma once

#include "../../interfaces/drawable.hpp"
#include "../types.hpp"

class Hallway : public Drawable{
  public:
    Coordinate start, end;
    int width;

    Hallway(Coordinate start, Coordinate end, int width){
      generate(start, end, width);
    };

    void draw(const IDrawer& drawer) const override {
      drawer.drawHallway(*this);
    };

  private:
    void generate(Coordinate hallway_start, Coordinate hallway_end, int hallway_width){
      start = hallway_start;
      end = hallway_end;
      width = hallway_width;
    }
};
