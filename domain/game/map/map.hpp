#pragma once

#include "../../interfaces/drawable.hpp"
#include "hallway.hpp"
#include "room.hpp"
#include <vector>

class Map : public Drawable{
  public:
    explicit Map(
      Area area, 
      int partition_width, 
      int partition_height, 
      int room_margin, 
      int hallway_size
    );

    std::vector<std::vector<bool>> getWalkableMap();
    std::vector<Room> getRooms() const;
    std::vector<Hallway> getHallways() const;
    void draw(const IDrawer& drawer) const override;

  private:
    std::vector<Room> rooms = {};
    std::vector<Hallway> hallways = {};
    std::vector<std::vector<bool>> walkable_map = {};

    std::vector<std::vector<bool>> generateWalkableMap(
        int width, 
        int height, 
        const std::vector<Room>& rooms, 
        const std::vector<Hallway>& hallways
    );
};
