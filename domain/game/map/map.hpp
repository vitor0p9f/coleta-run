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

    std::vector<std::vector<bool>> getWalkableMap() const;
    std::vector<Room> getRooms() const;
    std::vector<Hallway> getHallways() const;
    void draw(const IDrawer& drawer) const override;
    int getWidth() const;
    int getHeight() const;

  private:
    std::vector<Room> rooms = {};
    std::vector<Hallway> hallways = {};
    std::vector<std::vector<bool>> walkable_map = {};
    int width = 0;
    int height = 0;

    std::vector<std::vector<bool>> generateWalkableMap(
        int width, 
        int height, 
        const std::vector<Room>& rooms, 
        const std::vector<Hallway>& hallways
    );
};
