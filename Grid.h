#ifndef _GRID_H_
#define _GRID_H_

#include <algebra3.h>
#include <vector>
#include "triangle.h"

class TriangleGrid
{
  public:
    TriangleGrid(vec3 dim, vector<Vertex *> *verts, vector<Triangle *> *tris);

    void update();
    vector<Triangle *> * findTriangles(vec3 loc);


  private:
    vec3 dim;
    vector<Triangle*> * triangles;
    vector<Vertex*> * vertices;

    //hardcoded for now
    vector<Triangle *> bucketed[11][11][11];

    //Axis aligned
    vec3 minCoord;
    vec3 maxCoord;
    vec3 bucketSize;

    //empty
    vector<Triangle*> empty;
};

#endif
