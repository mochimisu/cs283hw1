#ifndef RENDERER_H
#define RENDERER_H

#include "triangle.h"

class Renderer {

public:
	Renderer();
	~Renderer();

	void draw();

	std::vector<Triangle *> triangles;

};

#endif
