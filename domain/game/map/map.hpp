#pragma once

#include "../../interfaces/drawable.hpp"
#include "bsp.hpp"
#include "hallway.hpp"
#include "room.hpp"
#include <cstdio>
#include <vector>

class Map : public Drawable{
  public:
    explicit Map(Area area, int room_size){
      BSP bsp;
      
      auto [map_rooms, map_hallways] = bsp.execute(area, room_size);
      
      for(Room room : map_rooms){
        printf("x: %d\n", room.coordinate.x);
        rooms.push_back(room);
      }

      for(Hallway hallway : map_hallways){
        printf("y: %d\n", hallway.start.y);
        hallways.push_back(hallway);
      }
    };

    void draw(const IDrawer& drawer) const override {
      for(const Room& room : rooms){
        room.draw(drawer);
      }

      for(const Hallway& hallway : hallways){
        hallway.draw(drawer);
      }
    };

  private:
    std::vector<Room> rooms;
    std::vector<Hallway> hallways;
};
