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

//#include "squareMesh.h"

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

	glColor3f(1.0f, 0.5f, 0.0f);

	vector<Triangle*> triangles;
	vector<Vertex*> vertices;
	generateMesh(triangles, vertices, 0.5, 0.5, 0.1);

	vector<Triangle*>::iterator t;

	for(t = triangles.begin(); t != triangles.end(); ++t) {
		Triangle *tri = *t; 
		vector<Vertex*> triVertices = tri->vertices;
		
		Vertex* v1 = triVertices[0];
		Vertex* v2 = triVertices[1];
		Vertex* v3 = triVertices[2];

		vec3 pos1 = v1->wPos;
		vec3 pos2 = v2->wPos;
		vec3 pos3 = v3->wPos;

		glBegin(GL_POLYGON);
		glVertex3f((float) pos1[0], (float) pos1[1], (float) pos1[2]);
		glVertex3f((float) pos2[0], (float) pos2[1], (float) pos2[2]);
		glVertex3f((float) pos3[0], (float) pos3[1], (float) pos3[2]);
		glEnd();
	}

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
int mainold(int argc, char *argv[]) {
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

