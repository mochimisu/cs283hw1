

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif


#include "renderer.h"

Renderer * activeRenderer;

void setActiveRenderer(Renderer * newRenderer)
{
  activeRenderer = newRenderer;
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  activeRenderer->draw();

  glutSwapBuffers();
}

void reshape(int w, int h)
{
  activeRenderer->width = w;
  activeRenderer->height = h;
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-1, 1, -1, 1, 1, -1);
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
    case 27:
      exit(0);
      break;
  }
}

void specialKeyboard(int key, int x, int y) 
{
}

void activeMotion(int x, int y)
{
}

void passiveMotion(int x, int y)
{
}


Renderer::Renderer()
{
  width = 600;
  height = 600;
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
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  
  //enable face culling for removal
  //glEnable(GL_CULL_FACE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

  //Set up the engine
  engine = Engine();
}

void Renderer::mainLoop() 
{
  glutMainLoop();
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


  
  //----------------------- code to draw objects --------------------------
  // Rectangle Code
  //glColor3f(red component, green component, blue component);
  glColor3f(1.0f,0.0f,0.0f);					// setting the color to pure red 90% for the rect

  glBegin(GL_POLYGON);						// draw rectangle 
  //glVertex3f(x val, y val, z val (won't change the point because of the projection type));
  glVertex3f(-0.8f, 0.0f, 0.0f);			// bottom left corner of rectangle
  glVertex3f(-0.8f, 0.5f, 0.0f);			// top left corner of rectangle
  glVertex3f( 0.0f, 0.5f, 0.0f);			// top right corner of rectangle
  glVertex3f( 0.0f, 0.0f, 0.0f);			// bottom right corner of rectangle
  glEnd();
}
