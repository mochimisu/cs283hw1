#include "squareMesh.h"
#include <math.h>

void generateMesh(vector<Triangle*>& triangles, vector<Vertex*>& vertices, float width, float height, float res) 
{

	int iterX = ceil(width/res);
	int iterY = ceil(height/res);

	Vertex* vertArray[iterX][iterY];

	float z = -0.5;
	for (int i=0; i<iterX; i++) {
		for(int j=0; j<iterY; j++) {
			bool pinned = (i==0) && (j==0);
			Vertex* v1 = new Vertex(vec3(i*res, j*res, z),vec3(i*res, j*res, z),vec3(i*res, j*res, z));
			v1->mass = 1;
			v1->pinned = pinned;
			vertices.push_back(v1);
			vertArray[i][j] = v1;
		}
	}

	for (int i=0; i<iterX-1; i++) {
		for(int j=0; j<iterY-1; j++) {
			Triangle *t1 = new Triangle(vertArray[i][j], vertArray[i+1][j], vertArray[i+1][j+1]);
			Triangle *t2 = new Triangle(vertArray[i][j], vertArray[i][j+1], vertArray[i+1][j+1]);

			triangles.push_back(t1);
			triangles.push_back(t2);
		}
	}
}
