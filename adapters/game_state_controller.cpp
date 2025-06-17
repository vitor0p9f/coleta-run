#include <GL/freeglut.h>
#include "types.hpp"
#include "game_state_controller.hpp"
#include "core.hpp"
#include "opengl_drawer.hpp"
#include <cstdio>
#include <vector>
#include <string>
#include "player.hpp"
#include "opengl_controller.hpp"
#include "../domain/interfaces/menu.hpp"
#include "../domain/interfaces/windows_config.hpp"

int splashStartTime = 0;
const float WINDOW_FRACTION = 0.97;
const int TILE_SIZE = 8;

const int UPPER_VIEWPORT_HEIGHT = WINDOW_HEIGHT - static_cast<int>(WINDOW_HEIGHT * WINDOW_FRACTION);
const int TIMER_CENTER_X = WINDOW_WIDTH / 2;
const int TIMER_CENTER_Y = UPPER_VIEWPORT_HEIGHT / 2;

extern OpenGLDrawer opengl_drawer;
extern OpenGLController opengl_controller;
extern Game game;
extern GameState gameState;
extern Menu mainMenu;
extern Menu pauseMenu;

const int WORLD_WIDTH = game.getMap().getWidth() * TILE_SIZE;
const int WORLD_HEIGHT = game.getMap().getHeight() * TILE_SIZE;

void drawText(const char* text, int x, int y) {
    glRasterPos2i(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void drawStrokeText(const char* text, float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.3f); 
    glLineWidth(3.0f); // 

    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);
    }
    
    glPopMatrix();
}


void drawSplash(){
  
    glClear(GL_COLOR_BUFFER_BIT); 
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    opengl_drawer.spriteManager.draw("splash", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glutSwapBuffers();
}

void drawMenuOptions(const std::vector<std::string>& options, int selectedOption, float startX, float startY, float spacing, float scale) {
    for (int i = 0; i < static_cast<int>(options.size()); i++) {
        float x = startX;
        float y = startY + i * spacing;

        if (selectedOption == i) {
            glColor3f(1.0, 1.0, 0.0); 
            glBegin(GL_LINE_LOOP);
                glVertex2f(x - 10, y - 5);
                glVertex2f(x + 400, y - 5); 
                glVertex2f(x + 400, y + 50);
                glVertex2f(x - 10, y + 50);
            glEnd();
            glColor3f(1.0, 1.0, 1.0); 
        }

        drawStrokeText(options[i].c_str(), x, y, scale);
    }
}


void drawPauseMenu() {
  
    glClear(GL_COLOR_BUFFER_BIT); 
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    opengl_drawer.spriteManager.draw("menu", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    std::vector<std::string> options = {"Continuar", "Instrucoes", "Sair"};

    float startY = WINDOW_HEIGHT/2 - 80;
    float spacing = -80;
    float scale = 0.3f;

    drawMenuOptions(options, pauseMenu.getSelectedOption(), WINDOW_WIDTH/2 + 60, startY, spacing, scale);

    glutSwapBuffers();
}

void drawInstructions(){
  
    glClear(GL_COLOR_BUFFER_BIT); 
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    opengl_drawer.spriteManager.draw("instructions", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glutSwapBuffers();
}

void drawMenu(){
  
    glClear(GL_COLOR_BUFFER_BIT); 
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    opengl_drawer.spriteManager.draw("menu", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    std::vector<std::string> options = {"Iniciar", "Instrucoes", "Sair"};

    float startY = WINDOW_HEIGHT/2 - 80;
    float spacing = -80;
    float scale = 0.3f;

    drawMenuOptions(options, mainMenu.getSelectedOption(), WINDOW_WIDTH/2 + 60, startY, spacing, scale);

    glutSwapBuffers();
}

void drawEnd(){
  
    glClear(GL_COLOR_BUFFER_BIT); 
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    opengl_drawer.spriteManager.draw("end", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    GameResult result = game.getResult();

    switch (result) {
        case GameResult::Player1Win:
            drawText("Parabéns! Player 1 ganhou!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2);
            break;
        case GameResult::Player2Win:
            drawText("Parabéns! Player 2 ganhou!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2);
            break;
        case GameResult::Tie:
            drawText("Empate!", WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2);
            break;
        default:
            drawText("Fim do jogo", WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2);
            break;
    }
   
    glutSwapBuffers();
}

void updateGame() {
    game.getTimer().update();

       if (gameState == GameState::SPLASH) {
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        if (splashStartTime == 0) splashStartTime = currentTime; 

        if (currentTime - splashStartTime > 2000) { 
            gameState = GameState::MENU;
        }

        glutPostRedisplay();
        return; 
    }

    opengl_controller.processInput();
    game.update();

    glutPostRedisplay();
}

void keyboardDown(unsigned char key, int x, int y) {
    key = tolower(key);

    switch (gameState) {
        case GameState::SPLASH:
            return;
            break;

        case GameState::MENU:
            if (key == 13) { // ENTER
                int selected = mainMenu.getSelectedOption();
                if (selected == 0) gameState = GameState::PLAYING;
                else if (selected == 1) gameState = GameState::INSTRUCTIONS;
                else if (selected == 2) gameState = GameState::EXIT;
            }
            break;

        case GameState::INSTRUCTIONS:
            if (key == 27) gameState = GameState::MENU; // ESC volta menu
            break;

            case GameState::PLAYING:
            if (key == ' ') {
                gameState = GameState::PAUSE;
            } else {
                opengl_controller.keyboardDown(key, x, y);  
            }
            break;

        case GameState::PAUSE:
            if (key == 13) { // ENTER
                int sel = pauseMenu.getSelectedOption();
                if (sel == 0) gameState = GameState::PLAYING;       // Reiniciar
                //else if (sel == 1) gameState = GameState::PLAYING;
                else if (sel == 1) gameState = GameState::INSTRUCTIONS;
                else if (sel == 2) gameState = GameState::MENU;
            } else if (key == 27) gameState = GameState::PLAYING;  // ESC resume
            break;

        case GameState::END:
            if (key == 'r') gameState = GameState::PLAYING;
            else if (key == 27) gameState = GameState::MENU;
            break;

        case GameState::EXIT:
            exit(0);
            break;
    }

    glutPostRedisplay();
}

void specialKeysDown(int key, int x, int y) {
    switch (gameState) {
        case GameState::MENU:
            if (key == GLUT_KEY_UP) mainMenu.moveUp();
            else if (key == GLUT_KEY_DOWN) mainMenu.moveDown();
            break;

            
        case GameState::PLAYING:
            opengl_controller.specialKeysDown(key, x, y); 
            break;

        case GameState::PAUSE:
            if (key == GLUT_KEY_UP) pauseMenu.moveUp();
            else if (key == GLUT_KEY_DOWN) pauseMenu.moveDown();
            break;

        default:
            break;
    }

    glutPostRedisplay();
}