#ifndef SQUAREMESH_H_
#define SQUAREMESH_H_

#include "vertex.h"
#include "triangle.h"
#include "edge.h"
#include <vector>

void generateMesh(vector<Triangle*>& triangles, vector<Vertex*>& vertices, \
                  vector<Edge *>& edges, \
                  float width, float height, float res);
void generateMesh2(vector<Triangle*>& triangles, vector<Vertex*>& vertices, \
                  vector<Edge *>& edges, \
                  float width, float height, float res);

void generateMesh3(vector<Triangle*>& triangles, vector<Vertex*>& vertices, \
                  vector<Edge *>& edges, \
                  float width, float height, float res);

void loadOBJ(vector<Triangle*>& triangles, vector<Vertex*>& vertices, \
                  vector<Edge *>& edges, \
                  string fname);
#endif
