#pragma once

#include "../../interfaces/drawable.hpp"
#include "hallway.hpp"
#include "room.hpp"
#include "../trash_can.hpp"
#include <functional>
#include <vector>

class Map : public Drawable{
  public:
    explicit Map(
      Area area, 
      int partition_width, 
      int partition_height, 
      int room_margin, 
      int hallway_size
    );

    const std::vector<std::vector<bool>>& getWalkableMap() const;
    const std::vector<Room>& getRooms() const;
    const std::vector<Hallway>& getHallways() const;
    const std::vector<TrashCan>& getTrashCans() const;
    void draw(const IDrawer& drawer) const override;
    int getWidth() const;
    int getHeight() const;
    void spawnInWalkableArea(Drawable& element);
    void addPlayers(std::vector<std::reference_wrapper<Player>>& players);
    void spawnTrashCans();
    void spawnTrashBags();

  private:
    std::vector<Room> rooms = {};
    std::vector<Hallway> hallways = {};
    std::vector<std::vector<bool>> walkable_map = {};
    std::vector<TrashCan> trash_cans = {};

    std::vector<std::vector<bool>> generateWalkableMap(
        int width, 
        int height, 
        const std::vector<Room>& rooms, 
        const std::vector<Hallway>& hallways
    );
};
