#include "engine.h"

float delta(int i, int j) {
	return i==j ? 1 : 0;
}

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::step(float stepSize)
{
	//TODO: move lame and mu constants somewhere else
	updateForces(20.0, 20.0);
	updatePos(stepSize);
}

void Engine::nodeForce(Triangle* t, float lame, float mu)
{
	//Green Strain
	float epsilon = 0.1;
	mat4 strain = mat4(1.0);

	Vertex *v1 = t->vertices[0];
	Vertex *v2 = t->vertices[1];
	Vertex *v3 = t->vertices[2];
	
	cout << v1->mPos << endl;
	cout << v2->mPos << endl;
	cout << v3->mPos << endl;

	mat3 materialToBary = mat3(
					vec3(v1->mPos[0], v2->mPos[0], v3->mPos[0]),
					vec3(v1->mPos[1], v2->mPos[1], v3->mPos[1]),
					vec3(1, 1, 1 )).inverse();

	mat3 materialToWorld = mat3(
					vec3(v1->wPos[0], v2->wPos[0], v3->wPos[0]),
					vec3(v1->wPos[1], v2->wPos[1], v3->wPos[1]),
					vec3(v1->wPos[2], v2->wPos[2], v3->wPos[2]))
	* materialToBary;

	for (int i = 0; i < 3; i++) {
		for (int j =0; j< 3; j++) {
			vec3 xu = materialToWorld * vec3(t->vertices[i]->mPos[0], t->vertices[i]->mPos[1] , 1);
			vec3 xuEpsilonI = materialToWorld * vec3(t->vertices[i]->mPos[0]+epsilon, t->vertices[i]->mPos[1] , 1);
			vec3 xuEpsilonJ = materialToWorld * vec3(t->vertices[i]->mPos[0], t->vertices[i]->mPos[1]+epsilon , 1);
			vec3 xui = (xuEpsilonI - xu) / epsilon;
			vec3 xuj = (xuEpsilonJ - xu) / epsilon;

			strain[i][j] = xui*xuj - delta(i,j);
		}
	}


	//Stress
	mat3 stress = mat3(
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
	float vol = 0.5 * ((v1->wPos - v2->wPos) ^ (v3->wPos - v2->wPos)).length();
	
	
	for(int i=0; i<3; i++) {
		vec3 pointSum = vec3(0.0);
		for(int j=0; j<3; j++) {
			float stressSum = 0.0;
			for(int k=0; k<3; k++) {
				for (int l=0; l<3; l++) {
					stressSum += materialToBary[j][l]*materialToBary[i][k]*stress[k][l];
				}
			}
			pointSum += t->vertices[j]->wPos * stressSum;
		}
		vec3 curForce = -0.5 * vol * pointSum;
		t->vertices[i]->force = curForce;
		//gravity
		t->vertices[i]->force += vec3(0,-9.8,0);
	}
}

void Engine::updatePos(float timeStep) 
{
	vector<Vertex*>::iterator vertexIter;
	for (vertexIter = vertices->begin(); vertexIter != vertices->end(); ++vertexIter) {
		
		Vertex * curVertex = *vertexIter;
		vec3 curForce = curVertex->force;
		vec3 curAccel = curForce/curVertex->mass;
		vec3 curVelocity = curVertex->vel + curAccel * timeStep;
		vec3 curPos = curVertex->wPos + curVelocity * timeStep;
		if (!curVertex->pinned) {
				curVertex->accel = curAccel;
				curVertex->vel = curVelocity;
				curVertex->wPos = curPos;
		}
		cout << "New position: " << curVertex->wPos[0] << "," << curVertex->wPos[1] << "," <<curVertex->wPos[2] << endl;
	}
}

void Engine::updateForces(float lame, float mu)
{
  vector<Triangle*>::iterator triangleIter;
  for (triangleIter = triangles->begin(); triangleIter != triangles->end(); ++triangleIter) {
    nodeForce(*triangleIter, lame, mu);
  }
}
