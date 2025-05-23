#pragma once

#include <memory>
#include <vector>
#include "../types.hpp"

enum class SplitDirection{
  VERTICAL,
  HORIZONTAL
};

struct Node {
  std::unique_ptr<Node> left = nullptr;
  std::unique_ptr<Node> right = nullptr;

  Area area;

  Node(){};

  explicit Node(Node* left_pointer, Node* right_pointer, Area area): left(left_pointer), right(right_pointer), area(area){};

  Node(const Node& other) : area(other.area){
      if (other.left)
          left = std::make_unique<Node>(*other.left);
      if (other.right)
          right = std::make_unique<Node>(*other.right);
  }
};

class BSPTree{
  public:
    explicit BSPTree(Area initial_area);

    Node* getRoot() const { return root.get(); }

    std::vector<Node> getLeafs();

  private:
    std::unique_ptr<Node> root = nullptr;
};
