#pragma once

#include "controllable.hpp"
#include <vector>

class Control;

class IController{
  public:
    virtual ~IController() = default;

    IController(
        Controllable& element
    ) : element(element) {};
   
    virtual void bindKeys(std::vector<Control> controls) = 0;

  protected:
    Controllable& element;
};
