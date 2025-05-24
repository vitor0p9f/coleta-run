#pragma once

#include "../../interfaces/drawable.hpp"
#include "hallway.hpp"
#include "room.hpp"
#include <vector>

class Map : public Drawable{
  public:
    std::vector<bool> walkable_map;
    
    explicit Map(Area area, int room_size);

    void draw(const IDrawer& drawer) const override;

  private:
    std::vector<Room> rooms;
    std::vector<Hallway> hallways;

    std::vector<bool> generateWalkableMap(
        int width, 
        int height, 
        const std::vector<Room>& rooms, 
        const std::vector<Hallway>& hallways
    );
};
