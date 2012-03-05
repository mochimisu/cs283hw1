#include "squareMesh.h"
#include <math.h>
#include <cstdlib>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

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
      z = (float)(rand() % 10)/150.0;
      bool pinned = false;// (j==iterY-1) && ((i==iterX-1) || (i==0));
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


void generateMesh2(vector<Triangle*>& triangles, vector<Vertex*>& vertices, \
    vector<Edge *>& edges, \
    float width, float height, float res) 
{

  int iterX = ceil(width/res);
  int iterY = ceil(height/res);
  float nodeMass = 1.0/(iterX*iterY);

  Vertex* vertArray[iterX][iterY];

  float z = 0.0;
  for (int i=0; i<iterX; i++) {
    for(int j=0; j<iterY; j++) {
	  //z = (float)(rand() % 10)/100.0 - 0.5;
      bool pinned = (j==iterY-1) && ((i==iterX-1) || (i==0) || i==ceil(iterX/2));
      Vertex* v1 = new Vertex(vec3(i*res, z, j*res-height/2.0),vec2(i*res, j*res));
      v1->mass = nodeMass;
      v1->pinned = pinned;
      vertices.push_back(v1);
      vertArray[i][j] = v1;
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



void generateMesh3(vector<Triangle*>& triangles, vector<Vertex*>& vertices, \
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
      z = (float)(rand() % 10)/150.0;
      bool pinned = (j==iterY-1) && (i==0);
      Vertex* v1 = new Vertex(vec3(i*res, j*res, z),vec2(i*res, j*res));
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




void loadOBJ(vector<Triangle*>& triangles, vector<Vertex*>& vertices, \
    vector<Edge *>& edges, \
    string fname)
{
  vector<vec3> verts;
  vector<vec3> normals;
  vector<vec3> rawFaces;
  vector<vec3> rawFacesNormals;

  ifstream inpfile(fname.c_str());
  if (!inpfile.is_open()) {
    cout << "Unable to open file" << endl;
  } else {
    string line;
    while(!getline(inpfile,line).eof()) {
    //while(inpfile.good()) {
      vector<string> splitline;
      string buf;

     
      stringstream ss(line);
      while (ss >> buf) {
        splitline.push_back(buf);
      }

      //Ignore blank lines
      if(splitline.size() == 0) {
        continue;
      }
      //Vertex
      if (splitline[0][0] == 'v') {
        //Vertex normal
        if (splitline[0].length() > 1 && splitline[0][1] == 'n'){
          normals.push_back(vec3(atof(splitline[1].c_str()),
                atof(splitline[2].c_str()),atof(splitline[3].c_str())));
        } else {
          verts.push_back(vec3(atof(splitline[1].c_str()),
                atof(splitline[2].c_str()),atof(splitline[3].c_str())));
        }
      } 
      //Face
      else if (splitline[0][0] == 'f') {
        int v1, v2, v3;
        int n1, n2, n3;
        //find "type"
        int numSlash = 0;
        for (int i=0; i<splitline[1].length(); i++) {
          if(splitline[1][i] == '/')
            numSlash++;
        }
        //cout << numSlash << endl;
        if (numSlash == 0) {
          sscanf(line.c_str(), "f %d %d %d", &v1, &v2, &v3);
          rawFaces.push_back(vec3(v1-1,v2-1,v3-1));
        } else if (numSlash == 1) {
          sscanf(line.c_str(), "f %d/%*d %d/%*d %d/%*d", &v1, &v2, &v3);
          rawFaces.push_back(vec3(v1-1,v2-1,v3-1));
        } else if (numSlash == 2) {
          sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
          rawFaces.push_back(vec3(v1-1,v2-1,v3-1));
          rawFacesNormals.push_back(vec3(n1-1,n2-1,n3-1));
        } else {
          cout << "Too many slashses in f" << endl;
        }

      }
    }
    inpfile.close();

    vector<Vertex *> newVertices;
    vector<Triangle *> newTriangles;

    for(vector<vec3>::iterator v = verts.begin(); v != verts.end(); ++ v) {
      vec3 worldPos = *v;
      //do transformations here for lack of better place to do them
      worldPos = 0.2 * worldPos;
      worldPos -= vec3(0,0.2,0);

      Vertex * newVert = new Vertex(worldPos, vec2(worldPos[0], worldPos[1]));


      newVert->pinned = true;
      newVert->mass = 1;
      newVertices.push_back(newVert);
    }
    for(int i=0; i<rawFaces.size(); i++) {
      vec3 rawFace = rawFaces[i];
      Triangle * newTri = new Triangle(newVertices[floor(rawFace[0])], 
          newVertices[rawFace[1]], newVertices[rawFace[2]]);
      newTri->simulate = false;
      if(i < rawFacesNormals.size()) {
        vec3 rawFacesNormal = rawFacesNormals[i];
        newTri->vertices[0]->norm = normals[rawFacesNormal[0]];
        newTri->vertices[1]->norm = normals[rawFacesNormal[1]];
        newTri->vertices[2]->norm = normals[rawFacesNormal[2]];
      }
      newTriangles.push_back(newTri);
    }
    vertices.resize(vertices.size() + newVertices.size());
    copy(newVertices.begin(), newVertices.end(), vertices.rbegin());

    triangles.resize(triangles.size() + newTriangles.size());
    copy(newTriangles.begin(), newTriangles.end(), triangles.rbegin());
  }

}
