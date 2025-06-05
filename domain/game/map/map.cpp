#include "map.hpp"
#include "bsp.hpp"
#include "hallway.hpp"
#include "../player.hpp"
#include <cstdio>
#include <functional>
#include <vector>
#include <chrono>

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

  for (const Hallway& hallway : hallways) {
    for (int x = hallway.coordinate.x; x < hallway.coordinate.x + hallway.width; x++) {
      for (int y = hallway.coordinate.y; y < hallway.coordinate.y + hallway.height; y++) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
          walkable[y][x] = true;
        }
      }
    }
  }


  return walkable;
}

const std::vector<std::vector<bool>>& Map::getWalkableMap() const {
  return walkable_map;
}

const std::vector<Room>& Map::getRooms() const {
  return rooms;
}

const std::vector<Hallway>& Map::getHallways() const {
  return hallways;
}

int Map::getWidth() const {
  return width;
}

int Map::getHeight() const {
  return height;
}

void Map::spawnInWalkableArea(Drawable& element) {
  static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  std::uniform_int_distribution<int> distribution_x(0, width - element.width);
  std::uniform_int_distribution<int> distribution_y(0, height - element.height);

  const int MAX_ATTEMPTS = 100;
  int attempts = 0;
  bool spawned = false;

  while (attempts < MAX_ATTEMPTS && !spawned) {
      int test_x = distribution_x(rng);
      int test_y = distribution_y(rng);

      bool area_is_walkable = true;

      // Check if the entire element's area is walkable
      for (int y_offset = 0; y_offset < element.height; y_offset++) {
        for (int x_offset = 0; x_offset < element.width; x_offset++) {
          int current_x = test_x + x_offset;
          int current_y = test_y + y_offset;

          if (current_x >= width || current_y >= height || current_x < 0 || current_y < 0) {
            area_is_walkable = false;
            break; 
          }
          
          if (!walkable_map[current_y][current_x]) {
            area_is_walkable = false;
            break;
          }
        }

          if (!area_is_walkable) break;
      }

      if (area_is_walkable) {
        printf("Antiga coordenada x:%d e y:%d", element.coordinate.x, element.coordinate.y);
          
        element.coordinate.x = test_x;
          element.coordinate.y = test_y;

        printf("Nova coordenada x:%d e y:%d", element.coordinate.x, element.coordinate.y);

          // If it's not a player, mark the area as non-walkable
          bool is_player = (dynamic_cast<Player*>(&element) != nullptr);

          if (!is_player) {
              for (int y_offset = 0; y_offset < element.height; ++y_offset) {
                  for (int x_offset = 0; x_offset < element.width; ++x_offset) {
                      walkable_map[test_y + y_offset][test_x + x_offset] = false;
                  }
              }
          }

          spawned = true;
      }
      attempts++;
  }

  if (!spawned) {
    printf(
        "Warning: Could not find a walkable area for element after %d attempts.\n", MAX_ATTEMPTS
    );
  }
}

void Map::addPlayers(std::vector<std::reference_wrapper<Player>>& players){
  for (Player& player : players) {
    spawnInWalkableArea(player);
  }
}
