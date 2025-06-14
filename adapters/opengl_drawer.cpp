#include <GL/freeglut.h>
#include <GL/gl.h>
#include <cmath>
#include <cstdio>
#include <iostream>
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
  int x = room.getCoordinate().x * tile_size;
  int y = room.getCoordinate().y * tile_size;
  int width = room.getWidth() * tile_size; 
  int height = room.getHeight() * tile_size;

  glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // RBG: black

    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
  glEnd();
}

void OpenGLDrawer::drawHallway(const Hallway& hallway) const {
  int x = hallway.getCoordinate().x * tile_size;
  int y = hallway.getCoordinate().y * tile_size;
  int width = hallway.getWidth() * tile_size; 
  int height = hallway.getHeight() * tile_size;

  glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // RGB: black

    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
  glEnd();
}

void OpenGLDrawer::drawTrashCan(const TrashCan& trash_can) const {
  int x = trash_can.getCoordinate().x * tile_size;
  int y = trash_can.getCoordinate().y * tile_size;
  int width = trash_can.getWidth() * tile_size; 
  int height = trash_can.getHeight() * tile_size;

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

  spriteManager.draw("trash_can_" + category, x, y, width, height);
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
  int x = player.getCoordinate().x * tile_size;
  int y = player.getCoordinate().y * tile_size;
  int width = player.getWidth() * tile_size; 
  int height = player.getHeight() * tile_size;

  spriteManager.draw("player_" + std::to_string(player.id), x, y, width, height);
}

void OpenGLDrawer::drawTrashBag(const TrashBag& trash_bag) const {
  int x = trash_bag.getCoordinate().x * tile_size;
  int y = trash_bag.getCoordinate().y * tile_size;
  int width = trash_bag.getWidth() * tile_size; 
  int height = trash_bag.getHeight() * tile_size;

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

  spriteManager.draw("trash_bag_" + category, x, y, width, height);
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
