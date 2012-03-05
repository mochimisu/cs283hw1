#ifndef ENGINE_H
#define ENGINE_H

#include "vertex.h"
#include "triangle.h"
#include "edge.h"
#include <vector>

class Engine {

  public:
    Engine();
    ~Engine();

    void step(float stepSize);
    void nodeForce(Triangle* t, float lame, float mu, float phi, float psi);
    void updatePos(float stepSize);
    void updateForces(float lame, float mu, float phi, float psi);
    double vertexCollisionDetect(vec3 start, vec3 end, Triangle *tri, Vertex *ver);
    double edgeCollisionDetect(Edge *edge, Triangle *tri, Vertex *ver);

    std::vector<Vertex *> *vertices; 
    std::vector<Triangle *> *triangles;
    std::vector<Edge *> *edges;

  private:
};

#endif //ENGINE_H
