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
  updateForces(1, 10, 1, 10);
  updatePos(stepSize);
}

void Engine::nodeForce(Triangle* t, float lame, float mu, float phi, float psi)
{
  //Green Strain
  Vertex *v1 = t->vertices[0];
  Vertex *v2 = t->vertices[1];
  Vertex *v3 = t->vertices[2];

  //Beta
  mat3 materialToBary = mat3(
      vec3(v1->mPos[0], v2->mPos[0], v3->mPos[0]),
      vec3(v1->mPos[1], v2->mPos[1], v3->mPos[1]),
      vec3(1, 1, 1 )).inverse();

  mat3 baryToWorld = mat3(
      vec3(v1->wPos[0], v2->wPos[0], v3->wPos[0]),
      vec3(v1->wPos[1], v2->wPos[1], v3->wPos[1]),
      vec3(v1->wPos[2], v2->wPos[2], v3->wPos[2]));

  mat3 materialToWorld = baryToWorld * materialToBary;

  mat3 strain = (materialToWorld.transpose()
      *materialToWorld - identity2D());

  //Stress
  //mat2 stress = mat2(0);
  mat3 stress = mat3(0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      float sum = 0.0;
      for (int k = 0; k< 3; k++) {
        sum += strain[k][k]; 
      }

      stress[i][j] = lame*sum*delta(i,j) + 2*mu*strain[i][j];
    }
  }

  //Green Rate
  mat3 baryToVelocityWorld = mat3(
      vec3(v1->vel[0], v2->vel[0], v3->vel[0]),
      vec3(v1->vel[1], v2->vel[1], v3->vel[1]),
      vec3(v1->vel[2], v2->vel[2], v3->vel[2]));

  mat3 materialToVelocityWorld = baryToVelocityWorld * materialToBary;

  mat3 rate = 0.5*(materialToWorld.transpose() * materialToVelocityWorld
      + materialToVelocityWorld.transpose() * materialToWorld);

  //Green Rate Stress
  mat3 rateStress = mat3(0);
  for (int i=0; i<3; i++) {
    for (int j=0; j<3; j++) {
      //change these to other consts later
      float sum = 0.0;
      for (int k=0; k<3; k++) {
        sum += strain[k][k];
      }
      rateStress[i][j] = phi*sum*delta(i,j) + 2*psi*rate[i][j];
    }
  }

  //Kinetic Forces
  float vol = 0.5 * ((v1->mPos - v2->mPos) ^ (v3->mPos - v2->mPos)).length();

  for(int i=0; i<3; i++) {
    vec3 pointSum = vec3(0.0);
    for(int j=0; j<3; j++) {
      float stressSum = 0.0;
      for(int k=0; k<3; k++) {
        for (int l=0; l<3; l++) {
          stressSum += materialToBary[j][l]*materialToBary[i][k]
            *(stress[k][l]+rateStress[k][l]);
        }
      }
      pointSum += t->vertices[j]->wPos * stressSum;
    }
    vec3 curForce = -0.5 * vol * pointSum;
    t->vertices[i]->force += curForce;
  }
}

void Engine::updatePos(float timeStep) 
{
  vector<Vertex*>::iterator vertexIter;
  for (vertexIter = vertices->begin(); vertexIter != vertices->end(); 
      ++vertexIter) {

    Vertex * curVertex = *vertexIter;
	vec3 curForce = curVertex->force;
    vec3 curAccel = curForce/curVertex->mass;
    vec3 curVelocity = curVertex->vel + curAccel * timeStep;
    vec3 prevPos = curVertex->wPos;
	vec3 curPos = curVertex->wPos + curVelocity * timeStep;


	vector<Triangle *> intersectingTris;

    if (!curVertex->pinned) {
      curVertex->accel = curAccel;
      curVertex->vel = curVelocity;

	  vertexCollisionDetect(prevPos, curPos,intersectingTris);
      if (intersectingTris.empty()) {
	    curVertex->wPos = curPos;
	  } else {
	    cout << "asdf";
	    curVertex->vel = 0;
	  }
    }
    //cout << "New position: " << curVertex->wPos[0] << "," 
    //<< curVertex->wPos[1] << "," <<curVertex->wPos[2] << endl;
  }
}

void Engine::updateForces(float lame, float mu, float phi, float psi)
{
  for (vector<Vertex*>::iterator vertIter = vertices->begin();
      vertIter != vertices->end(); ++vertIter) {
    Vertex * curVert = *vertIter;
    //Reset
    curVert->force = vec3(0);
    //Gravity
    curVert->force += vec3(0,-9.8*curVert->mass,0);
  }

  vector<Triangle*>::iterator triangleIter;
  for (triangleIter = triangles->begin(); triangleIter != triangles->end(); 
      ++triangleIter) {
    nodeForce(*triangleIter, lame, mu, phi, psi);
  }
}

//returns t value of triangle-ray collision
void Engine::vertexCollisionDetect(vec3 start, vec3 end, vector<Triangle*>intersectingTri){

	for(std::vector<Triangle *>::iterator iter = triangles->begin();
		iter != triangles->end(); ++iter) {
		Triangle * t = *iter;
		
		vec3 u, v, n; //triangle vectors
		float r,a,b;
		vec3 dir, w0, w; //ray vectors
		vec3 intersectPt;
		
		//get triangle vec u
		u = t->vertices[1]->wPos - t->vertices[0]->wPos;
		
		//get vec v
		v = t->vertices[2]->wPos - t->vertices[0]->wPos;

		n = u ^ v;
		if (n == vec3(0)) continue;

		//get direction of the "ray"
		dir = end - start;
		
		w0 = start - t->vertices[0]->wPos;	

		a = -1*n*w0;
		b = n*dir;
		
		//ray goes away from triangle, no intersection
		r = a / b;
		if (r < 0.0) continue; 
		
		//get intersect point of ray with triangle plane
		intersectPt = start + r * dir;
		cout << r << endl;
	    
		float uu, uv, vv, wu, wv, D;
		uu = u*u;
		uv = u*v;
		vv = v*v;
	
		w = intersectPt - t->vertices[0]->wPos; 

		wu = w*u;
		wv = w*v;

		D = uv * uv - uu * vv;

		float s, t_0; 
		s = (uv * wv - vv* wu) / D;
		t_0 = (uv * wu - uu * wv) / D;
		
		if (s < 0.0 || s > 1.0)
			continue;
		if (t_0 < 0.0 || (s+t_0) > 1.0)
			continue;
		intersectingTri.push_back(t);
	}
}
