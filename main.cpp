#include <GL/freeglut.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <functional>
#include <vector>
#include "domain/game/map/map.hpp"
#include "adapters/opengl_drawer.hpp"
#include "domain/game/player.hpp"
#include "adapters/opengl_controller.hpp"

const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 688;
const float WINDOW_FRACTION = 0.95;
const int TILE_SIZE = 8;

const int PARTITION_WIDTH = 180;
const int PARTITION_HEIGHT = 150;
const int ROOM_MARGIN = 25;
const int HALLWAY_SIZE = 80;

const int MAP_WIDTH = 600;
const int MAP_HEIGHT = 600;

const int WORLD_WIDTH = MAP_WIDTH * TILE_SIZE;
const int WORLD_HEIGHT = MAP_HEIGHT * TILE_SIZE;

Map* map_pointer = nullptr;
OpenGLDrawer* opengl_drawer_pointer = nullptr;
OpenGLController* player_1_controller_pointer = nullptr;
OpenGLController* player_2_controller_pointer = nullptr;
Player* player_1_pointer = nullptr;
Player* player_2_pointer = nullptr;

void display(){
  glClear(GL_COLOR_BUFFER_BIT);

  // Superior viewport
  glViewport(0, WINDOW_HEIGHT * WINDOW_FRACTION, WINDOW_WIDTH, WINDOW_HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT * WINDOW_FRACTION, WINDOW_HEIGHT - WINDOW_HEIGHT * WINDOW_FRACTION);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glColor3f(0.5f, 1.0f, 0.5f);

  glBegin(GL_QUADS);
      glVertex2f(0, 0);
      glVertex2f(WINDOW_WIDTH, 0);
      glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
      glVertex2f(0, WINDOW_HEIGHT);
  glEnd();

  // Inferior viewport
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT * WINDOW_FRACTION);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, WORLD_WIDTH, 0, WORLD_HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  map_pointer->draw(*opengl_drawer_pointer);
  
  player_1_pointer->draw(*opengl_drawer_pointer);
  player_2_pointer->draw(*opengl_drawer_pointer);

  glutSwapBuffers();
}

void updateGame(){
 player_1_controller_pointer->processInput(); 
 player_2_controller_pointer->processInput(); 

 glutPostRedisplay();
}

int main (int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("ColetaRun");
  
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  Area map_area = Area(Point(0,0), MAP_WIDTH, MAP_HEIGHT);

  Map map(map_area, PARTITION_WIDTH, PARTITION_HEIGHT, ROOM_MARGIN, HALLWAY_SIZE);

  Player player_1 = Player({0, 0}, 10, 10, map.getWalkableMap());
  OpenGLController player_1_controller(player_1);

  player_1_controller.bindKeys({
      Control{'w', UP, false},
      Control{'s', DOWN, false},
      Control{'a', LEFT, false},
      Control{'d', RIGHT, false},
  });

  Player player_2 = Player({10, 10}, 10, 10, map.getWalkableMap());
  OpenGLController player_2_controller(player_2);

  player_2_controller.bindKeys({
      Control{GLUT_KEY_UP, UP, true},
      Control{GLUT_KEY_DOWN, DOWN, true},
      Control{GLUT_KEY_LEFT, LEFT, true},
      Control{GLUT_KEY_RIGHT, RIGHT, true},
  });

  std::vector<std::reference_wrapper<Player>> players = {player_1, player_2};

  map.addPlayers(players);

  OpenGLDrawer opengl_drawer;

  opengl_drawer.setTileSize(TILE_SIZE);

  map_pointer = &map;
  opengl_drawer_pointer = &opengl_drawer;
  player_1_controller_pointer = &player_1_controller;
  player_1_pointer = &player_1;
  player_2_controller_pointer = &player_2_controller;
  player_2_pointer = &player_2;
  
  glutKeyboardFunc(OpenGLController::keyboardDown);
  glutKeyboardUpFunc(OpenGLController::keyboardUp);
  glutSpecialFunc(OpenGLController::specialKeysDown);
  glutSpecialUpFunc(OpenGLController::specialKeysUp);
  
  glutDisplayFunc(display);
  glutIdleFunc(updateGame);
  glutMainLoop();

  return 0;
}
