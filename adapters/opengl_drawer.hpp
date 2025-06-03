#pragma once

#include "../domain/interfaces/drawer.hpp"

class OpenGLDrawer : public IDrawer {
  public:
    OpenGLDrawer();

    void drawRoom(const Room& room) const override;
    void drawHallway(const Hallway& hallway) const override;
    void drawMap(const Map& map) const override;
    void drawPlayer(const Player& player) const override;
    void setTileSize(int tile_size) override;
};
