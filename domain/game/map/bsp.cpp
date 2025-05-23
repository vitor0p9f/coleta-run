#include "bsp.hpp"
#include "bsp_tree.hpp"
#include "hallway.hpp"
#include "room.hpp"
#include <cstdio>
#include <memory>
#include <random>
#include <tuple>
#include <vector>

BSP::BSP(){
}

std::tuple<std::vector<Room>, std::vector<Hallway>> BSP::execute(Area initial_area, int min_room_size){
  BSPTree bsp_tree(initial_area);

  splitArea(bsp_tree.getRoot(), min_room_size);

  std::vector<Node> leafs = bsp_tree.getLeafs();

  std::vector<Room> rooms = generateRooms(leafs);

  std::vector<Hallway> hallways = connectRoomsByNearest(rooms, 100);

  return std::make_tuple(rooms, hallways);
}

void BSP::splitArea(Node* node, int min_room_size){
  int width = node->area.width;
  int height = node->area.height;

  if(!areaIsDivisible(width, height, min_room_size, 2)) return;

  SplitDirection split_direction;
  int split_position;

  std::random_device random_device;
  std::mt19937 generator(random_device());

  if(width > height){
    split_direction = SplitDirection::VERTICAL;
  }else if(height > width){
    split_direction = SplitDirection::HORIZONTAL;
  }else{
    std::uniform_int_distribution<> distribution(0, 1);
    
    split_direction = distribution(generator) == 0 ? SplitDirection::VERTICAL : SplitDirection::HORIZONTAL;
  }

  if(split_direction == SplitDirection::VERTICAL){
    std::uniform_int_distribution<int> distribution(min_room_size, (width - min_room_size));

    split_position = distribution(generator);

    Area left_area = {
      .coordinate = node->area.coordinate,
      .width = split_position,
      .height = height
    };
    Area right_area = {
      .coordinate = {
        .x = (node->area.coordinate.x + split_position),
        .y = node->area.coordinate.y
      },
      .width = width - split_position,
      .height = height
    };

    node->left = std::make_unique<Node>(Node{nullptr, nullptr, left_area});
    node->right = std::make_unique<Node>(Node{nullptr, nullptr, right_area});
  }else{
    std::uniform_int_distribution<int> distribution(min_room_size, (height - min_room_size));

    split_position = distribution(generator);

    Area top_area = {
      .coordinate = {
        .x = node->area.coordinate.x,
        .y = node->area.coordinate.y + split_position
      },
      .width = width,
      .height = height - split_position
    };
    Area bottom_area = {
      .coordinate = node->area.coordinate,
      .width = width,
      .height = split_position

    };

    node->left = std::make_unique<Node>(Node{ nullptr, nullptr, top_area });
    node->right = std::make_unique<Node>(Node{ nullptr, nullptr, bottom_area });
  } 

  splitArea(node->left.get(), min_room_size);
  splitArea(node->right.get(), min_room_size);
}

bool BSP::areaIsDivisible(int width, int height, int min_room_size, int multiplication_factor){
  int boundarie = min_room_size * multiplication_factor;

  return ((width > boundarie) or (height > boundarie));
}

std::vector<Room> BSP::generateRooms(const std::vector<Node>& leafs){
  std::vector<Room> rooms;

  for(Node leaf : leafs){
        Room room(leaf.area);

        rooms.push_back(room);

        leafs_and_rooms.emplace(&leaf, room);
  }

  return rooms;
}

double distance(const Coordinate& a, const Coordinate& b) {
  return std::sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

std::vector<Hallway> BSP::connectRoomsByNearest(const std::vector<Room>& rooms, int hallway_width) {
  std::vector<Hallway> hallways;
  if (rooms.size() < 2) return hallways;

  std::vector<bool> connected(rooms.size(), false);
  connected[0] = true;  // começamos conectando a primeira sala

  int connected_count = 1;

  while (connected_count < (int)rooms.size()) {
    double min_dist = std::numeric_limits<double>::max();
    int room_a = -1, room_b = -1;

    for (int i = 0; i < (int)rooms.size(); i++) {
      if (!connected[i]) continue;

      for (int j = 0; j < (int)rooms.size(); j++) {
        if (connected[j]) continue;

        double dist = distance(rooms[i].center, rooms[j].center);
        if (dist < min_dist) {
          min_dist = dist;
          room_a = i;
          room_b = j;
        }
      }
    }

    Coordinate center_1 = rooms[room_a].center;
    Coordinate center_2 = rooms[room_b].center;
    Coordinate corner = { center_2.x, center_1.y };

    hallways.emplace_back(Hallway(center_1, corner, hallway_width));
    hallways.emplace_back(Hallway(corner, center_2, hallway_width));

    printf("%d\n", hallways[0].start.y);

    connected[room_b] = true;
    connected_count++;
  }

  for(Hallway hallway : hallways){
        printf("y: %d\n", hallway.start.y);
  }

  return hallways;
}
