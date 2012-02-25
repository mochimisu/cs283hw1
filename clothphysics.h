/*
 * clothphyiscs.h
 *
 * Cloth physics for CS283 HW1
 *
 * Created on: February 15, 2012
 *     Author: bmwang, yglee
 */


#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vertex.h"
class ClothEngine {

public:
  Triangle(Vertex *v1, Vertex *v2, Vertex *v3);
  
  Vertex *vtx1;
  Vertex *vtx2;
  Vertex *vtx3;  

private:
};

#endif
