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
    Vertex(vec3 pos, vec2 mPos) : wPos(pos), mPos(mPos) {
      force = vec3(0);
      accel = vec3(0);
      vel = vec3(0.0);
      pinned = false;
      marked = 0;
    };

    vec3 wPos;
    vec2 mPos;
    vec3 force;
    vec3 accel;
    vec3 vel;
    vec3 norm;
    int normCount;

    float mass;
    bool pinned;
    int marked;
};

#endif 
