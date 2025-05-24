#include "map.hpp"
#include "bsp.hpp"

Map::Map(Area area, int room_size){
  BSP bsp;
      
  auto [map_rooms, map_hallways] = bsp.execute(area, room_size);
  
  for(Room room : map_rooms){
    rooms.push_back(room);
  }

  for(Hallway hallway : map_hallways){
    hallways.push_back(hallway);
  }

  walkable_map = generateWalkableMap(area.width, area.height, rooms, hallways);
}

void Map::draw(const IDrawer& drawer) const{
  for(const Room& room : rooms){
    room.draw(drawer);
  }

  for(const Hallway& hallway : hallways){
    hallway.draw(drawer);
  }
};
