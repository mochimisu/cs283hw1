#ifndef ENGINE_H
#define ENGINE_H

#include "vertex.h"

class Engine {

public:
	Engine();
	~Engine();
	
	void step(float stepSize);
	mat4 nodeForce(Triangle* t, float lame, float mu);
	void updatePos();

	std::vector<Vertex *> vertices; 
	std::vector<Triangle *> triangles;

private:
};

#endif //ENGINE_H
