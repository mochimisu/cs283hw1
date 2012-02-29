#include "squareMesh.h"
#include <math.h>
#include <cstdlib>

void generateMesh(vector<Triangle*>& triangles, vector<Vertex*>& vertices, 
    float width, float height, float res) 
{

  int iterX = ceil(width/res);
  int iterY = ceil(height/res);
  float nodeMass = 1.0/(iterX*iterY);

  Vertex* vertArray[iterX][iterY];

  float z = -0;
  for (int i=0; i<iterX; i++) {
    for(int j=0; j<iterY; j++) {
	  z = (float)(rand() % 10)/100.0;
      bool pinned = (i==0) && (j==0);//(j==iterY-1) && ((i==iterX-1) || (i==0) || i==ceil(iterX/2));
      Vertex* v1 = new Vertex(vec3(i*res, j*res, z),vec2(i*res, j*res));
      v1->mass = nodeMass;
      v1->pinned = pinned;
      vertices.push_back(v1);
      vertArray[i][j] = v1;
    }
  }

  for (int i=0; i<iterX-1; i++) {
    for(int j=0; j<iterY-1; j++) {
      Triangle *t1 = new Triangle(vertArray[i][j], 
          vertArray[i+1][j], vertArray[i+1][j+1]);
      Triangle *t2 = new Triangle(vertArray[i][j], 
          vertArray[i][j+1], vertArray[i+1][j+1]);

      triangles.push_back(t1);
      triangles.push_back(t2);
    }
  }
}
