#ifndef SQUAREMESH_H_
#define SQUAREMESH_H_

#include "vertex.h"
#include "triangle.h"
#include <vector>

void generateMesh(vector<Triangle*>& triangles, vector<Vertex*>& vertices, float width, float height, float res);

#endif
