/*
 * triangle.h
 *
 * Triangles for CS283 HW1
 *
 * Created on: February 15, 2012
 *     Author: yglee
 */


#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vertex.h"
#include <vector>
#include <algebra3.h>

class Triangle {

public:
  Triangle(Vertex *v1, Vertex *v2, Vertex *v3);

  vector<Vertex *> vertices;
  bool simulate;

  mat3 matToBary;

private:
};

#endif
