
#include "graphics.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#define PI 3.14159265f

Figure * figure;

/* Initialize OpenGL Graphics */
void Figure::initGL() {
  // Set "clearing"/background color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and Opaque
}

/* Called back when timer expired */
void Figure::Timer(int value) {
  glutPostRedisplay();                           // Post a re-pain request to activate display()
  glutTimerFunc(figure -> dt, Figure::Timer, 0); // next Timer call milliseconds later
}


Figure::Figure(Spring ** springs, Mass ** masses, SpringMass * SM, int m, int n, GLuint dt, double maxG, int divs):
springs(springs), masses(masses), springmass(SM), m(m), n(n), dt(dt), maxG(maxG), 
divs(divs),glCircle(0), glGrid(0) {

  figure = this; 
  glutInitDisplayMode(GLUT_DOUBLE);                 // Enable double buffered mode
  glutInitWindowSize(640, 480);                     // Set the window's initial width & height - non-square
  glutInitWindowPosition(50, 50);                   // Position the window's initial top-left corner
  glutCreateWindow("Masses Connected by Springs");  // Create window with the given title
  glutDisplayFunc(display);                         // Register callback handler for window re-paint event
  glutReshapeFunc(reshape);                         // Register callback handler for window re-size event
  glutTimerFunc(0, Timer, 0);                       // First timer call immediately
  initGL();                                         // Our own OpenGL initialization
  glutMainLoop();                                   // Enter infinite event loop

}

void Figure::drawCircle(GLfloat x, GLfloat y, GLfloat radius) 
{
/* 
 * Draw a circle at position (x,y) of a given radius
 */
  if (glCircle == 0) {
    glCircle = glGenLists(1);
    glNewList(glCircle, GL_COMPILE);
    
    glBegin(GL_TRIANGLE_FAN);       // Begin drawing triangle fan
    glColor3f(1,0,0);
    glVertex2f(0.0f, 0.0f);
    int numSegments = 100;
    GLfloat angle;
    for (int i = 0; i <= numSegments; ++i) {  // Create circle out of a triangle fan
      angle = i * 2.0f * PI / numSegments;
      glVertex2f(cos(angle), sin(angle));
    }

  glEnd();
  glEndList();
  } else {

    glPushMatrix();                                 // Save the current state before performing operations
    glTranslatef(x,y,0.0f);               // Translate shape to by x,y
    glScalef(radius,radius,radius);
    glCallList(glCircle);
    glPopMatrix(); 
  }

}

void Figure::drawLine(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat * color) {
/*
 * Function for drawing a line
*/

  glPushMatrix();  
  glLineWidth(1.0);
  glBegin(GL_LINES);
    glColor3f(*color,*(color + 1),*(color + 2));
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
  glEnd();
  glPopMatrix(); 
}

void Figure::drawString(GLfloat x, GLfloat y, std::string str) {
  
  glRasterPos2f(x,y);
  for (const char * cstr = str.c_str(); *cstr; ++cstr) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *cstr);
  }
}

void Figure::updateGrid() {

  // if (glGrid) {
  //   glDeleteLists(glGrid, 1);
  // }

  
  if (glGrid == 0) {
    glGrid = glGenLists(1);
    glNewList(glGrid, GL_COMPILE);

    double delta = 2.0/divs;

    std::stringstream sstr1;
    std::stringstream sstr2;

    for (int i = 0; i < divs + 1; ++i) {

      float val = -1 + i * delta;

      sstr1.str("");
      sstr2.str("");

      sstr1 << std::fixed << std::setprecision(1) << val*maxG;
      sstr2 << std::fixed << std::setprecision(1) << val*maxG;

      GLfloat color[] = {0,1,0};
      
      drawLine(val, val, -1, 1, color);
      drawLine(-1, 1, val, val, color);

      drawString(val + 2*pixelSize, 2*pixelSize, sstr1.str());

      if (i != divs/2) drawString(2*pixelSize, val + 2*pixelSize, sstr2.str());

    };

    glEndList();
  }


  glCallList(figure->glGrid);

}


void Figure::display(){
/* 
 * Call-back function for window repaint event. Call back when the window first appears
 * or whenever the window needs repainting 
 */
  glClear(GL_COLOR_BUFFER_BIT);                           // "Clear the buffer", i.e. set the background colour
  
  // glCallList(figure->glGrid);

  figure->updateGrid();

  // Draw the line
  for (int i = 0; i < (figure->n); ++i) {                 
    Mass * m1 = (*(figure->springs + i)) -> getMass1();   // Obtain the masses connected to this spring
    Mass * m2 = (*(figure->springs + i)) -> getMass2();

    GLfloat x1 = (m1 -> getPosition()).x/figure->maxG;       // Get their positions in the clipping area
    GLfloat y1 = (m1 -> getPosition()).y/figure->maxG;

    GLfloat x2 = (m2 -> getPosition()).x/figure->maxG;
    GLfloat y2 = (m2 -> getPosition()).y/figure->maxG;

    GLfloat color[] = {0,0,1};
    
    figure->drawLine(x1,x2,y1,y2, color);                        // Draw the line

  }
  
  // Draw each of the masses at their correct positions
  for (int i = 0; i < figure->m; ++i) {
    GLfloat x = ((*((figure->masses)+i))->getPosition()).x/figure->maxG;
    GLfloat y = ((*((figure->masses)+i))->getPosition()).y/figure->maxG;
    GLfloat r = (*((figure->masses)+i))->getRadius()/figure->maxG;

    figure->drawCircle(x,y,r);
  }

  glutSwapBuffers();  // The program is double buffered meaning that front and back buffer will be swapped
                      // alllowing for smoother animations
  (figure->springmass) -> step((double)(figure->dt)/1000);  // Update simulation

}

void Figure::reshape(GLsizei width, GLsizei height) {
/*
 * Call-back function for window re-size event. Required to maintain the correct proportions
 * of the shapes.
*/

  if (height == 0) height = 1;
  GLfloat aspect = (GLfloat) width / (GLfloat)height; // Compute the aspect ratio

  glViewport(0,0,width,height); // Set the view port to cover the new window

  glMatrixMode(GL_PROJECTION);  // Use the projection matrix
  glLoadIdentity();             // Reset the projection matrix

  figure->pixelSize = 2.0/height;

  //Set the clipping area of the viewport to match the viewport
  if(width >= height) {                                 // Ensure the smallest dimension is of length 1
    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0); // Set the viewport dimensions
  
  } else {
    gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect); 
  }

}
