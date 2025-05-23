#pragma once

class Room;
class Hallway;

class IDrawer{
  public:
    virtual ~IDrawer() = default;

    virtual void drawRoom(const Room& room) const = 0;
    virtual void drawHallway(const Hallway& hallway) const = 0;
};
