#include "bsp_tree.hpp"
#include <memory>
#include <queue>

BSPTree::BSPTree(Area initial_area): root(std::make_unique<Node>()){
  root->area = initial_area;
}

std::vector<Node> BSPTree::getLeafs() {
    std::vector<Node> leafs;
    std::queue<Node*> queue;

    queue.push(root.get());

    while (!queue.empty()) {
      Node* current_node = queue.front();
      
      queue.pop();

      if (!current_node->left && !current_node->right) {
        leafs.push_back(*current_node);
      } else {
        if (current_node->left) queue.push(current_node->left.get());
        if (current_node->right) queue.push(current_node->right.get());
      }
    }
    return leafs;
  }
