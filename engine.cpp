#include "engine.h"

float delta(int i, int j) {
	return i==j ? 1 : 0;
}

Engine::Engine()
{
}

Engine::~Engine()
{
	vertices.clear();
}

mat4 Engine::nodeForce(Triangle* t, float lame, float mu)
{
	//Green Strain
	float epsilon = 0.1;
	mat4 strain = mat4(1.0);

	Vertex *v1 = t->vertices[0];
	Vertex *v2 = t->vertices[1];
	Vertex *v3 = t->vertices[2];
	
	materialToBary = mat3(
	vec3(v1->mPos[0], v2->mPos[0], v3->mPos[0]),
	vec3(v1->mPos[1], v2->mPos[1], v3->mPos[1]),
	vec3(1, 1, 1 )).inverse();

	materialToWorld = mat3(
	vec3(v1->wPos[0], v2->wPos[0], v3->wPos[0]),
	vec3(v1->wPos[1], v2->wPos[1], v3->wPos[1]),
	vec3(v1->wPos[2], v2->wPos[2], v3->wPos[2]))
	* materialToBary;

	for (int i = 0; i < 3; i++) {
		for (int j =0; j< 3; j++) {
			vec3 xu = materialToWorld * vec3(t->vertices[i]->mPos[j][0], t->vertices[i]->mPos[j][1] , 1);
			vec3 xuEpsilonI = materialToWorld * vec3(t->vertices[i]->mPos[j][0]+epsilon, t->vertices[i]->mPos[j][1] , 1);
			vec3 xuEpsilonJ = materialToWorld * vec3(t->vertices[i]->mPos[j][0], t->vertices[i]->mPos[j][1]+epsilon , 1);
			vec3 xui = (xuEpsilonI - xu) / epsilon;
			vec3 xuj = (xuEpsilonJ - xu) / epsilon;

			strain[i][j] = xui*xuj - delta(i,j);
		}
	}


	//Stress
	mat4 stress = mat3(
				  vec3(0.0),
				  vec3(0.0),
				  vec3(0.0));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			
			float sum = 0.0;
			for (int k = 0; k< 3; k++) {
				sum += lame * strain[k][k] * delta(k,k); 
			}

			stress[i][j] = sum + 2*mu*strain[i][j];
		}
	}

	//Kinetic Forces
	mat3 forces = mat3(0.0);
	float vol = 0.5 * abs((v1->wPos - v2->wPos) ^ (v3->wPos - v2->wPos));
	
	
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			forces[i][j] = -0.5 * vol;	
			for(int k=0; k<3; k++) {
				
			}
		}
	}

}




