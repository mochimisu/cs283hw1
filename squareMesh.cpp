#include "squareMesh.h"
#include <math.h>
#include <cstdlib>

void generateMesh(vector<Triangle*>& triangles, vector<Vertex*>& vertices, \
                  vector<Edge *>& edges, \
                  float width, float height, float res) 
{

  int iterX = ceil(width/res);
  int iterY = ceil(height/res);
  float nodeMass = 0.5;//10000.0/(iterX*iterY);
  cout << "Mass per node: " << nodeMass << endl;

  Vertex* vertArray[iterX][iterY];

  float z = -0;
  for (int i=0; i<iterX; i++) {
    for(int j=0; j<iterY; j++) {
      z = (float)(rand() % 10)/200.0;
      bool pinned = (j==iterY-1) && ((i==iterX-1) || (i==0));
      Vertex* v1 = new Vertex(vec3(i*res-width/2.0, j*res, z),vec2(i*res, j*res));
      v1->mass = nodeMass;
      v1->pinned = pinned;
      vertices.push_back(v1);
      vertArray[i][j] = v1;
      if(pinned)
        v1->marked = 10000;
    }
  }

  //triangles
  for (int i=0; i<iterX-1; i++) {
    for(int j=0; j<iterY-1; j++) {
      Triangle *t1 = new Triangle(vertArray[i][j], 
          vertArray[i+1][j], vertArray[i+1][j+1]);
      Triangle *t2 = new Triangle(vertArray[i][j], 
           vertArray[i+1][j+1],vertArray[i][j+1]);

      triangles.push_back(t1);
      triangles.push_back(t2);
    }
  }

 //edges
 int counter = 0;
 for (vector<Triangle *>::iterator it = triangles.begin(); 
     it != triangles.end(); ++it) {
     
     Triangle *tri = *it;
     vector<Vertex *> verts = tri->vertices;

     //get the three edges of the square
     if (counter %2 != 0) {
      Edge *e1 = new Edge(verts[0], verts[1]);
      edges.push_back(e1);
      
      Edge *e2 = new Edge(verts[0], verts[2]);
      edges.push_back(e2);

     } else {
      Edge *e3 = new Edge(verts[0], verts[1]);
      edges.push_back(e3);
     }
     counter ++;
 }

 //edges: handle boundary cases
 for (int i = 0; i< iterX-1; i++) {
    Edge *e = new Edge(vertArray[i][iterY-1], vertArray[i+1][iterY-1]);
    edges.push_back(e);
 }

 for (int j = 0; j< iterY-1; j++) {
    Edge *e = new Edge(vertArray[iterX-1][j], vertArray[iterX-1][j+1]);
    edges.push_back(e); 
 }

}


void generateMesh2(vector<Triangle*>& triangles, vector<Vertex*>& vertices, 
    float width, float height, float res) 
{

  int iterX = ceil(width/res);
  int iterY = ceil(height/res);
  float nodeMass = 1.0/(iterX*iterY);

  Vertex* vertArray[iterX][iterY];

  float z = -0;
  for (int i=0; i<iterX; i++) {
    for(int j=0; j<iterY; j++) {
	  z = (float)(rand() % 10)/100.0 - 0.5;
      bool pinned = true;//(j==iterY-1) && ((i==iterX-1) || (i==0) || i==ceil(iterX/2));
      Vertex* v1 = new Vertex(vec3(i*res, z, j*res-height/2.0),vec2(i*res, j*res));
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
