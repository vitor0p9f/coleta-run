#include "bsp.hpp"
#include "hallway.hpp"
#include "room.hpp"
#include <cstdlib>
#include <functional>
#include <memory>
#include <random>
#include <vector>
#include <random>

BSP::BSP() : random_device(), generator(random_device()){}

std::tuple<std::vector<std::reference_wrapper<Room>>, std::vector<Hallway>> BSP::execute(
  Area initial_area, 
  int partition_width, 
  int partition_height, 
  int room_margin, 
  int hallway_size
){
  Node tree_root;

  root = &tree_root;
  root->area = initial_area;

  splitArea(root, partition_width, partition_height);
 
  std::vector<std::reference_wrapper<Room>> rooms = getRooms(root, room_margin);
  std::vector<Hallway> hallways = connectRooms(hallway_size, room_margin);

  return std::make_pair(rooms, hallways);
}

void BSP::splitArea(Node* node, int partition_width, int partition_height){
  int width = node->area.width;
  int height = node->area.height;

  bool can_split_vertically = (width > 2 * partition_width);
  bool can_split_horizontally = (height > 2 * partition_height);

  if (!can_split_vertically && !can_split_horizontally) {
      return;
  }

  if(!areaIsDivisible(width, height, partition_width, partition_height, 2)) return;

  SplitDirection split_direction;
  int split_position;

  if(width > height){
    split_direction = can_split_vertically ? 
      SplitDirection::VERTICAL : SplitDirection::HORIZONTAL;
  }else if(height > width){
    split_direction = can_split_horizontally ? 
      SplitDirection::HORIZONTAL : SplitDirection::VERTICAL;
  }else{
    if (can_split_vertically && can_split_horizontally) {
      std::uniform_int_distribution<> dist(0, 1);
      
      split_direction = (dist(generator) == 0) ? 
        SplitDirection::VERTICAL : SplitDirection::HORIZONTAL;
    } else if (can_split_vertically) {
        split_direction = SplitDirection::VERTICAL;
    } else {
        split_direction = SplitDirection::HORIZONTAL;
    }
  }

  if(split_direction == SplitDirection::VERTICAL){
    std::uniform_int_distribution<int> distribution(
      partition_width, (width - partition_width)
    );

    split_position = distribution(generator);

    Area left_area = Area(node->area.coordinate, split_position, height);
    Area right_area = Area(
      Point(node->area.coordinate.x + split_position, node->area.coordinate.y),
      width - split_position,
      height
    );

    node->left_node = std::make_unique<Node>(left_area);
    node->right_node = std::make_unique<Node>(right_area);
  }else{
    std::uniform_int_distribution<int> distribution(
      partition_height, (height - partition_height)
    );

    split_position = distribution(generator);

    Area top_area = Area(
      Point(node->area.coordinate.x, node->area.coordinate.y + split_position),
      width,
      height - split_position
    );
    Area bottom_area = Area(node->area.coordinate, width, split_position);

    node->left_node = std::make_unique<Node>(top_area);
    node->right_node = std::make_unique<Node>(bottom_area);
  } 

  splitArea(node->left_node.get(), partition_width, partition_height);
  splitArea(node->right_node.get(), partition_width, partition_height);
}

bool BSP::areaIsDivisible(
  int width, 
  int height, 
  int partition_width, 
  int partition_height, 
  int multiplication_factor
){
  int width_boundarie = partition_width * multiplication_factor;
  int height_boundarie = partition_height * multiplication_factor;

  return ((width > width_boundarie) or (height > height_boundarie));
}

void BSP::generateRoomsRecursively(
    Node* node, std::vector<std::reference_wrapper<Room>>& rooms, int room_margin
){
  if(!node){
    return;
  }

  if(!node->left_node and !node->right_node){
    Point room_coordinate = Point(
      node->area.coordinate.x + room_margin, 
      node->area.coordinate.y + room_margin
    );
    int room_width = node->area.width - (2 * room_margin);
    int room_height = node->area.height - (2 * room_margin);

    if (room_width > 0 && room_height > 0) {
      node->room = Room(room_coordinate, room_width, room_height);
      
      rooms.push_back(node->room);
    }
  }else{
    if(node->left_node){
      generateRoomsRecursively(node->left_node.get(), rooms, room_margin);
    }

    if(node->right_node){
      generateRoomsRecursively(node->right_node.get(), rooms, room_margin);
    }
  }
}
    
