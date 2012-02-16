/*
 * vertex.h
 *
 * Vertices for CS283 HW1
 *
 * Created on: February 15, 2012
 *     Author: bmwang, yglee
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include <algebra3.h>

class Vertex
{
  public:
    Vertex() {};
    Vertex(vec3 pos, vec3 rest, vec3 force, vec3 accel) : pos(pos), rest(rest),
      force(force), accel(accel) {};

    vec3 pos;
    vec3 rest;
    vec3 force;
    vec3 accel;
};

#endif 
