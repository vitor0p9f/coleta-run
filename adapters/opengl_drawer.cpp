#include <GL/freeglut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
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

void OpenGLDrawer::drawTrashBag(const TrashBag& trash_bag) const {
    int center_x = (trash_bag.getCoordinate().x * tile_size) + (trash_bag.getWidth() * tile_size / 2);
    int center_y = (trash_bag.getCoordinate().y * tile_size) + (trash_bag.getHeight() * tile_size / 2);
    float radius = (float)std::min(trash_bag.getWidth(), trash_bag.getHeight()) * tile_size / 2.0f;

    std::vector<float> color;

    switch (trash_bag.category) { // Assumindo que TrashBag tem um membro 'type'
        case PAPER:
            color = {0.0f, 0.0f, 1.0f}; // Azul
            break;
        case GLASS:
            color = {0.0f, 1.0f, 0.0f}; // Verde
            break;
        case PLASTIC:
            color = {1.0f, 0.0f, 0.0f}; // Vermelho
            break;
        case METAL:
            color = {1.0f, 1.0f, 0.0f}; // Amarelo
            break;
        case ORGANIC:
            color = {0.58f, 0.29f, 0.0f}; // Marrom (corrigido 1.4, 0.6, 0.2 para 0-1 range)
            break;
        default:
            color = {0.5f, 0.5f, 0.5f}; // Cinza padrão
            break;
    }

    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2i(center_x, center_y);

        const int NUM_SEGMENTS = 30; 
        for (int i = 0; i <= NUM_SEGMENTS; ++i) {
            float angle = 2.0f * M_PI * (float)i / (float)NUM_SEGMENTS;
            int x = center_x + static_cast<int>(radius * std::cos(angle));
            int y = center_y + static_cast<int>(radius * std::sin(angle));
            glVertex2i(x, y);
        }
    glEnd();
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
