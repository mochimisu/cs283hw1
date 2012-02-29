#ifndef ENGINE_H
#define ENGINE_H

#include "vertex.h"
#include "triangle.h"
#include <vector>

class Engine {

public:
	Engine();
	~Engine();
	
	void step(float stepSize);
	void nodeForce(Triangle* t, float lame, float mu, float phi, float psi);
	void updatePos(float stepSize);
  void updateForces(float lame, float mu, float phi, float psi);

	std::vector<Vertex *> *vertices; 
	std::vector<Triangle *> *triangles;

private:
};

#endif //ENGINE_H
