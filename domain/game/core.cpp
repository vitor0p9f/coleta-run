#include "core.hpp"
#include "map/map.hpp"
#include "trash_bag.hpp"
#include "trash_can.hpp"
#include "types.hpp"
#include <random>
#include <chrono>
#include <vector>

Game::Game(
    const IDrawer& drawer, 
    IController& controller
) : drawer(drawer), controller(controller){}

void Game::spawnElement(Drawable& element, Map& map){
  static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  int map_width = map.getWidth();
  int map_height = map.getHeight();
  WalkableMap& walkable_map = map.getWalkableMap();

  int element_width = element.getWidth();
  int element_height = element.getHeight();

  std::uniform_int_distribution<int> distribution_x(0, map_width - element_width);
  std::uniform_int_distribution<int> distribution_y(0, map_height - element_height);

  const int MAX_ATTEMPTS = 100;
  int attempts = 0;
  bool spawned = false;

  while (attempts < MAX_ATTEMPTS && !spawned) {
    int test_x = distribution_x(rng);
    int test_y = distribution_y(rng);

    bool area_is_walkable = true;

    // Check if the entire element's area is walkable
    for (int y_offset = 0; y_offset < element_height; y_offset++) {
      for (int x_offset = 0; x_offset < element_width; x_offset++) {
        int current_x = test_x + x_offset;
        int current_y = test_y + y_offset;

        if (current_x >= map_width || current_y >= map_height || current_x < 0 || current_y < 0) {
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
      element.setCoordinate({test_x, test_y});

      // If it's not a player, mark the area as non-walkable
      bool is_player = (dynamic_cast<Player*>(&element) != nullptr);

      if (!is_player) {
        for (int y_offset = 0; y_offset < element_height; ++y_offset) {
          for (int x_offset = 0; x_offset < element_width; ++x_offset) {
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

void Game::init(){
  controller.addElement(player_1);
  controller.addElement(player_2);

  for (TrashCan& trash_can : trash_cans) {
    spawnElement(trash_can, map); 
  }
  
  spawnElement(player_1, map);
  spawnElement(player_2, map);
}

void Game::draw(){
  map.draw(drawer);

  for (const TrashCan& trash_can : trash_cans) {
    trash_can.draw(drawer);
  }

  player_1.draw(drawer);
  player_2.draw(drawer);

  for (const TrashBag& trash_bag : trash_bags) {
    trash_bag.draw(drawer);
  }
}

void Game::spawnTrashBags(){
  if (trash_bags.size() >= max_trash_bags) {
    return;
  }

  auto current_time = std::chrono::high_resolution_clock::now();
  long long time_since_last_spawn_ms = 
      std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_spawn_time).count();

  if (time_since_last_spawn_ms < spawn_interval_ms) {
      return;
  }

  static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  const int NUM_CATEGORIES = ORGANIC + 1; 

  std::uniform_int_distribution<int> category_dist(0, NUM_CATEGORIES - 1);

  Category random_category = static_cast<Category>(category_dist(rng));

  TrashBag new_bag({0,0}, 10, 10, random_category);

  spawnElement(new_bag, map); 

  trash_bags.emplace_back(new_bag);
  
  last_spawn_time = current_time;
}
