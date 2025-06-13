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
#include <string>
#include "player.hpp"
#include "opengl_controller.hpp"
#include "../domain/interfaces/menu.hpp"

int splashStartTime = 0;
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
extern Menu mainMenu;
extern Menu startMenu;
extern Menu pauseMenu;

const int WORLD_WIDTH = game.getMap().getWidth() * TILE_SIZE;
const int WORLD_HEIGHT = game.getMap().getHeight() * TILE_SIZE;

void drawText(const char* text, int x, int y) {
    glRasterPos2i(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawMenuOptions(const std::vector<std::string>& options, int selectedOption, int startX, int startY, int spacing) {
    for (int i = 0; i < static_cast<int>(options.size()); i++) {
        int x = startX;
        int y = startY + i * spacing;

        // Desenha contorno amarelo se estiver selecionado
        if (selectedOption == i) {
            glColor3f(1.0, 1.0, 0.0); // amarelo
            glBegin(GL_LINE_LOOP);
                glVertex2f(x - 10, y - 5);
                glVertex2f(x + 200, y - 5);
                glVertex2f(x + 200, y + 20);
                glVertex2f(x - 10, y + 20);
            glEnd();
            glColor3f(1.0, 1.0, 1.0); // volta a branco
        }

        drawText(options[i].c_str(), x, y);
    }
}

void drawSplash() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    glColor3f(1.0, 1.0, 1.0); 
    drawText("Bem vindo ao Coleta Run!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2);
    
    glutSwapBuffers();
}

void drawMainMenu() {
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    glColor3f(1.0, 1.0, 1.0); 

    drawText("COLETA RUN", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 80 );

    std::vector<std::string> options = {"Iniciar", "Instrucoes", "Sair"};

    int startY = WINDOW_HEIGHT/2 + 40; 
    int spacing = -40;                 

    drawMenuOptions(options, mainMenu.getSelectedOption(), WINDOW_WIDTH/2 - 100, startY, spacing);

    glutSwapBuffers();
}

void drawStartMenu() {
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    glColor3f(1.0, 1.0, 1.0);
    drawText("Escolha o modo:", WINDOW_WIDTH/2 - 70, WINDOW_HEIGHT/2 + 80);
    std::vector<std::string> options = {"1 Player", "2 Player","Voltar"};

    int startY = WINDOW_HEIGHT/2 + 40; 
    int spacing = -40;                 

    drawMenuOptions(options, startMenu.getSelectedOption(), WINDOW_WIDTH/2 - 100, startY, spacing);

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

void drawInstructions() {
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    glColor3f(1.0, 1.0, 1.0);
    drawText("Instruções do jogo:", 50, WINDOW_HEIGHT - 50);
    drawText("- Use setas para mover", 50, WINDOW_HEIGHT - 80);
    drawText("- Colete os itens para ganhar pontos", 50, WINDOW_HEIGHT - 110);
    drawText("- Pressione ESC para voltar", 50, WINDOW_HEIGHT - 140);

    glutSwapBuffers();
}

void drawPauseMenu() {
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    glColor3f(1.0, 1.0, 1.0);
    drawText("Pausado", WINDOW_WIDTH/2 - 30, WINDOW_HEIGHT/2 + 120);
    std::vector<std::string> options = {"Continuar", "Reiniciar","Instrucoes","Sair"};

    int startY = WINDOW_HEIGHT/2 + 80; 
    int spacing = -40;                 

    drawMenuOptions(options, pauseMenu.getSelectedOption(), WINDOW_WIDTH/2 - 100, startY, spacing);

    glutSwapBuffers();
}

void drawWinScreen() {
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    glColor3f(1.0, 1.0, 1.0);
    drawText("Parabéns! Você ganhou!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2);
    drawText("Pressione R para reiniciar ou ESC para sair", WINDOW_WIDTH/2 - 170, WINDOW_HEIGHT/2 - 40);

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
        case GameState::SPLASH: 
             drawSplash(); 
             break;
        case GameState::MENU: 
             drawMainMenu(); 
             break;
        case GameState::START_MENU: 
             drawStartMenu(); 
             break;
        case GameState::INSTRUCTIONS: 
             drawInstructions(); 
             break;
        case GameState::PLAYING: 
             drawPlaying(); 
             break;
        case GameState::PAUSE_MENU:
             drawPauseMenu();
             break;
        case GameState::WIN_SCREEN: 
             drawWinScreen(); 
             break;
        case GameState::GAME_OVER:
            drawGameOverScreen();
            break;
        case GameState::EXIT: 
             exit(0); 
             break;
    }
}

void updateGame() {
    game.getTimer().update();

    // Executa a Splash por 3s
    if (gameState == GameState::SPLASH) {
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        if (splashStartTime == 0) splashStartTime = currentTime; 

        if (currentTime - splashStartTime > 3000) { 
            gameState = GameState::MENU;
        }

        glutPostRedisplay();
        return; 
    }

    // Apos Splash, segue o jogo normalmente
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
                if (selected == 0) gameState = GameState::START_MENU;
                else if (selected == 1) gameState = GameState::INSTRUCTIONS;
                else if (selected == 2) gameState = GameState::EXIT;
            }
            break;

        case GameState::START_MENU:
            if (key == 13) {
                int sel = startMenu.getSelectedOption();
                if (sel == 0) gameState = GameState::PLAYING;
                else if (sel == 1) gameState = GameState::PLAYING;
                else if (sel == 2) gameState = GameState::MENU;
            }
            break;

        case GameState::INSTRUCTIONS:
            if (key == 27) gameState = GameState::MENU; // ESC volta menu
            break;

        case GameState::PLAYING:
            if (key == ' ') gameState = GameState::PAUSE_MENU;
            break;

        case GameState::PAUSE_MENU:
            if (key == 13) { // ENTER
                int sel = pauseMenu.getSelectedOption();
                if (sel == 0) gameState = GameState::PLAYING;       // Reiniciar
                else if (sel == 1) gameState = GameState::PLAYING;
                else if (sel == 2) gameState = GameState::INSTRUCTIONS;
                else if (sel == 3) gameState = GameState::MENU;
            } else if (key == 27) gameState = GameState::PLAYING;  // ESC resume
            break;

        case GameState::WIN_SCREEN:
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

        case GameState::START_MENU:
            if (key == GLUT_KEY_UP) startMenu.moveUp();
            else if (key == GLUT_KEY_DOWN) startMenu.moveDown();
            break;

        case GameState::PAUSE_MENU:
            if (key == GLUT_KEY_UP) pauseMenu.moveUp();
            else if (key == GLUT_KEY_DOWN) pauseMenu.moveDown();
            break;

        default:
            break;
    }

    glutPostRedisplay();
}