std::vector<std::reference_wrapper<Room>> BSP::getRooms(Node* root, int room_margin){
  std::vector<std::reference_wrapper<Room>> rooms;

  generateRoomsRecursively(root, rooms, room_margin);

  return rooms;
}

bool BSP::isLeaf(const Node* node) const {
  if (!node) return false;
  
  return !node->left_node && !node->right_node;
}

const Node* BSP::getRandomLeaf(const Node* node) {
    if (!node) return nullptr;

    std::vector<const Node*> leafs;

    std::function<void(const Node*)> collect_leafs = [&](const Node* current_node) {
        if (!current_node) return;

        if (isLeaf(current_node)) {
          leafs.emplace_back(current_node);
        } else {
          if (current_node->left_node) {
              collect_leafs(current_node->left_node.get());
          }

          if (current_node->right_node) {
              collect_leafs(current_node->right_node.get());
          }
        }
    };

    collect_leafs(node);

    if (leafs.empty()) {
        return nullptr;
    }

    std::uniform_int_distribution<size_t> dist(0, leafs.size() - 1);

    return leafs[dist(generator)];
}

void BSP::generateHallwaysRecursively(
  Node* root, std::vector<Hallway>& hallways, int hallway_size, int room_margin
){
  if(!root or !root->left_node or !root->right_node){
    return;
  }

  generateHallwaysRecursively(root->left_node.get(), hallways, hallway_size, room_margin);
  generateHallwaysRecursively(root->right_node.get(), hallways, hallway_size, room_margin);

  const Node* left_leaf = getRandomLeaf(root->left_node.get());
  const Node* right_leaf = getRandomLeaf(root->right_node.get());

  if (!left_leaf or !right_leaf){
    return;
  }

  const Room& room_1 = left_leaf->room; 
  const Room& room_2 = right_leaf->room;

  if(generator() % 2 == 0){
    // Horizontal first and vertical after it
    hallways.emplace_back(Hallway(
      Point(std::min(room_1.center.x, room_2.center.x), room_1.center.y - hallway_size / 2),
      std::abs(room_1.center.x - room_2.center.x) == 0 ? 
        hallway_size : std::abs(room_1.center.x - room_2.center.x),
      hallway_size,
      hallway_size
    ));

    hallways.emplace_back(Hallway(
      Point(room_2.center.x - hallway_size / 2, std::min(room_1.center.y, room_2.center.y)),
      hallway_size,
      std::abs(room_1.center.y - room_2.center.y) == 0 ? 
        hallway_size : std::abs(room_1.center.y - room_2.center.y),
      hallway_size
    ));
  }else{
    hallways.emplace_back(Hallway(
      Point(room_1.center.x - hallway_size / 2, std::min(room_1.center.y, room_2.center.y)),
      hallway_size,
      std::abs(room_1.center.y - room_2.center.y) == 0 ? 
        hallway_size : std::abs(room_1.center.y - room_2.center.y),
      hallway_size
    ));

    hallways.emplace_back(Hallway(
      Point(std::min(room_1.center.x, room_2.center.x), room_2.center.y - hallway_size / 2),
      std::abs(room_1.center.x - room_2.center.x) == 0 ? 
        hallway_size : std::abs(room_1.center.x - room_2.center.x),
      hallway_size,
      hallway_size
    ));
  }
}

std::vector<Hallway> BSP::connectRooms(
    int hallway_size, 
    int room_margin
){
  std::vector<Hallway> hallways;

  generateHallwaysRecursively(root, hallways, hallway_size, room_margin);

  return hallways;
}
