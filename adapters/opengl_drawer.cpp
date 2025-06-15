#include <GL/freeglut.h>
#include <GL/gl.h>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include "opengl_drawer.hpp"
#include "../domain/game/map/map.hpp"
#include "../domain/game/player.hpp"
#include "../domain/game/trash_can.hpp"
#include "../domain/game/trash_bag.hpp"
#include "../domain/game/timer.hpp"

OpenGLDrawer::OpenGLDrawer(){};

void OpenGLDrawer::drawRoom(const Room& room) const {
  Point coordinate = room.getCoordinate();
  int width = room.getWidth();
  int height = room.getHeight();
  
  for (int y = 0; y < height; y += GRASS_SPRITE_SIZE) {
    for (int x = 0; x < width; x += GRASS_SPRITE_SIZE) {
      int next_x = coordinate.x + x;
      int next_y = coordinate.y + y;

      int draw_width = GRASS_SPRITE_SIZE;
      int draw_height = GRASS_SPRITE_SIZE;

      if (x + GRASS_SPRITE_SIZE > width) {
          draw_width = width - x;
      }
      if (y + GRASS_SPRITE_SIZE > height) {
          draw_height = height - y;
      }

      spriteManager.draw("walkable", next_x, next_y, draw_width, draw_height);
    }
  }
}

void OpenGLDrawer::drawHallway(const Hallway& hallway) const {
  Point coordinate = hallway.getCoordinate();
  int width = hallway.getWidth();
  int height = hallway.getHeight();

  for (int y = 0; y < height; y += GRASS_SPRITE_SIZE) {
    for (int x = 0; x < width; x += GRASS_SPRITE_SIZE) {
      int next_x = coordinate.x + x;
      int next_y = coordinate.y + y;

      int draw_width = GRASS_SPRITE_SIZE;
      int draw_height = GRASS_SPRITE_SIZE;

      if (x + GRASS_SPRITE_SIZE > width) {
          draw_width = width - x;
      }
      if (y + GRASS_SPRITE_SIZE > height) {
          draw_height = height - y;
      }

      spriteManager.draw("walkable", next_x, next_y, draw_width, draw_height);
    }
  }
}

void OpenGLDrawer::drawTrashCan(const TrashCan& trash_can) const {
  int x = trash_can.getCoordinate().x;
  int y = trash_can.getCoordinate().y;

  std::string category = "";

  switch (trash_can.category) {
    case PAPER:
      category = "paper";
    break;

    case GLASS:
      category = "glass";
    break;

    case PLASTIC:
      category = "plastic";
    break;

    case METAL:
      category = "metal";
    break;

    case ORGANIC:
      category = "organic";
    break;
  }

  spriteManager.draw("trash_can_" + category, x, y, TRASH_CAN_SPRITE_SIZE, TRASH_CAN_SPRITE_SIZE);
}

void OpenGLDrawer::drawMap(const Map& map) const {
  std::vector<Room> rooms = map.getRooms();
  std::vector<Hallway> hallways = map.getHallways();

  for(const Room& room : rooms){
      room.draw(*this);
  }

  for(const Hallway& hallway : hallways){
    hallway.draw(*this);
  }
}

void OpenGLDrawer::setTileSize(int tile_size){
  this->tile_size = tile_size;
}

void OpenGLDrawer::drawPlayer(const Player& player) const {
  int x = player.getCoordinate().x;
  int y = player.getCoordinate().y;

  spriteManager.draw(
    "player_" + std::to_string(player.id), x, y, PLAYER_SPRITE_SIZE, PLAYER_SPRITE_SIZE
  );
}

void OpenGLDrawer::drawTrashBag(const TrashBag& trash_bag) const {
  int x = trash_bag.getCoordinate().x * tile_size;
  int y = trash_bag.getCoordinate().y * tile_size;

  std::string category = "";

  switch (trash_bag.category) {
    case PAPER:
      category = "paper";
    break;

    case GLASS:
      category = "glass";
    break;

    case PLASTIC:
      category = "plastic";
    break;

    case METAL:
      category = "metal";
    break;

    case ORGANIC:
      category = "organic";
    break;
  }

  spriteManager.draw("trash_bag_" + category, x, y, TRASH_BAG_SPRITE_SIZE, TRASH_BAG_SPRITE_SIZE);
}

void OpenGLDrawer::drawTimer(const Timer& timer) const {
    std::string time_str = timer.getFormattedTime();
    Point coord = timer.getCoordinate(); // Canto superior esquerdo da caixa delimitadora do timer
    int width_px = timer.getWidth();     // Largura da caixa delimitadora

    void* font = GLUT_BITMAP_TIMES_ROMAN_24; // Esta fonte tem aproximadamente 24 pixels de altura

    int text_width = 0;
    for (char c : time_str) {
        text_width += glutBitmapWidth(font, c);
    }

    int text_draw_x = coord.x + (width_px - text_width) / 2;

    glColor3f(0.0f, 0.0f, 0.0f); // Texto preto

    glRasterPos2i(text_draw_x, 0);

    for (char c : time_str) {
        glutBitmapCharacter(font, c);
    }
}

void OpenGLDrawer::drawPlayerScore(const Player& player, Point position, int width, int height) const {
    std::string score_str = "P" + std::to_string(player.id) + " SCORE: " + std::to_string(player.getScore());

    void* font = GLUT_BITMAP_8_BY_13;

    int text_width = 0;

    for (char c : score_str) {
        text_width += glutBitmapWidth(font, c);
    }

    int text_draw_x = position.x + (width - text_width) / 2;

    glColor3f(0.0f, 0.0f, 0.0f); // Cor do texto: preto

    glRasterPos2i(text_draw_x, 0);

    for (char c : score_str) {
        glutBitmapCharacter(font, c);
    }
}
