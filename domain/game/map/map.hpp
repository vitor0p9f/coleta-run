#pragma once

#include "../../interfaces/drawable.hpp"
#include "hallway.hpp"
#include "room.hpp"
#include "../trash_can.hpp"
#include <functional>
#include <vector>

typedef std::vector<std::vector<bool>> WalkableMap;
typedef std::vector<std::reference_wrapper<Player>> PlayerList;

class Map : public Drawable{
  public:
    explicit Map(
      Area area, 
      int partition_width, 
      int partition_height, 
      int room_margin, 
      int hallway_size
    );

    WalkableMap& getWalkableMap();
    const std::vector<Room>& getRooms() const;
    const std::vector<Hallway>& getHallways() const;
    const std::vector<TrashCan>& getTrashCans() const;
    void draw(const IDrawer& drawer) const override;

  private:
    std::vector<Room> rooms = {};
    std::vector<Hallway> hallways = {};
    WalkableMap walkable_map = {};

    WalkableMap generateWalkableMap(
        int width, 
        int height, 
        const std::vector<Room>& rooms, 
        const std::vector<Hallway>& hallways
    );
};
