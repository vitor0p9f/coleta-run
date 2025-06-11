#pragma once

class Room;
class Hallway;
class Map;
class Player;
class TrashCan;
class TrashBag;
class Timer;

class IDrawer{
  public:
    int tile_size = 1;

    virtual ~IDrawer() = default;
    
    virtual void setTileSize(int tile_size) = 0;
    virtual void drawRoom(const Room& room) const = 0;
    virtual void drawHallway(const Hallway& hallway) const = 0;
    virtual void drawMap(const Map& map) const = 0;
    virtual void drawPlayer(const Player& player) const = 0;
    virtual void drawTrashCan(const TrashCan& trash_can) const = 0;
    virtual void drawTrashBag(const TrashBag& trash_bag) const = 0;
    virtual void drawTimer(const Timer& timer) const = 0;
};
