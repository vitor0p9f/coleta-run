#include <GL/freeglut.h>
#include <GL/gl.h>
#include <vector>
#include "opengl_drawer.hpp"
#include "../domain/game/map/map.hpp"
#include "../domain/game/player.hpp"
#include "../domain/game/trash_can.hpp"

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

  std::vector<float> color;

  switch (trash_can.category) {
    case PAPER:
      color = {0.0f, 0.0f, 1.0f}; // RGB: blue
    break;

    case GLASS:
      color = {0.0f, 1.0f, 0.0f}; // RGB: green
    break;

    case PLASTIC:
      color = {1.0f, 0.0f, 0.0f}; // RGB: vermelho
    break;

    case METAL:
      color = {1.0f, 1.0f, 0.0f}; // RGB: yellow
    break;

    case ORGANIC:
      color = {1.4f, 0.6f, 0.2f}; // RGB: brown
    break;
  }

  glBegin(GL_QUADS);
    glColor3f(color[0], color[1], color[2]); // RGB: black

    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
  glEnd();
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

  glBegin(GL_QUADS);
    glColor3f(0.8f, 0.5f, 0.3f);

    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
  glEnd();
}
