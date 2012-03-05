#include "edge.h"

Edge::Edge(Vertex *v1, Vertex *v2)
{
  vertices.push_back(v1);
  vertices.push_back(v2);
}

