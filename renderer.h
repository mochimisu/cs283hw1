#ifndef RENDERER_H
#define RENDERER_H

#include "triangle.h"
#include "engine.h"

class Renderer {

public:
	Renderer();
	~Renderer();

  void init(int argc, char** argv);
	void draw();
  void mainLoop();

	std::vector<Triangle *> triangles;
  Engine engine;

  int width, height;
};

void setActiveRenderer(Renderer * newRenderer);

#endif
