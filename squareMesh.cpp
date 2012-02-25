#include "squareMesh.h"

void generateMesh(vector<Triangle*>& triangles, vector<Vertex*>& vertices, float width, float height, float res) 
{
	float z = -0.5;
	for (float i =0.0; i< height; i+=res) {
		for (float j=0.0; j< width; j+=res) {
			Vertex* v1 = new Vertex(vec3(i,j,z),vec3(i,j,z),vec3(i,j,z));
			vertices.push_back(v1);
			
			Vertex* v2 = new Vertex(vec3(i,j+res,z),vec3(i,j+res,z),vec3(i,j+res,z));
			vertices.push_back(v2);
			
			Vertex* v3 = new Vertex(vec3(i+res,j,z),vec3(i+res,j,z),vec3(i+res,j,z));
			vertices.push_back(v3);
	
			Vertex* v4 = new Vertex(vec3(i+res,j+res,z),vec3(i+res,j+res,z),vec3(i+res,j+res,z));
			vertices.push_back(v4);
		
			Triangle* t1 = new Triangle(v1, v2, v4);
			Triangle* t2 = new Triangle(v1, v3, v4);

			triangles.push_back(t1);
			triangles.push_back(t2);
		}
	}
}
