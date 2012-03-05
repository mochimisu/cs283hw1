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

void Engine::init(vector<Vertex *> * verts, vector<Triangle *> * tris, 
    vector<Edge *> * edgs) {
  triGrid = new TriangleGrid(vec3(10,10,2), verts, tris);
  vertices = verts;
  triangles = tris;
  edges = edgs;
}

void Engine::step(float stepSize)
{
  //TODO: move lame and mu constants somewhere else
  updateForces(100000, 1000, 1, 500);
  updatePos(stepSize);
}

void Engine::nodeForce(Triangle* t, float lame, float mu, float phi, float psi)
{
  if(!t->simulate) {
    return;
  }
  //Green Strain
  Vertex *v1 = t->vertices[0];
  Vertex *v2 = t->vertices[1];
  Vertex *v3 = t->vertices[2];

  //Beta
#if 0
  mat3 materialToBary = mat3(
      vec3(v1->mPos[0], v2->mPos[0], v3->mPos[0]),
      vec3(v1->mPos[1], v2->mPos[1], v3->mPos[1]),
      vec3(1, 1, 1 )).inverse();
#else
vec2 v1_mPos_norm = vec2(0);
vec2 v2_mPos_norm = (v2->mPos - v1->mPos);
vec2 v3_mPos_norm = (v3->mPos - v1->mPos);

mat3 materialToBary = mat3(
    vec3(v1_mPos_norm[0], v2_mPos_norm[0], v3_mPos_norm[0]),
    vec3(v1_mPos_norm[1], v2_mPos_norm[1], v3_mPos_norm[1]),
    vec3(1, 1, 1 )).inverse();

#endif

#if 0
  mat3 baryToWorld = mat3(
      vec3(v1->wPos[0], v2->wPos[0], v3->wPos[0]),
      vec3(v1->wPos[1], v2->wPos[1], v3->wPos[1]),
      vec3(v1->wPos[2], v2->wPos[2], v3->wPos[2]));
#else
  mat3 baryToWorld = mat3(
vec3(v1->wPos[0]-v1->wPos[0], v2->wPos[0]-v1->wPos[0], v3->wPos[0]-v1->wPos[0]),
vec3(v1->wPos[1]-v1->wPos[1], v2->wPos[1]-v1->wPos[1], v3->wPos[1]-v1->wPos[1]),
vec3(v1->wPos[2]-v1->wPos[2], v2->wPos[2]-v1->wPos[2], v3->wPos[2]-v1->wPos[2]));

#endif

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

  mat3 edges = mat3(
      v1->wPos[0]-v1->wPos[0],
      v1->wPos[1]-v1->wPos[0],
      v1->wPos[2]-v1->wPos[0]);



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
      //pointSum += t->vertices[j]->wPos - t->vertices[0]->wPos * stressSum;
      pointSum += t->vertices[j]->wPos * stressSum;
      //pointSum += edges[j] * stressSum;
    }
    vec3 curForce = -0.5 * vol * pointSum;
    t->vertices[i]->force += curForce;
  }
}

