/*
 * CS283 HW1
 *
 * Simulation
 *
 * Created on: February 15, 2012
 *     Author: bmwang, yglee
 */

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <algebra3.h>
#include "vertex.h"
#include "triangle.h"
#include "viewport.h"

#include <time.h>
#include <math.h>

#define PI 3.14159265

#include "squareMesh.h"

//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  // sets the rectangle that will be the window
  glViewport(0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  // loading the identity matrix for the screen
  glLoadIdentity();

  //----------- setting the projection -------------------------
  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
  // glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

  glOrtho(-1, 1, -1, 1, 1, -1);	// resize type = stretch

  //------------------------------------------------------------
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  myReshape(viewport.w,viewport.h);
}


//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer (sets everything to black)

  glMatrixMode(GL_MODELVIEW);					// indicate we are specifying camera transformations
  glLoadIdentity();							// make sure transformation is "zero'd"

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
  // Triangle Code
  glColor3f(1.0f,0.5f,0.0f);					// setting the color to orange for the triangle

#if 0
	vector<Triangle*> triangles;
	vector<Vertex*> vertices;

	generateMesh(triangles, vertices, 100.0, 100.0, 1.0);

	vector<Triangle*>::iterator t;

	for(t = triangles.begin(); t != triangles.end(); ++t) {
		glVertex3f();
	}

#endif
  glFlush();
  glutSwapBuffers();
}


//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  //give ~10ms back to OS (so as not to waste the CPU)
  Sleep(10);
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("CS283 HW1");

  //Set up the scene
  initScene();

  // function to run when its time to draw something
  glutDisplayFunc(myDisplay);
  // function to run when the window gets resized
  glutReshapeFunc(myReshape);
  // function to run when not handling any other task
  glutIdleFunc(myFrameMove);
  // infinite loop that will keep drawing and resizing and whatever else
  glutMainLoop();

  return 0;
}








