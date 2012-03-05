#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif


#include "renderer.h"
#include "squareMesh.h"
#include "shaders.h"

// Constants to set up lighting on the teapot
const GLfloat light_position[] = {0,5,10,1};    // Position of light 0
const GLfloat light_position1[] = {0,5,-10,1};  // Position of light 1
const GLfloat light_specular[] = {0.6,0.6,0.6,1};    // Specular of light 0
const GLfloat light_specular1[] = {0.6,0.6,0.6,1};   // Specular of light 1
const GLfloat one[] = {1,1,1,1};                 // Specular on teapot
const GLfloat medium[] = {1,1,1,1};        // Diffuse on teapot
const GLfloat small[] = {0.2,0.2,0.2,1};         // Ambient on teapot
const GLfloat none[] = {0,0,0,1};
const GLfloat red[] = {1,0,0,1};
const GLfloat high[] = {100};                      // Shininess of teapot
GLfloat light0[4],light1[4]; 

GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
GLuint islight; 
GLuint light0posn; 
GLuint light0color; 
GLuint light1posn; 
GLuint light1color; 
GLuint ambient; 
GLuint diffuse; 
GLuint specular; 
GLuint shininess; 


Renderer * activeRenderer;

void transformvec (const GLfloat input[4],GLfloat output[4]) {
  GLfloat modelview[16]; // in column major order
  glGetFloatv(GL_MODELVIEW_MATRIX,modelview); 

  for (int i = 0; i < 4; i++) {
    output[i] = 0; 
    for (int j = 0; j < 4; j++) 
      output[i] += modelview[4*j+i] * input[j]; 
  }
}

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
  transformvec(light_position,light0); 
	transformvec(light_position1,light1); 

  glUniform4fv(light0posn,1,light0); 
  glUniform4fv(light0color,1,light_specular); 
  glUniform4fv(light1posn,1,light1); 
  glUniform4fv(light1color,1,light_specular1); 

  //glUniform4fv(ambient,1,small); 
  glUniform4fv(diffuse,1,medium); 
  glUniform4fv(ambient,1,small); 
  //glUniform4fv(diffuse,1,small); 
  glUniform4fv(specular,1,none); 
  glUniform1fv(shininess,1,high); 
  glUniform1i(islight,true);

  //Wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  activeRenderer->draw();

  //Normal
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  activeRenderer->draw();

  //Marked Vertices
  glUniform4fv(specular,1,red); 

  glUniform4fv(diffuse,1,red); 
  glUniform4fv(ambient,1,red); 
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
  
  //glOrtho(-1.5, 1.5, -1.5, 1.5, 1.5, -1.5);

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
  width = 1024;
  height = 800;
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

  //shaders
  vertexshader = initshaders(GL_VERTEX_SHADER,"shaders/light.vert.glsl");
  fragmentshader = initshaders(GL_FRAGMENT_SHADER,"shaders/light.frag.glsl");
  shaderprogram = initprogram(vertexshader,fragmentshader); 
  islight = glGetUniformLocation(shaderprogram,"islight");        
  light0posn = glGetUniformLocation(shaderprogram,"light0posn");       
  light0color = glGetUniformLocation(shaderprogram,"light0color");       
  light1posn = glGetUniformLocation(shaderprogram,"light1posn");       
  light1color = glGetUniformLocation(shaderprogram,"light1color");       
  ambient = glGetUniformLocation(shaderprogram,"ambient");       
  diffuse = glGetUniformLocation(shaderprogram,"diffuse");       
  specular = glGetUniformLocation(shaderprogram,"specular");       
  shininess = glGetUniformLocation(shaderprogram,"shininess");       


  //Generate the mesh
  //generateMesh(triangles, vertices, 1.5, 0.5, 0.025);
  //generateMesh(triangles, vertices, 15, 5, 0.25);
  //generateMesh3(triangles, vertices, edges, 1, 1, 0.05);
  //generateMesh(triangles, vertices, edges, 1, 1, 0.025);
  generateMesh(triangles, vertices, edges, 1, 1, 0.025);
  //generateMesh2(triangles, vertices, edges, 1, 1, 0.05);
  //loadOBJ(triangles, vertices, edges, "sphere.obj");

  //Set up the engine
  engine = Engine();
  engine.init(&vertices, &triangles, &edges);


}

void Renderer::mainLoop() 
{
  glutMainLoop();
}

void Renderer::stepEngine()
{
  engine.step(0.00007);
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
  //compute normals
  for(vector<Vertex *>::iterator vt = vertices.begin(); vt != vertices.end(); ++vt) {
    (*vt)->norm = vec3(0);
    (*vt)->normCount = 0;
  }
  for(vector<Triangle *>::iterator it = triangles.begin(); 
      it != triangles.end(); ++it) {
      Triangle * curTriangle = *it;
      
    vec3 curNorm = ((curTriangle->vertices[1]->wPos - curTriangle->vertices[0]->wPos)
                    ^ (curTriangle->vertices[2]->wPos - curTriangle->vertices[0]->wPos)).normalize();

        //front facing, hack for now
    if (curNorm * vec3(0,0,1) < 0) {
      curNorm = -curNorm;
    }

      for(vector<Vertex *>::iterator vt = curTriangle->vertices.begin();
          vt != curTriangle->vertices.end(); ++vt) {
        Vertex * curVert = *vt;
        curVert->norm += curNorm;
        curVert->normCount += 1;
      }

  }
  for(vector<Vertex *>::iterator vt = vertices.begin(); vt != vertices.end(); ++vt) {
    (*vt)->norm /= ((*vt)->normCount);
  }


  //bind buffers later instead of calling glVertex w/e
  glBegin(GL_TRIANGLES);
  for(vector<Triangle *>::iterator it = triangles.begin(); 
      it != triangles.end(); ++it) {

    Triangle * curTriangle = *it;

    vec3 curNorm = ((curTriangle->vertices[1]->wPos - curTriangle->vertices[0]->wPos)
                    ^ (curTriangle->vertices[2]->wPos - curTriangle->vertices[0]->wPos)).normalize();


    for(vector<Vertex *>::iterator vt = curTriangle->vertices.begin();
        vt != curTriangle->vertices.end(); ++vt) {
      Vertex * curVert = *vt;
      
      glNormal3f(curVert->norm[0], curVert->norm[1], curVert->norm[2]);
      glVertex3f(curVert->wPos[0], curVert->wPos[1], curVert->wPos[2]);
    }
  }
  glEnd();

}
