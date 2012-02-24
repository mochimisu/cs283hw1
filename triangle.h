#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vertex.h"
class Triangle {

public:
  Triangle(Vertex *v1, Vertex *v2, Vertex *v3);

  vector<Vertex *> vertices;

private:
};

#endif
