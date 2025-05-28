#include "map.hpp"
#include "bsp.hpp"
#include "hallway.hpp"
#include <vector>

Map::Map(Area area, int partition_width, int partition_height, int room_margin, int hallway_size){
  BSP bsp;
  
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
  int width, 
  int height, 
  const std::vector<Room>& rooms, 
  const std::vector<Hallway>& hallways
) {
  std::vector<std::vector<bool>> walkable = std::vector(height, std::vector(width, false));

  for (const Room& room : rooms) {
    for (int x = room.coordinate.x; x < room.coordinate.x + room.width; ++x) {
      for (int y = room.coordinate.y; y < room.coordinate.y + room.height; ++y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
          walkable[y][x] = true;
        }
      }
    }
  }

  return walkable;
}

std::vector<std::vector<bool>> Map::getWalkableMap(){
  return walkable_map;
}

std::vector<Room> Map::getRooms() const {
  return rooms;
}

std::vector<Hallway> Map::getHallways() const {
  return hallways;
}
