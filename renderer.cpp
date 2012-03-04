#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif


#include "renderer.h"
#include "squareMesh.h"

Renderer * activeRenderer;


void applyMat4(mat4 &mat) {
  double glmat[16];
  int k = 0;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      glmat[k++] = mat[j][i];
    }
  }
  glMultMatrixd(glmat);
}

void setActiveRenderer(Renderer * newRenderer)
{
  activeRenderer = newRenderer;
}

void display()
{
  activeRenderer->stepEngine();

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  applyMat4(activeRenderer->orientation);    
  //Wireframe
  glColor3f(1,1,1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  activeRenderer->draw();

  //Normal
  glColor3f(0.7,0.0,0.3);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  activeRenderer->draw();

  //Marked Vertices
  glColor3f(0.0,0.9,0.1);
  activeRenderer->drawMarkedPoints();
  
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  activeRenderer->width = w;
  activeRenderer->height = h;
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //glOrtho(-100, 100, -100, 100, 100, -100);
  //glOrtho(-10, 10, -10, 10, 10, -10);
  glOrtho(-1, 1, -1, 1, 1, -1);
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
    case 27:
      exit(0);
      break;
  }
  glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) 
{
}

void activeMotion(int x, int y)
{
  vec2 newMouse = vec2((double)x / activeRenderer->width,(double)y / 
      activeRenderer->height);
  vec2 diff = (newMouse - activeRenderer->mousePos);
  double len = diff.length();
  if (len > .001) {
    vec3 axis = vec3(diff[1]/len, diff[0]/len, 0);
    activeRenderer->orientation = activeRenderer->orientation * 
      rotation3D(axis, -180 * len);
  }

  //Record the mouse location for drawing crosshairs
  activeRenderer->mousePos = newMouse;

  //Force a redraw of the window.
  glutPostRedisplay();
}

void passiveMotion(int x, int y)
{ 
  //Record the mouse location for drawing crosshairs
  activeRenderer-> mousePos = vec2((double)x / activeRenderer->width,
      (double)y / activeRenderer->height);

  //Force a redraw of the window.
  glutPostRedisplay();
}


Renderer::Renderer()
{
  width = 600;
  height = 600;
  orientation = identity3D();
}

Renderer::~Renderer()
{
  triangles.clear();
}

void Renderer::init(int argc,char** argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);

  glutInitWindowSize(width, height); 
  glutInitWindowPosition(0,0); 
  glutCreateWindow("CS283 HW1");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(activeMotion);
  glutPassiveMotionFunc(passiveMotion);
  glutSpecialFunc(specialKeyboard);
  glutIdleFunc(display);

  // set some lights
  {
    float ambient[4] = { .1f, .1f, .1f, 1.f };
    float diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.f };
    float pos[4] = { 4, 25, 0, 0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
    glEnable(GL_LIGHT0);
  }
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  //glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);


  //enable face culling for removal
  //glEnable(GL_CULL_FACE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);



  //Generate the mesh
  generateMesh(triangles, vertices, 1.5, 0.5, 0.025);
  //generateMesh(triangles, vertices, 15, 5, 0.25);
  //generateMesh2(triangles, vertices, 2, 2, 0.2);

  //Set up the engine
  engine = Engine();
  engine.vertices = &vertices;
  engine.triangles = &triangles;

}

void Renderer::mainLoop() 
{
  glutMainLoop();
}

void Renderer::stepEngine()
{
  engine.step(0.0001);
}

void Renderer::drawMarkedPoints()
{

  glPointSize(7.0);
  glBegin(GL_POINTS);
  for(vector<Vertex *>::iterator vt = vertices.begin();
      vt != vertices.end(); ++vt) {
    Vertex * curVert = *vt;
    if(curVert->marked > 0) {
      //cout << "asdf" << endl;
      glVertex3f(curVert->wPos[0], curVert->wPos[1], 0);
      curVert->marked -= 1;
    }
  }
  glEnd();
}

void Renderer::draw()
{
    //bind buffers later instead of calling glVertex w/e
  glBegin(GL_TRIANGLES);
  for(vector<Triangle *>::iterator it = triangles.begin(); 
      it != triangles.end(); ++it) {

    Triangle * curTriangle = *it;
    for(vector<Vertex *>::iterator vt = curTriangle->vertices.begin();
        vt != curTriangle->vertices.end(); ++vt) {
      Vertex * curVert = *vt;
      glVertex3f(curVert->wPos[0], curVert->wPos[1], curVert->wPos[2]);
    }
  }
  glEnd();
  
}
