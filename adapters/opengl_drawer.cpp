#include <GL/freeglut.h>
#include <GL/gl.h>
#include <vector>
#include "opengl_drawer.hpp"
#include "../domain/game/map/map.hpp"
#include "../domain/game/player.hpp"

OpenGLDrawer::OpenGLDrawer(){};

void OpenGLDrawer::drawRoom(const Room& room) const {
  int x = room.coordinate.x * tile_size;
  int y = room.coordinate.y * tile_size;
  int width = room.width * tile_size; 
  int height = room.height * tile_size;

  glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // RBG: black

    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
  glEnd();
}

void OpenGLDrawer::drawHallway(const Hallway& hallway) const {
  int x = hallway.coordinate.x * tile_size;
  int y = hallway.coordinate.y * tile_size;
  int width = hallway.width * tile_size; 
  int height = hallway.height * tile_size;

  glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // RGB: black

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
  int x = player.coordinate.x * tile_size;
  int y = player.coordinate.y * tile_size;
  int width = player.width * tile_size; 
  int height = player.height * tile_size;

  glBegin(GL_QUADS);
    glColor3f(0.8f, 0.5f, 0.3f);

    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
  glEnd();
}
