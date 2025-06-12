#include <GL/freeglut.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <vector>
#include "domain/game/core.hpp"
#include "adapters/opengl_drawer.hpp"
#include "domain/game/player.hpp"
#include "adapters/opengl_controller.hpp"

const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 688;
const float WINDOW_FRACTION = 0.95;
const int TILE_SIZE = 8;

OpenGLDrawer opengl_drawer;
OpenGLController opengl_controller;
Game game = Game(opengl_drawer,opengl_controller);

const int UPPER_VIEWPORT_HEIGHT = WINDOW_HEIGHT - static_cast<int>(WINDOW_HEIGHT * WINDOW_FRACTION);
const int TIMER_CENTER_X = WINDOW_WIDTH / 2;
const int TIMER_CENTER_Y = UPPER_VIEWPORT_HEIGHT / 2;

const int WORLD_WIDTH = game.getMap().getWidth() * TILE_SIZE;
const int WORLD_HEIGHT = game.getMap().getHeight() * TILE_SIZE;

void display(){
  glClear(GL_COLOR_BUFFER_BIT);

  // Superior viewport
  glViewport(0, WINDOW_HEIGHT * WINDOW_FRACTION, WINDOW_WIDTH, WINDOW_HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT - int(WINDOW_HEIGHT * WINDOW_FRACTION));

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //glColor3f(0.5f, 1.0f, 0.5f);

  //glBegin(GL_QUADS);
      //glVertex2f(0, 0);
      //glVertex2f(WINDOW_WIDTH, 0);
      //glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
      //glVertex2f(0, WINDOW_HEIGHT);
  //glEnd();
  //

  game.getTimer().draw(opengl_drawer);
  opengl_drawer.drawPlayerScore(game.getPlayer1(), Point{0,0}, 100, 60);
  opengl_drawer.drawPlayerScore(game.getPlayer2(), Point{WINDOW_WIDTH - 100,0}, 100, 60);

  // Inferior viewport
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT * WINDOW_FRACTION);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, WORLD_WIDTH, 0, WORLD_HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  game.draw();

  glutSwapBuffers();
}

void updateGame(){
  game.getTimer().update();
  opengl_controller.processInput();
  game.update();

  glutPostRedisplay();
}

int main (int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("ColetaRun");
  
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  game.init();

  game.getPlayer1().bindKeys({
      Control{'w', UP, false},
      Control{'s', DOWN, false},
      Control{'a', LEFT, false},
      Control{'d', RIGHT, false},
  });

  game.getPlayer2().bindKeys({
      Control{GLUT_KEY_UP, UP, true},
      Control{GLUT_KEY_DOWN, DOWN, true},
      Control{GLUT_KEY_LEFT, LEFT, true},
      Control{GLUT_KEY_RIGHT, RIGHT, true},
  });

  game.getTimer().setCoordinate(Point{TIMER_CENTER_X, TIMER_CENTER_Y});

  opengl_drawer.setTileSize(TILE_SIZE);

  glutKeyboardFunc(OpenGLController::keyboardDown);
  glutKeyboardUpFunc(OpenGLController::keyboardUp);
  glutSpecialFunc(OpenGLController::specialKeysDown);
  glutSpecialUpFunc(OpenGLController::specialKeysUp);
  
  glutDisplayFunc(display);
  glutIdleFunc(updateGame);
  glutMainLoop();

  return 0;
}
