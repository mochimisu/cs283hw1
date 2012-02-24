#include "triangle.h"

Triangle::Triangle(Vertex *v1, Vertex *v2, Vertex *v3) : vtx1(v1), vtx2(v2), vtx3(v3) 
{
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
}

