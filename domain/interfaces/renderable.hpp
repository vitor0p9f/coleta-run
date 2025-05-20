#ifndef RENDERABLE_H
  #define RENDERABLE_H
  
  #include "renderer.hpp"

  class Renderable {
    public:
      virtual ~Renderable() = default;
      virtual void render(RendererInterface& renderer) = 0;
  };
#endif

