#pragma once

#include "../interfaces/drawable.hpp"
#include "../interfaces/controllable.hpp"
#include "types.hpp"
#include <cstdio>
#include <vector>

struct Player: public Drawable, public Controllable {
  int step_size = 2;
  const std::vector<std::vector<bool>>& walkable_map;

  Player();

  Player(
      Point coordinate, 
      int width, 
      int height, 
      const std::vector<std::vector<bool>>& walkable_map
  ) : Drawable(coordinate, width, height), walkable_map(walkable_map){}

  void draw(const IDrawer& drawer) const override {
    drawer.drawPlayer(*this);
  }

  virtual void setStepSize(int step_size) override {
    this->step_size = step_size;
  }

  virtual void move(Direction direction) override {
    int map_width = walkable_map[0].size();
    int map_height = walkable_map.size();

    int target_x = coordinate.x;
    int target_y = coordinate.y;

    switch (direction) {
        case UP:    target_y = coordinate.y + step_size; break;
        case DOWN:  target_y = coordinate.y - step_size; break;
        case LEFT:  target_x = coordinate.x - step_size; break;
        case RIGHT: target_x = coordinate.x + step_size; break;
    }

    bool can_move = true;

    for (int y_offset = 0; y_offset < height; ++y_offset) {
      for (int x_offset = 0; x_offset < width; ++x_offset) {
        int check_x = target_x + x_offset;
        int check_y = target_y + y_offset;

        if (check_x < 0 or check_x >= map_width or
            check_y < 0 or check_y >= map_height or
            !walkable_map[check_y][check_x]
        ) {
            can_move = false;
            break;
        }
      }

      if (!can_move) break;
    }

    if (can_move) {
      coordinate.x = target_x;
      coordinate.y = target_y;
    }
  }
};
