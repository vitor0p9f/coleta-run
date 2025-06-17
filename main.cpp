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
#include "adapters/game_state_controller.hpp"
#include "domain/interfaces/menu.hpp"



int WINDOW_WIDTH = 1366;
int WINDOW_HEIGHT = 768;
const float WINDOW_FRACTION = 0.97;
const int FLOOR_SPRITE_SIZE = 16;

GameState gameState = GameState::SPLASH;

OpenGLDrawer opengl_drawer;
OpenGLController opengl_controller;
Game game = Game(opengl_drawer,opengl_controller);

Menu mainMenu({"Iniciar", "Instruções", "Sair"});
Menu pauseMenu({"Continuar","Reiniciar", "Instruções", "Sair"});

const int UPPER_VIEWPORT_HEIGHT = WINDOW_HEIGHT - static_cast<int>(WINDOW_HEIGHT * WINDOW_FRACTION);
const int TIMER_CENTER_X = WINDOW_WIDTH / 2;
const int TIMER_CENTER_Y = UPPER_VIEWPORT_HEIGHT / 2;

const int WORLD_WIDTH = game.getMap().getWidth();
const int WORLD_HEIGHT = game.getMap().getHeight();

void drawSplash();
void drawPauseMenu();
void drawInstructions();
void drawMenu();
void drawEnd();
void keyboardDown(unsigned char key, int x, int y);
void specialKeysDown(int key, int x, int y);


void drawPlay(){
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Draw floor
  for (int y = 0; y < WINDOW_HEIGHT; y += FLOOR_SPRITE_SIZE) {
    for (int x = 0; x < WINDOW_WIDTH; x += FLOOR_SPRITE_SIZE) {

      int draw_width = FLOOR_SPRITE_SIZE;
      int draw_height = FLOOR_SPRITE_SIZE;

      if (x + FLOOR_SPRITE_SIZE > WINDOW_WIDTH) {
          draw_width = WINDOW_WIDTH - x;
      }
      if (y + FLOOR_SPRITE_SIZE > WINDOW_HEIGHT) {
          draw_height = WINDOW_HEIGHT - y;
      }

      opengl_drawer.spriteManager.draw("not_walkable", x, y, draw_width, draw_height);
    }
  }


  // Superior viewport
  glViewport(0, WINDOW_HEIGHT * WINDOW_FRACTION, WINDOW_WIDTH, WINDOW_HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT - int(WINDOW_HEIGHT * WINDOW_FRACTION));

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

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

void reshape(int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;

    glViewport(0, 0, width, height);

    // Atualize a projeção para toda a janela
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    switch (gameState) {
        case GameState::SPLASH: 
             drawSplash(); 
             break;
        case GameState::MENU: 
             drawMenu(); 
             break;
        case GameState::INSTRUCTIONS: 
             drawInstructions(); 
             break;
        case GameState::PLAYING: 
             drawPlay(); 
             break;
        case GameState::PAUSE:
             drawPauseMenu();
             break;
        case GameState::END: 
             drawEnd(); 
             break;
        case GameState::EXIT: 
             exit(0); 
             break;
    }
}

int main (int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("ColetaRun");
  
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  
  opengl_drawer.spriteManager.load("player_1", "sprites/player_1.png");
  opengl_drawer.spriteManager.load("player_2", "sprites/player_2.png");

  opengl_drawer.spriteManager.load("splash", "sprites/splash.png");
  opengl_drawer.spriteManager.load("menu", "sprites/menu.png");
  opengl_drawer.spriteManager.load("instructions", "sprites/instructions.png");
  opengl_drawer.spriteManager.load("end", "sprites/end.png");

  
  opengl_drawer.spriteManager.load("trash_can_paper", "sprites/trash_can_paper.png");
  opengl_drawer.spriteManager.load("trash_can_metal", "sprites/trash_can_metal.png");
  opengl_drawer.spriteManager.load("trash_can_organic", "sprites/trash_can_organic.png");
  opengl_drawer.spriteManager.load("trash_can_glass", "sprites/trash_can_glass.png");
  opengl_drawer.spriteManager.load("trash_can_plastic", "sprites/trash_can_plastic.png");

  opengl_drawer.spriteManager.load("trash_bag_paper", "sprites/trash_bag_paper.png");
  opengl_drawer.spriteManager.load("trash_bag_metal", "sprites/trash_bag_metal.png");
  opengl_drawer.spriteManager.load("trash_bag_organic", "sprites/trash_bag_organic.png");
  opengl_drawer.spriteManager.load("trash_bag_glass", "sprites/trash_bag_glass.png");
  opengl_drawer.spriteManager.load("trash_bag_plastic", "sprites/trash_bag_plastic.png");

  opengl_drawer.spriteManager.load("walkable", "sprites/floor.png");
  opengl_drawer.spriteManager.load("not_walkable", "sprites/grass.png");
  
  game.init();
  game.setGameState(&gameState);

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

  glutKeyboardFunc(keyboardDown);
  glutKeyboardUpFunc(OpenGLController::keyboardUp);
  glutSpecialFunc(specialKeysDown);
  glutSpecialUpFunc(OpenGLController::specialKeysUp);
  
  glutDisplayFunc(display);
  glutIdleFunc(updateGame);
  glutReshapeFunc(reshape);
  glutMainLoop();

  return 0;
}