void Engine::updatePos(float timeStep) 
{
  vector<Vertex*>::iterator vertexIter;
  vec3 curForce, curAccel, curVelocity, prevPos, curPos;

/*
  vector<Edge*>::iterator iter; 
  for (iter = edges->begin();
      iter != edges->end(); ++iter) {

    Edge *e = *iter;
    bool edgeCollision = false;
    double intersectV = numeric_limits<double>::infinity();
    Triangle *cTri; //colliding triangle with edge

    Vertex *ver = new Vertex(vec3(0,0,0),vec3(0,0,0)); //dummy 

    for(std::vector<Triangle *>::iterator iter = triangles->begin();
        iter != triangles->end(); ++iter) {	
      Triangle * tri = *iter;
      double interV = edgeCollisionDetect(e, tri, ver);

      if (interV > 0){
        edgeCollision = true;
        cTri = tri;
        intersectV = min(intersectV, interV);
      }
    }
    if (edgeCollision) {
    }
  }
  */

  //test code for intersection
#if 0
  curPos = vec3(0,0,5); prevPos = vec3(0,0,-5);
  Vertex *v1, *v2, *v3;
  v1 = new Vertex(vec3(0,0,0), vec3(0,0,0));
  v2 = new Vertex(vec3(5,0,0), vec3(5,0,0));
  v3 = new Vertex(vec3(0,5,0), vec3(0,5,0));
  Triangle *testTri = new Triangle(v1,v2,v3);
  cout<<"intersect? "<<vertexCollisionDetect(prevPos,curPos, testTri) <<endl;
#endif
  triGrid->update();

  for (vertexIter = vertices->begin(); vertexIter != vertices->end(); 
      ++vertexIter) {

    Vertex *curVertex = *vertexIter;

    curForce = curVertex->force;
    curAccel = curForce/curVertex->mass + vec3(0,-9.8,0);
    curVelocity = curVertex->vel + curAccel * timeStep;
    prevPos = curVertex->wPos;
    curPos = curVertex->wPos + curVelocity * timeStep;

    bool vertCollision = false;

    double intersectT = numeric_limits<double>::infinity();
    Triangle * collisionTri;


    vector<Triangle *> * startTris = triGrid->findTriangles(prevPos);
    vector<Triangle *> * endTris = triGrid->findTriangles(curPos);
    //cout << "start tris: " << startTris->size() << endl;
    //cout << "end tris: " << endTris->size() << endl;
/*
for(std::vector<Triangle *>::iterator iter = startTris->begin();
    iter != startTris->end(); ++iter) {	      Triangle * tri = *iter;
  double interT = vertexCollisionDetect(prevPos, curPos, tri, curVertex);

  if (interT > 0){
    vertCollision = true;
    collisionTri = tri;
    intersectT = min(intersectT, interT);
  }    

} 

if(startTris != endTris) {
  //cout << "boundary crossing" << endl;

  for(std::vector<Triangle *>::iterator iter = endTris->begin();
      iter != endTris->end(); ++iter) {	
    Triangle * tri = *iter;
    double interT = vertexCollisionDetect(prevPos, curPos, tri, curVertex);
    if (interT > 0){
      vertCollision = true;
      collisionTri = tri;
      intersectT = min(intersectT, interT);
    }
  }

} else {
  //cout << "no boundary cross" << endl;
}*/

    if(vertCollision) {
      curVertex->marked = 100;
      //cout << "Collision on mpos: " << curVertex->mPos << endl;

      vec3 edge1, edge2, normal;
      edge1 = collisionTri->vertices[2]->wPos - collisionTri->vertices[0]->wPos;
      edge2 = collisionTri->vertices[1]->wPos - collisionTri->vertices[0]->wPos;
      normal = (edge1^edge2).normalize();
      /*
      float dampingMultiplier = 0.7;
      vec3 penaltyForce = dampingMultiplier * (-curForce*normal);
      vec3 collVel = curVelocity * normal;
      //cout << "collvel: " << collVel << endl;
      //curVelocity = curVelocity - collVel + penaltyForce/curVertex->mass * timeStep;
      */
      if(normal * curVelocity < 0)
        normal = -normal;
      float magVel = curVelocity * normal * 1.7;
      curVelocity = curVelocity - magVel*normal;

      curPos = curVertex->wPos + curVelocity * timeStep;
      //curPos = prevPos;
    }

    /*
      //if (false) {        curVertex->vel = -0.1*curVertex->vel;
        //get the triangle's normal, dot it with the curForce
        //subtract this*damping from the curForce      
  
        vec3 edge1, edge2, normal;
        edge1 = tri->vertices[2]->wPos - tri->vertices[0]->wPos;
        edge2 = tri->vertices[1]->wPos - tri->vertices[0]->wPos;
        normal = edge1^edge2;

        float dampingMultiplier = 0.7;
        vec3 penaltyForce = dampingMultiplier*(curForce*normal);
        vec3 collVel = curVertex->vel * normal;
        //curVertex->vel = curVertex->vel - 10*collVel;

        //curForce = curForce - penaltyForce;
        //curForce += vec3(0,500,0);
        
        //cout<<"intersecting tri"<<endl;
        //curVertex->vel = 0;
        //curForce = -curForce;
        //curForce = vec3(0,200,0);
        //cout << "intersection!" << endl;
        

        curForce = vec3(0,9.8,0) * curVertex->mass;
        curVertex->vel = vec3(0,0,0);
        */

if(curPos[1] < 0) {
  curPos[1] = 0;
}
    

    if (!curVertex->pinned) {
      curVertex->accel = curAccel;
      curVertex->vel = curVelocity;
      curVertex->wPos = curPos;
   } else {
     //curVertex->wPos = curVertex->wPos - vec3(0,0.001,0);
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
    //curVert->force += vec3(0,-9.8*curVert->mass,0);
  }

  vector<Triangle*>::iterator triangleIter;
  for (triangleIter = triangles->begin(); triangleIter != triangles->end(); 
      ++triangleIter) {
    nodeForce(*triangleIter, lame, mu, phi, psi);
  }
}

double Engine::vertexCollisionDetect(vec3 start, vec3 end, Triangle *tri, Vertex *ver){

  //cout << "Start: " << start << endl;
  //cout << "End: " << end << endl;
  
  if (ver == tri->vertices[0] || ver == tri->vertices[1] || ver == tri->vertices[2]) {
    //cout << "self intersection" << endl;
    return false;
  }

  double epsilon = 0.001;
  vec3 veca = tri->vertices[0]->wPos;
  vec3 vecb = tri->vertices[1]->wPos;
  vec3 vecc = tri->vertices[2]->wPos;

  //cout << "A: " <<veca <<endl << "B: " <<vecb <<endl << "C: " <<vecc <<endl;

  //vec3 vecd(_worldToModel * (ray.direction()), VW);
  //vec3 vece(_worldToModel * (ray.start()), VW);
  vec3 vecd = end-start;
  vec3 vece = start;
  //want nonzero length
  if (vecd.length2() < 0.0000000001) {
    //cout << "asdf: " << vecd.length2() << endl;
    return -1;
  }


  vec3 aminusb = veca - vecb;
  vec3 aminusc = veca - vecc;
  vec3 aminuse = veca - vece;

  mat3 cm = mat3(aminusb, aminusc, vecd);

  double eiminushf = (cm[1][1] * cm[2][2]) - (cm[2][1] * cm[1][2]);
  double gfminusdi = (cm[2][0] * cm[1][2]) - (cm[1][0] * cm[2][2]);
  double dhminuseg = (cm[1][0] * cm[2][1]) - (cm[1][1] * cm[2][0]);

  double akminusjb = (cm[0][0] * aminuse[1]) - (aminuse[0] * cm[0][1]);
  double jcminusal = (aminuse[0] * cm[0][2]) - (cm[0][0] * aminuse[2]);
  double blminuskc = (cm[0][1] * aminuse[2]) - (aminuse[1] * cm[0][2]);

  double m = (cm[0][0] * eiminushf) + (cm[0][1] * gfminusdi) + (cm[0][2] * dhminuseg);

  double t = - ( (cm[1][2] * akminusjb) + (cm[1][1] * jcminusal) + (cm[1][0] * blminuskc) ) /m;
  if(t < epsilon)
    return -1;

  double gamma = ( (cm[2][2] * akminusjb) + (cm[2][1] * jcminusal) + (cm[2][0] * blminuskc) ) / m;
  if(gamma < 0 || gamma > 1)
    return -1;

  double beta = ( (aminuse[0] * eiminushf) + (aminuse[1] * gfminusdi) + (aminuse[2] * dhminuseg) ) / m;
  if(beta < 0 || (beta > (1 - gamma)))
    return -1;

  //cout << "TR; " << vece << "/" << ray.start() << "," << vecd << "/" << ray.direction() << " intersects " << veca << "," << vecb << "," << vecc << endl << "@" << vece + (vecd * t) << endl;
  if (t >= 1-epsilon)
    return -1;
  //cout << start << ", " << end << endl;
  //cout << vecd << endl;
  //cout << vecd.length2() << endl;
  //cout << "(intersection t): " << t << endl;
  return t;

}

double Engine::edgeCollisionDetect(Edge *edge, Triangle *tri, Vertex *ver){
  vec3 start = edge->vertices[0]->wPos;
  vec3 end = edge->vertices[1]->wPos;
  return vertexCollisionDetect(start, end, tri, ver);
}
#if 0
//returns true if triangle intersects with ray
bool Engine::vertexCollisionDetect(vec3 start, vec3 end, Triangle *tri, Vertex *ver){
  vec3 u, v, n;
  vec3 dir, w0, w;
  float r, a, b;

  if (ver == tri->vertices[0] || ver == tri->vertices[1] || ver == tri->vertices[2]) {
    //cout << "self intersection" << endl;
    return false;
  }

  //triangle vertices
  vec3 t0 = tri->vertices[0]->wPos;
  vec3 t1 = tri->vertices[1]->wPos;
  vec3 t2 = tri->vertices[2]->wPos;

  u = t1 - t0;
  v = t2 - t0;

  n = u ^ v;
  if (n == vec3(0)){
    //cout<<1<<endl;
    return false; //degenerate tri
  }

  dir = end - start;
  dir.normalize();
  w0 = start - t0; 

  a = -1*(n*w0); 
  b =  n*dir;

  r = a/b; 

  if (r < 0.0) {//ray goes away from tri
    //cout<<3<<endl;
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
  cout << "D: " << D << endl;

  //get and test param coords
  float s, t;
  //cout<<uv*wv<<endl;
  //cout<<vv*wu<<endl;
  //cout<<D<<endl;
  s = ((uv*wv) - (vv*wu)) / D;

  if (s < 0.0 || s > 1.0){
    //cout<<4<<endl;
    //cout<<"s is "<<s<<endl;
    return false;
  }

  t = ((uv*wu) - (uu*wv)) / D;
  if (t < 0.0 || (s+t) > 1.0){
    //cout<<5<<endl;
    //cout<<"t is "<<t<<endl;
    return false;
  }


  float epsilon = 0.1;
  if ( t > 1-epsilon || t < 0+epsilon ) 
    return false;

  cout<<"t is "<<t<<endl;

  return true;
}
#endif
