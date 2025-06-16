#pragma once

#include "../interfaces/drawable.hpp"
#include "../interfaces/controllable.hpp"
#include "map/map.hpp"
#include "types.hpp"
#include "trash_bag.hpp"
#include <optional>

struct Player: public Drawable, public Controllable {
  int step_size = 2;
  const WalkableMap& walkable_map;
  std::optional<TrashBag> carried_bag;
  int score = 0;
  int id;

  Player(
      Point coordinate, 
      int width, 
      int height,
      int id,
      const WalkableMap& walkable_map
  ) : Drawable(coordinate, width, height), walkable_map(walkable_map), id(id){}

  void draw(const IDrawer& drawer) const override {
    drawer.drawPlayer(*this);

    if (carried_bag) {
      drawer.drawTrashBag(*carried_bag);
    }
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

      if (carried_bag) {
          carried_bag->setCoordinate(coordinate);
      }
    }
  }

  void attachBag(const TrashBag& bag) {
    carried_bag = bag;

    if (carried_bag) {
        carried_bag->setCoordinate(coordinate);
    }
  }

  void detachBag() {
      carried_bag.reset();
  }

  void increaseScore(int points){
    score += points;
  }

  int getScore() const {
    return score;
  }
};
