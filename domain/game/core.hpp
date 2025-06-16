#pragma once

#include "../interfaces/controller.hpp"
#include "../interfaces/drawer.hpp"
#include "../interfaces/drawable.hpp"
#include "map/map.hpp"
#include "player.hpp"
#include "timer.hpp"
#include "trash_bag.hpp"
#include <chrono>
#include <vector>

const int PARTITION_WIDTH = 180;
const int PARTITION_HEIGHT = 150;
const int ROOM_MARGIN = 25;
const int HALLWAY_SIZE = 80;

const int MAP_WIDTH = 600;
const int MAP_HEIGHT = 600;

const int PLAYER_SIZE = 30;
const int TRASH_CAN_SIZE = 32;
const int TRASH_BAG_SIZE = 24;

const int TIME_IN_SECONDS = 120;

class Game {
  public:
    Game(
      const IDrawer& drawer, 
      IController& controller
    );
    
    void draw();

    Player& getPlayer1(){
      return player_1;
    }

    Player& getPlayer2(){
      return player_2;
    }

    const Map& getMap(){
      return map;
    }

    Timer& getTimer() {
      return game_timer;
    }

    void init();

    
    void update();

  private:
    const IDrawer& drawer;
    
    IController& controller;
    
    std::vector<TrashCan> trash_cans = {
      TrashCan{Point{0, 0}, TRASH_CAN_SIZE, TRASH_CAN_SIZE, METAL},
      TrashCan{Point{0, 0}, TRASH_CAN_SIZE, TRASH_CAN_SIZE, PAPER},
      TrashCan{Point{0, 0}, TRASH_CAN_SIZE, TRASH_CAN_SIZE, GLASS},
      TrashCan{Point{0, 0}, TRASH_CAN_SIZE, TRASH_CAN_SIZE, PLASTIC},
      TrashCan{Point{0, 0}, TRASH_CAN_SIZE, TRASH_CAN_SIZE, ORGANIC}
    };
    
    Map map = Map(Area{Point{0,0}, MAP_WIDTH, MAP_HEIGHT}, 
      PARTITION_WIDTH, 
      PARTITION_HEIGHT, 
      ROOM_MARGIN, 
      HALLWAY_SIZE
    );
    
    Player player_1 = Player{Point{0,0}, PLAYER_SIZE, PLAYER_SIZE, 1, map.getWalkableMap()};
    Player player_2 = Player{Point{0,0}, PLAYER_SIZE, PLAYER_SIZE, 2, map.getWalkableMap()};
    
    std::vector<TrashBag> trash_bags = {};
    int max_trash_bags = 30;
    int spawn_interval_ms = 10000;
    std::chrono::high_resolution_clock::time_point last_spawn_time = std::chrono::high_resolution_clock::now();
    
    Timer game_timer = Timer(Point{0, 0}, 10, 10, TIME_IN_SECONDS);

    bool map_was_drawed = false;

    void spawnElement(Drawable& element, Map& map);
    void spawnTrashBags();
    void handleCollisions();
    void removeTrashBag(TrashBag* bag_to_remove);
    bool isColliding(const Drawable& obj1, const Drawable& obj2);
};
