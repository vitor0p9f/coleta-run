#pragma once

class Room;
class Hallway;
class Map;

class IDrawer{
  public:
    int tile_size = 1;

    virtual ~IDrawer() = default;
    
    virtual void setTileSize(int tile_size) = 0;
    virtual void drawRoom(const Room& room) const = 0;
    virtual void drawHallway(const Hallway& hallway) const = 0;
    virtual void drawMap(const Map& map) const = 0;
};
