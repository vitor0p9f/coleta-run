#include "hallway.hpp"

Hallway::Hallway(Coordinate hallway_start, Coordinate hallway_end, int hallway_width){
  start = hallway_start;
  end = hallway_end;
  width = hallway_width;
}

void Hallway::draw(const IDrawer& drawer) const{
  drawer.drawHallway(*this);
}
