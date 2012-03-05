#ifndef RENDERER_H
#define RENDERER_H

#include "triangle.h"
#include "engine.h"
#include "edge.h"
#include <GL/glut.h>   


class Renderer {

public:
	Renderer();
	~Renderer();

  void init(int argc, char** argv);
  void draw();
  void stepEngine();
  void mainLoop();
  void drawMarkedPoints();

  std::vector<Triangle *> triangles;
  std::vector<Vertex *> vertices;
  std::vector<Edge *> edges;

  Engine engine;

  int width, height;
  vec2 mousePos;
  mat4 orientation;
};

void setActiveRenderer(Renderer * newRenderer);

#endif
