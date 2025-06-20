#pragma once

#include <GL/freeglut.h>
#include "../domain/interfaces/drawer.hpp"
#include "sprite_manager.hpp"
#include "../domain/interfaces/drawable.hpp"

class OpenGLDrawer : public IDrawer {
  public:
    OpenGLDrawer();

    void drawRoom(const Room& room) const override;
    void drawHallway(const Hallway& hallway) const override;
    void drawMap(const Map& map) const override;
    void drawPlayer(const Player& player) const override;
    void drawTrashCan(const TrashCan& trash_can) const override;
    void drawTrashBag(const TrashBag& trash_bag) const override;
    void drawTimer(const Timer& timer) const override;
    void drawPlayerScore(const Player& player, Point position, int width, int height) const;
    void setTileSize(int tile_size) override;

    SpriteManager spriteManager;

  private:
    const int GRASS_SPRITE_SIZE = 16; 
    const int PLAYER_SPRITE_SIZE = 32; 
    const int TRASH_CAN_SPRITE_SIZE = 32; 
    const int TRASH_BAG_SPRITE_SIZE = 16; 
};
