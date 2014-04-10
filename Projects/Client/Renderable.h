#pragma once
/* Interface for classes that can be rendered to the display. */
class Renderable
{
public:
  virtual ~Renderable() = default;
  /* Render the Renderable to the display. */
  void render() { do_render(); }
private:
  virtual void do_render() = 0;
};

