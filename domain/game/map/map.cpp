#include "map.hpp"
#include "bsp.hpp"
#include "hallway.hpp"
#include <cstdio>
#include <vector>

Map::Map(
  Area area, 
  int partition_width, 
  int partition_height, 
  int room_margin, 
  int hallway_size
) : Drawable({0,0}, area.width, area.height){
  BSP bsp;

  width = area.width;
  height = area.height;

  auto [map_rooms, map_hallways] = bsp.execute(
    area, partition_width, partition_height, room_margin, hallway_size
  );
 
  for(const Room& room : map_rooms){
    rooms.emplace_back(room);
  }

  for(const Hallway& hallway : map_hallways){
    hallways.emplace_back(hallway);
  }

  walkable_map = generateWalkableMap(area.width, area.height, rooms, hallways);
}

void Map::draw(const IDrawer& drawer) const{
  drawer.drawMap(*this);
};

std::vector<std::vector<bool>> Map::generateWalkableMap(
  int map_width, 
  int map_height, 
  const std::vector<Room>& rooms, 
  const std::vector<Hallway>& hallways
) {
  std::vector<std::vector<bool>> walkable = std::vector(height, std::vector(width, false));

  for (const Room& room : rooms) {
    Point room_coordinate = room.getCoordinate();
    int room_width = room.getWidth();
    int room_height = room.getHeight();

    for (int x = room_coordinate.x; x < room_coordinate.x + room_width; ++x) {
      for (int y = room_coordinate.y; y < room_coordinate.y + room_height; ++y) {
        if (x >= 0 && x < map_width && y >= 0 && y < map_height) {
          walkable[y][x] = true;
        }
      }
    }
  }

  for (const Hallway& hallway : hallways) {
    Point hallway_coordinate = hallway.getCoordinate();
    int hallway_width = hallway.getWidth();
    int hallway_height = hallway.getHeight();

    for (int x = hallway_coordinate.x; x < hallway_coordinate.x + hallway_width; x++) {
      for (int y = hallway_coordinate.y; y < hallway_coordinate.y + hallway_height; y++) {
        if (x >= 0 && x < map_width && y >= 0 && y < map_height) {
          walkable[y][x] = true;
        }
      }
    }
  }

  return walkable;
}

WalkableMap& Map::getWalkableMap(){
  return walkable_map;
}

const std::vector<Room>& Map::getRooms() const {
  return rooms;
}

const std::vector<Hallway>& Map::getHallways() const {
  return hallways;
}