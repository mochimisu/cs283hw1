#ifndef RENDERER_H
#define RENDERER_H

class Renderer {

public:
	Renderer();
	~Renderer();

	void draw();

	std::vector<Triangle *> triangles;

};

#endif
