// game_state_controller.cpp
#include <GL/freeglut.h>
#include "types.hpp"
#include "game_state_controller.hpp"
#include "core.hpp"
#include "opengl_drawer.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <vector>
#include "player.hpp"
#include "opengl_controller.hpp"

const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 688;
const float WINDOW_FRACTION = 0.95;
const int TILE_SIZE = 8;

const int UPPER_VIEWPORT_HEIGHT = WINDOW_HEIGHT - static_cast<int>(WINDOW_HEIGHT * WINDOW_FRACTION);
const int TIMER_CENTER_X = WINDOW_WIDTH / 2;
const int TIMER_CENTER_Y = UPPER_VIEWPORT_HEIGHT / 2;

extern OpenGLDrawer opengl_drawer;
extern OpenGLController opengl_controller;
extern Game game;
extern GameState gameState;

const int WORLD_WIDTH = game.getMap().getWidth() * TILE_SIZE;
const int WORLD_HEIGHT = game.getMap().getHeight() * TILE_SIZE;

void drawText(const char* text, int x, int y) {
    glRasterPos2i(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawMainMenu() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // fundo preto
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Configura viewport padrão (tela cheia)
    glColor3f(1.0, 1.0, 1.0); // texto branco
    drawText("COLETA RUN", 100, 300);
    drawText("Pressione ENTER para comecar", 100, 250);
    glutSwapBuffers();
}

void drawPlaying() {

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
     glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Definir cor de fundo para branco antes!
     glClear(GL_COLOR_BUFFER_BIT);     // Agora sim limpa com branco
     
        // Superior viewport
        glViewport(0, WINDOW_HEIGHT * WINDOW_FRACTION, WINDOW_WIDTH, WINDOW_HEIGHT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT - int(WINDOW_HEIGHT * WINDOW_FRACTION));

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glColor3f(0.0f, 0.0f, 0.0f);
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

void drawGameOverScreen() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // fundo preto
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Configura viewport padrão (tela cheia)
    glColor3f(1.0, 1.0, 1.0); // texto branco
    drawText("FIM DE JOGO!", 100, 300);
    drawText("Pressione R para reiniciar", 100, 250);
    glutSwapBuffers();
}

void display() {
    
    switch (gameState) {
        case GameState::MENU:
            drawMainMenu();
            break;

        case GameState::PLAYING:
            drawPlaying();
            break;

        case GameState::GAME_OVER:
            drawGameOverScreen();
            break;
    }
}


void updateGame(){
  game.getTimer().update();
  opengl_controller.processInput();
  game.update();

  glutPostRedisplay();
}

/*
void keyboard(unsigned char key, int x, int y) {
    if (gameState == GameState::MENU && key == 13) { // ENTER
        gameState = GameState::PLAYING;
    } else if (gameState == GameState::GAME_OVER && (key == 'r' || key == 'R')) {
        game.restart();
        gameState = GameState::PLAYING;
    } else if (key == 27) { // ESC
        exit(0);
    }
}
*/
