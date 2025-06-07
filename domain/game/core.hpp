#pragma once

#include "../interfaces/controller.hpp"
#include "../interfaces/drawer.hpp"
#include "../interfaces/drawable.hpp"
#include "map/map.hpp"
#include "player.hpp"
#include <vector>

const int PARTITION_WIDTH = 180;
const int PARTITION_HEIGHT = 150;
const int ROOM_MARGIN = 25;
const int HALLWAY_SIZE = 80;

const int MAP_WIDTH = 600;
const int MAP_HEIGHT = 600;

const int PLAYER_SIZE = 10;

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

    void run();
    
    void init();

  private:
    const IDrawer& drawer;
    IController& controller;
    std::vector<TrashCan> trash_cans = {
      TrashCan{Point{0, 0}, 10, 10, METAL},
      TrashCan{Point{0, 0}, 10, 10, PAPER},
      TrashCan{Point{0, 0}, 10, 10, GLASS},
      TrashCan{Point{0, 0}, 10, 10, PLASTIC},
      TrashCan{Point{0, 0}, 10, 10, ORGANIC}
    };
    Map map = Map(Area{Point{0,0}, MAP_WIDTH, MAP_HEIGHT}, 
      PARTITION_WIDTH, 
      PARTITION_HEIGHT, 
      ROOM_MARGIN, 
      HALLWAY_SIZE
    );
    Player player_1 = Player{Point{0,0}, PLAYER_SIZE, PLAYER_SIZE, map.getWalkableMap()};
    Player player_2 = Player{Point{0,0}, PLAYER_SIZE, PLAYER_SIZE, map.getWalkableMap()};

    void spawnElement(Drawable& element, Map& map);
};
