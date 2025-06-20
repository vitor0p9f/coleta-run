#pragma once

enum Category{
  PAPER,
  PLASTIC,
  GLASS,
  METAL,
  ORGANIC
};

enum class GameState {
  SPLASH,
  MENU,
  PAUSE,
  INSTRUCTIONS,
  PLAYING,
  END,
  EXIT
};

enum class GameResult {
  Player1Win,
  Player2Win,
  Tie,
  Ongoing
};


struct Point {
  int x, y;

 Point() : x(0), y(0) {} 

 Point(int x, int y) : x(x), y(y) {} 
};

struct Area{
  Point coordinate;
  int width, height;
  
  Area() : coordinate(), width(0), height(0) {}

  Area(Point coordinate, int width, int height) : 
    coordinate(coordinate), width(width), height(height) {}
};
