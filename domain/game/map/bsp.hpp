#pragma once

#include "bsp_tree.hpp"
#include "room.hpp"
#include <map>
#include <vector>

class BSP{
  public:
    BSP();

    std::tuple<std::vector<Room>, std::vector<Hallway>> execute(Area area, int min_room_size);

  private:
    std::map<Node*, Room> leafs_and_rooms;

    void splitArea(Node* node, int min_room_size);
    bool areaIsDivisible(int width, int height, int min_room_size, int multiplication_factor);
    std::vector<Room> generateRooms(const std::vector<Node>& leafs);
    std::vector<Hallway> connectRoomsByNearest(const std::vector<Room>& rooms, int hallway_width); 
};
