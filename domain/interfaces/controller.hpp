#pragma once

#include "controllable.hpp"
#include <vector>

class Control;

class IController{
  public:
    virtual ~IController() = default;

    IController(
        Controllable& element, 
        const std::vector<std::vector<bool>>& walkable_map
    ) : element(element), walkable_map(walkable_map) {};
   
    virtual void bindKeys(std::vector<Control> controls) = 0;

  protected:
    Controllable& element;
    const std::vector<std::vector<bool>>& walkable_map;
};
