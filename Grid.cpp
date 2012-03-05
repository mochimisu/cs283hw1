#include "grid.h"

TriangleGrid::TriangleGrid(vec3 dim, vector<Vertex *> *verts, vector<Triangle *> *tris)
{ 
  this->dim = dim;
  this->triangles = tris;
  this->vertices = verts;
  this->minCoord = vec3(10000,10000,10000);
  this->maxCoord = vec3(-10000,-10000,-10000);
   
  //hardcoded for now
  this->dim = vec3(10,10,10);
}

void TriangleGrid::update()
{
  
  for (vector<Vertex*>::iterator vertexIter = vertices->begin(); vertexIter != vertices->end(); 
      ++vertexIter) {
    Vertex * curVertex = *vertexIter;
    minCoord[0] = min(minCoord[0],curVertex->wPos[0]);
    minCoord[1] = min(minCoord[1],curVertex->wPos[1]);
    minCoord[2] = min(minCoord[2],curVertex->wPos[2]);

    maxCoord[0] = max(maxCoord[0],curVertex->wPos[0]);
    maxCoord[1] = max(maxCoord[1],curVertex->wPos[1]);
    maxCoord[2] = max(maxCoord[2],curVertex->wPos[2]);
  }


  bucketSize = vec3((maxCoord[0]-minCoord[0])/dim[0],
      (maxCoord[1]-minCoord[1])/dim[1],
      (maxCoord[2]-minCoord[2])/dim[2]);

  for(int i=0; i<dim[0]; i++) {
    for(int j=0; j<dim[1]; j++) {
      for(int k=0; k<dim[2]; k++) {
        bucketed[i][j][k].clear();
      }
    }
  }


  for(std::vector<Triangle *>::iterator iter = triangles->begin();
      iter != triangles->end(); ++iter) {
    Triangle * curTriangle = *iter;
    this->findTriangles(curTriangle->vertices[0]->wPos)->push_back(curTriangle);
    this->findTriangles(curTriangle->vertices[1]->wPos)->push_back(curTriangle);
    this->findTriangles(curTriangle->vertices[2]->wPos)->push_back(curTriangle);
  }

}

vector<Triangle *> * TriangleGrid::findTriangles(vec3 loc)
{
  int b0 = floor((loc[0]-minCoord[0])/bucketSize[0]);
  int b1 = floor((loc[1]-minCoord[1])/bucketSize[1]);
  int b2 = floor((loc[2]-minCoord[2])/bucketSize[2]);
  if(b0 < 0 || b1 < 0 || b2 < 0 || b0 > dim[0]+1 || b1 > dim[1]+1 || b2 > dim[2]+1) {
    //cout << "oh no: " << b0 << ", " << b1 << ", " << b2 << endl;
    return &empty;
    //return triangles;
  }
  //cout << "b0: " << b0 << ", b1: " << b1 << ", b2: " << b2 << endl;
  return &bucketed[b0][b1][b2];
  //cout << "success" << endl;

}
