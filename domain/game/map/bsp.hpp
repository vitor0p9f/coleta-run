#pragma once

#include "room.hpp"
#include "hallway.hpp"
#include <functional>
#include <memory>
#include <random>
#include <vector>

enum class SplitDirection{
  VERTICAL,
  HORIZONTAL
};

struct Node{
  std::unique_ptr<Node> left_node = nullptr;
  std::unique_ptr<Node> right_node = nullptr;
  Area area;
  Room room;
  
  Node() : area(), room() {}

  explicit Node(Area initial_area) : area(initial_area), room() {}
};

class BSP{
  public:
    BSP();

    std::tuple<std::vector<std::reference_wrapper<Room>>, std::vector<Hallway>> execute(
      Area initial_area, 
      int partition_width, 
      int partition_height, 
      int room_margin, 
      int hallway_size
    );

  private:
    Node* root = nullptr;
    std::random_device random_device;
    std::mt19937 generator;

    void splitArea(Node* node, int partition_width, int partition_height);
    
    bool areaIsDivisible(
      int width, 
      int height, 
      int partition_width, 
      int partition_height, 
      int multiplication_factor
    );
    
    void generateRoomsRecursively(
      Node* root, 
      std::vector<std::reference_wrapper<Room>>& rooms, 
      int room_margin
    );
    
    void generateHallwaysRecursively(
      Node* root, std::vector<Hallway>& hallways, int hallway_size, int room_margin
    );

    std::vector<Hallway> connectRooms(int hallway_size, int room_margin);

    std::vector<std::reference_wrapper<Room>> getRooms(Node* root, int room_margin);
    
    bool isLeaf(const Node* node) const;
    
    const Node* getRandomLeaf(const Node* node);
};
