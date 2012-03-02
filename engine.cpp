#include "engine.h"
#define EPSILON 0.000000001

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
  vec3 curForce, curAccel, curVelocity, prevPos, curPos;

//test code for intersection
#if 1
  curPos = vec3(0,6,5); prevPos = vec3(0,6,-5);
  Vertex *v1, *v2, *v3;
  v1 = new Vertex(vec3(0,0,0), vec3(0,0,0));
  v2 = new Vertex(vec3(5,0,0), vec3(5,0,0));
  v3 = new Vertex(vec3(0,5,0), vec3(0,5,0));
  Triangle *testTri = new Triangle(v1,v2,v3);
  cout<<"intersect? "<<vertexCollisionDetect(prevPos,curPos, testTri) <<endl;
#endif
  for (vertexIter = vertices->begin(); vertexIter != vertices->end(); 
      ++vertexIter) {
  	Vertex *curVertex = *vertexIter;
	curForce = curVertex->force;
    curAccel = curForce/curVertex->mass;
    curVelocity = curVertex->vel + curAccel * timeStep;
    prevPos = curVertex->wPos;
	curPos = curVertex->wPos + curVelocity * timeStep;

	vector<Triangle *> intersectingTris;

    if (!curVertex->pinned) {
      curVertex->accel = curAccel;
      curVertex->vel = curVelocity;
	  
	  for(std::vector<Triangle *>::iterator iter = triangles->begin();
		iter != triangles->end(); ++iter) {	
		Triangle * tri = *iter;
		
	    //if (!vertexCollisionDetect(prevPos, curPos, tri)){
		    curVertex->wPos = curPos;
	  	/*
		} else {
	  		cout<<"intersecting tri"<<endl;
	    	curVertex->vel = 0;
	  	} */
		curVertex->wPos = curPos;
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

//returns true if triangle intersects with ray
bool Engine::vertexCollisionDetect(vec3 start, vec3 end, Triangle *tri){
	vec3 u, v, n;
	vec3 dir, w0, w;
	float r, a, b;

	//triangle vertices
	vec3 t0 = tri->vertices[0]->wPos;
	vec3 t1 = tri->vertices[1]->wPos;
	vec3 t2 = tri->vertices[2]->wPos;

	u = t1 - t0;
	v = t2 - t0;

	n = u ^ v;
	if (n == vec3(0)){
		cout<<1<<endl;
		return false; //degenerate tri
	}

	dir = end - start;
	dir.normalize();
	w0 = start - t0; 

	a = -1*(n*w0); 
	b =  n*dir;

	r = a/b; 

	if (r < 0.0) {//ray goes away from tri
		cout<<3<<endl;
		return false;
	}

	//intersect pt of ray and plane
	vec3 intersectPt = start + (r*dir);

	//is intesect pt inside Tri?
	float uu, uv, vv, wu, wv, D;
	uu = u*u;
	uv = u*v;
	vv = v*v;
	w = intersectPt - t0;
	wu = w*u;
	wv = w*v;
	D = (uv*uv) - (uu*vv);
	
	//get and test param coords
	float s, t;
	cout<<uv*wv<<endl;
	cout<<vv*wu<<endl;
	cout<<D<<endl;
	s = ((uv*wv) - (vv*wu)) / D;
		
	if (s < 0.0 || s > 1.0){
		cout<<4<<endl;
		cout<<"s is "<<s<<endl;
		return false;
	}
	
	t = ((uv*wu) - (uu*wv)) / D;
	if (t < 0.0 || (s+t) > 1.0){
		cout<<5<<endl;
		cout<<"t is "<<t<<endl;
		return false;
	}
	
	return true;
}
