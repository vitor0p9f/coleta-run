#pragma once

#include "controllable.hpp"
#include <functional>
#include <vector>

class Control;

class IController{
  public:
    virtual ~IController() = default;

    IController(){}

    void addElement(const Controllable& element){
      elements.emplace_back(element);
    }
   
  protected:
    std::vector<std::reference_wrapper<const Controllable>> elements;
};
