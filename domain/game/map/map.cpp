#include "map.hpp"
#include "bsp.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

Map::Map(Area area, int room_size){
  BSP bsp;
      
  auto [map_rooms, map_hallways] = bsp.execute(area, room_size);
 
  for(const Room& room : map_rooms){
    rooms.push_back(room);
  }

  for(const Hallway& hallway : map_hallways){
    hallways.push_back(hallway);
  }

  walkable_map = generateWalkableMap(area.width, area.height, rooms, hallways);

  for (int x = 0; x < walkable_map[0].size(); ++x) { // percorre linhas (altura)
        for (int y = 0; y < walkable_map.size(); ++y) { // percorre colunas (largura)
            if (walkable_map[y][x]) {
                std::cout << '.'; // andar possível
            } else {
                std::cout << '#'; // parede/inacessível
            }
        }
        std::cout << '\n';
    }
}

void Map::draw(const IDrawer& drawer) const{
  for(const Room& room : rooms){
    room.draw(drawer);
  }

  for(const Hallway& hallway : hallways){
    hallway.draw(drawer);
  }
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

  for (const Hallway& hall : hallways) {
    int x1 = hall.start.x;
    int y1 = hall.start.y;
    int x2 = hall.end.x;

    if (x1 == x2) {
      // Corredor vertical
      int y2 = hall.end.y;
      int y_start = std::min(y1, y2);
      int y_end = std::max(y1, y2);
      int half_width = hall.width / 2;

      for (int y = y_start; y <= y_end; ++y) {
        for (int dx = -half_width; dx <= half_width; ++dx) {
          int x = x1 + dx;

          if (x >= 0 && x < width && y >= 0 && y < height) {
              walkable[y][x] = true;
          }
        }
      }
    } else {
      // Corredor horizontal
      int x_start = std::min(x1, x2);
      int x_end = std::max(x1, x2);
      int half_width = hall.width / 2;

      for (int x = x_start; x <= x_end; ++x) {
        for (int dy = -half_width; dy <= half_width; ++dy) {
          int y = y1 + dy;

          if (x >= 0 && x < width && y >= 0 && y < height) {
              walkable[y][x] = true;
          }
        }
      }
    }
  }

  return walkable;
}
