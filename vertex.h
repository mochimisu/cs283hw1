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
    Vertex(vec3 pos, vec3 mPos, vec3 rest) : wPos(pos), mPos(mPos), rest(rest),
      force(force), accel(accel) {
	  vel = vec3(0.0);
	  pinned = false;
	  };

    vec3 wPos;
	vec2 mPos;
    vec3 rest;
    vec3 force;
    vec3 accel;
	vec3 vel;

	float mass;
	bool pinned;
};

#endif 
