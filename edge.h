#ifndef EDGE_H_
#define EDGE_H_

#include "edge.h"
#include "vertex.h"
#include <vector>

class Edge {

public: 
    Edge(Vertex *v1, Vertex *v2);
    vector<Vertex *> vertices;

private:

};
#endif
