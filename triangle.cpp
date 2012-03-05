/*
 * triangle.cpp
 *
 * Triangles for CS283 HW1
 *
 * Created on: February 15, 2012
 *     Author: yglee
 */


#include "triangle.h"

Triangle::Triangle(Vertex *v1, Vertex *v2, Vertex *v3) 
{
  vertices.push_back(v1);
  vertices.push_back(v2);
  vertices.push_back(v3);
  simulate = true;
}
/*
BoundingBox Triangle::boundingBox() {
  BoundingBox bb = BoundingBox();
  bb.addPoint(vertices[0]->wPos);
  bb.addPoint(vertices[1]->wPos);
  bb.addPoint(vertices[2]->wPos);
  return bb;
}
*/
