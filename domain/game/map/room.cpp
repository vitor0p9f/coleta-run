#include "room.hpp"

Room::Room(Area area){
  generate(area);
}

void Room::draw(const IDrawer& drawer) const{
  drawer.drawRoom(*this);
};

void Room::generate(Area area){
  std::random_device random_device;
  std::mt19937 generator(random_device());

  int max_width = std::max(area.width - 25, area.width - 10);
  int max_height = std::max(area.height - 25, area.height - 10);

  std::uniform_int_distribution<> width_distribution(area.width - 50, max_width);
  std::uniform_int_distribution<> height_distribution(area.height -50 , max_height);

  int room_width = width_distribution(generator);
  int room_height = height_distribution(generator);

  int max_x_offset = area.width - room_width;
  int max_y_offset = area.height - room_height;

  std::uniform_int_distribution<> x_offset_distribution(0, max_x_offset);
  std::uniform_int_distribution<> y_offset_distribution(0, max_y_offset);

  int offset_x = x_offset_distribution(generator);
  int offset_y = y_offset_distribution(generator);

  coordinate = {.x = area.coordinate.x + offset_x, .y = area.coordinate.y + offset_y};
  width = room_width;
  height = room_height;
  center = {.x = area.coordinate.x + area.width / 2, .y = area.coordinate.y + area.height / 2};
}
