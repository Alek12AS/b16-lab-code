
#include "graphics.h"
#include "Figure.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#define PI 3.14159265358979323846

Figure * figure;

/* ---------------------------------------------------------------- */
// class Figure
/* ---------------------------------------------------------------- */

void Figure::display(){
/* 
 * Call-back function for window repaint event. Call back when the window first appears
 * or whenever the window needs repainting 
 */
  
  figure -> draw();

  glutSwapBuffers();  // The program is double buffered meaning that front and back buffer will be swapped
                      // alllowing for smoother animations

  figure -> updateAnimation();

}

/* Called back when timer expired */
void Figure::Timer(int value) {
  glutPostRedisplay();                                   // Post a re-paint request to activate display()
  glutTimerFunc(figure -> getDt(), Figure::Timer, 0);    // next Timer call milliseconds later
}

void Figure::specialKeys(int key, int x, int y) {
  
  switch(key) {
    case GLUT_KEY_RIGHT:
      figure->rightOrLeftKey(true); break;

    case GLUT_KEY_LEFT:
      figure->rightOrLeftKey(false); break;

    case GLUT_KEY_UP:
      figure->upOrDownKey(true); break;

    case GLUT_KEY_DOWN:
      figure->upOrDownKey(false); break;
  } 
}

void Figure::keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case 113: // q
      figure->qOrEKey(true); break;
    case 101: // e
      figure->qOrEKey(false); break;
  }
}

void Figure::rightOrLeftKey(bool b) {
  // to implement
}
void Figure::upOrDownKey(bool b) {
  // to implement
}
void Figure::qOrEKey(bool q) {
  // to implement
}


/* ---------------------------------------------------------------- */
// class Figure2D : public Figure
/* ---------------------------------------------------------------- */


Figure2D::Figure2D(Spring ** springs, Mass ** masses, SpringMass * SM, int m, int n, double maxG, int divs):
springs(springs), masses(masses), springmass(SM), m(m), n(n), dt(30), maxG(maxG), 
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

/* Initialize OpenGL Graphics */
void Figure2D::initGL() {
  // Set "clearing"/background color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and Opaque
}

void Figure2D::drawCircle(GLfloat x, GLfloat y, GLfloat radius) 
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
    glTranslatef(x,y,0.0f);                         // Translate shape to by x,y
    glScalef(radius,radius,radius);
    glCallList(glCircle);
    glPopMatrix(); 
  }

}

void Figure2D::drawLine(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat * color) {
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

void Figure2D::drawString(GLfloat x, GLfloat y, std::string str) {
  
  glRasterPos2f(x,y);
  for (const char * cstr = str.c_str(); *cstr; ++cstr) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *cstr);
  }
}

void Figure2D::drawGrid() {

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


  glCallList(glGrid);

}


void Figure2D::draw() {

  glClear(GL_COLOR_BUFFER_BIT);                    // "Clear the buffer", i.e. set the background colour

  drawGrid();

  // Draw the lines
  for (int i = 0; i < (n); ++i) {                 
    Mass * m1 = (*(springs + i)) -> getMass1();   // Obtain the masses connected to this spring
    Mass * m2 = (*(springs + i)) -> getMass2();

    GLfloat x1 = (m1 -> getPosition()).x/maxG;    // Get their positions in the clipping area
    GLfloat y1 = (m1 -> getPosition()).y/maxG;

    GLfloat x2 = (m2 -> getPosition()).x/maxG;
    GLfloat y2 = (m2 -> getPosition()).y/maxG;

    GLfloat color[] = {0,0,1};
    
    drawLine(x1,x2,y1,y2, color);
  }

    // Draw each of the masses at their correct positions
    for (int i = 0; i < m; ++i) {
      GLfloat x = ((*(masses+i))->getPosition()).x/maxG;
      GLfloat y = ((*(masses+i))->getPosition()).y/maxG;
      GLfloat r = (*(masses+i))->getRadius()/maxG;

      drawCircle(x,y,r);
    }

}   


void Figure2D::updateAnimation() {
  springmass -> step((double)dt/1000);
}

void Figure::reshape(GLsizei width, GLsizei height) {
/*
 * Call-back function for window re-size event. Required to maintain the correct proportions
 * of the shapes.
*/

  if (height == 0) height = 1;
  GLfloat aspect = (GLfloat) width / (GLfloat)height; // Compute the aspect ratio

  glViewport(0,0,width,height); // Set the view port to cover the new window

  figure -> updateClip(width, height);

}

void Figure2D::updateClip(GLfloat width, GLfloat height) {

  GLfloat aspect = width/height;

  glMatrixMode(GL_PROJECTION);  // Use the projection matrix
  glLoadIdentity();             // Reset the projection matrix

  if(aspect >= 1) {                                 // Ensure the smallest dimension is of length 1
    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0); // Set the viewport dimensions
  
  } else {
    gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect); 
  }

  pixelSize = 2.0/height;

}

GLuint Figure2D::getDt() {
  return dt;
}

/* ---------------------------------------------------------------- */
// class Figure3D
/* ---------------------------------------------------------------- */

Figure3D::Figure3D(Spring ** springs, Mass ** masses, SpringMass * SM, int m, int n, double maxG):
springs(springs), masses(masses), springmass(SM), m(m), n(n), dt(30), glSphere(0), maxG(maxG), rotIncr(2),
moveIncr(0.2) {

  findCentroid();                                   // Find the intial value of the centroid and CE to be used
                                                    // for adjusting the view 
  figure = this; 
  glutInitDisplayMode(GLUT_DOUBLE);                 // Enable double buffered mode
  glutInitWindowSize(640, 640);                     // Set the window's initial width & height - non-square
  glutInitWindowPosition(50, 50);                   // Position the window's initial top-left corner
  glutCreateWindow("Masses Connected by Springs");  // Create window with the given title
  glutDisplayFunc(display);                         // Register callback handler for window re-paint event
  glutReshapeFunc(reshape);                         // Register callback handler for window re-size event
  glutTimerFunc(0, Timer, 0);                       // First timer call immediately
  glutSpecialFunc(specialKeys);                     // Register call-back handler for special key events
  glutKeyboardFunc(keyboard);                       // Register call-back handler for the other keys
  initGL();                                         // Our own OpenGL initialization
  glutMainLoop();                                   // Enter infinite event loop

}

/* Initialize OpenGL Graphics */
void Figure3D::initGL() {

  // Set "clearing"/background color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Black and Opaque
  glClearDepth(1.0f);                                     // Set background depth to farthest
  
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };      // Position of the light source 
  GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };      // Global ambient light available
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); //
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHTING);                                  // Enable lighting for opengl to perform calculations
  glEnable(GL_LIGHT0);                                    // Enable the white light source

  glShadeModel (GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);                                // Enable depth testing for z culling
  glDepthFunc(GL_LEQUAL);                                 // Set the type of of depth test
  glShadeModel(GL_SMOOTH);                                // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);       // Nice perspective correction
}

void Figure3D::draw() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear colour and depth buffer
  glMatrixMode(GL_MODELVIEW);

  // Draw the lines
  for (int i = 0; i < n; ++i) {                 
    Mass * m1 = (*(springs + i)) -> getMass1();   // Obtain the masses connected to this spring
    Mass * m2 = (*(springs + i)) -> getMass2();

    GLfloat x1 = (m1 -> getPosition()).x/maxG;    // Get their positions in the clipping area
    GLfloat y1 = (m1 -> getPosition()).y/maxG;
    GLfloat z1 = (m1 -> getPosition()).z/maxG;

    GLfloat x2 = (m2 -> getPosition()).x/maxG;
    GLfloat y2 = (m2 -> getPosition()).y/maxG;
    GLfloat z2 = (m2 -> getPosition()).z/maxG;

    GLfloat color[] = {0,0,1};

    GLfloat coords[] = {x1,y1,z1,x2,y2,z2};

    drawLine(coords, color);

    Vector3 newPos = CE + centroid;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(newPos.x, newPos.y, newPos.z, centroid.x, centroid.y, centroid.z, up.x, up.y, up.z);
  }

  // Draw each of the masses at their correct positions
  for (int i = 0; i < m; ++i) {
    GLfloat x = ((*(masses+i))->getPosition()).x/maxG;
    GLfloat y = ((*(masses+i))->getPosition()).y/maxG;
    GLfloat z = ((*(masses+i))->getPosition()).z/maxG;
    GLfloat r = (*(masses+i))->getRadius()/maxG;

    drawSphere(x,y,z,r);
  }
                    
}

void Figure3D::drawSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
  
  /* 
   * Draw a circle at position (x,y) of a given radius
   */
   

  if (glSphere == 0) {
    glSphere = glGenLists(1);
    glNewList(glSphere, GL_COMPILE);
    
    GLfloat mat_shininess[] = { 50 };                                       // Shininess value out of 100
    GLfloat mat_specular[] = { 0, 0.702, 1.0, 1.0 };                        // Specular reflectivity
    GLfloat mat_amb_diff[] = { 0, 0.702, 1.0, 1.0 };                        // ambient and diffuse reflectivity
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);  
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    // gluSphere(glQuad,1.0,3,3);
    glutSolidSphere(1.0,50,50);
    
    glEndList();
  } else {

    glPushMatrix();                                 // Save the current state before performing operations
    glTranslatef(x,y,z);                            // Translate shape to by x,y
    glScalef(radius,radius,radius);
    glCallList(glSphere);
    glPopMatrix(); 
  }

}

void Figure3D::drawLine(GLfloat * coords, GLfloat * color) {
  glPushMatrix();  
  
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat mat_specular[] = { 0, 1.0, 0, 1.0 };
  GLfloat mat_amb_diff[] = { 0, 1.0, 0, 1.0 };
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glLineWidth(1.0);
  glBegin(GL_LINES);
    glColor3f(*color,*(color + 1),*(color + 2));
    glVertex3f(*(coords),*(coords+1),*(coords+2));
    glVertex3f(*(coords+3),*(coords+4),*(coords+5));
  glEnd();
  glPopMatrix(); 
}


void Figure3D::updateAnimation() {
  springmass->step((double)dt/1000);
}

void Figure3D::updateClip(GLfloat width, GLfloat height) {
  
  GLfloat aspect = width/height;

  glMatrixMode(GL_PROJECTION);  // Use the projection matrix
  glLoadIdentity();             // Reset the projection matrix

  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

GLuint Figure3D::getDt() {
  return dt;
}

void Figure3D::findCentroid() {

  centroid.x = 0;
  centroid.y = 0;
  centroid.z = 0;
  for(int i = 0; i < m; ++i) {
    centroid = centroid + (*(masses + i))->getPosition();
  }

  centroid = centroid/m;

  CE = -1 * centroid;
  planeVec.x = 1;
  up.y = 1;
}


void Figure3D::rightOrLeftKey(bool r){
  
  if (r) {
    rotate(CE, false, true);
    
  } else {
    rotate(CE, true, true);
    
  }
}


void Figure3D::upOrDownKey(bool u) {
  
  if (u) {
    rotate(CE, false, false);

  } else {
    rotate(CE, true, false);
  
    }
}

void Figure3D::qOrEKey(bool q) {
  
  Vector3 delta = CE/CE.norm() * moveIncr;
  
  if (q) {
    CE = CE - delta;
  } else {
    CE = CE + delta;
  }
}


void Figure3D::rotate(Vector3 & p, bool cc, bool hp) {

  if (cc && hp) {
    double x = p.x;
    double z = p.z;
    p.x  = x * std::cos(rotIncr*PI/180) - z * std::sin(rotIncr*PI/180);
    p.z = x * std::sin(rotIncr*PI/180) + z * std::cos(rotIncr*PI/180);
  } else if (!cc && hp) {
    double x = p.x;
    double z = p.z;
    p.x  = x * std::cos(rotIncr*PI/180) + z * std::sin(rotIncr*PI/180);
    p.z = - x * std::sin(rotIncr*PI/180) + z * std::cos(rotIncr*PI/180);
  } else if (cc && !hp) {
    double z = p.z;
    double y = p.y;
    p.z  = z * std::cos(rotIncr*PI/180) - y * std::sin(rotIncr*PI/180);
    p.y = z * std::sin(rotIncr*PI/180) + y * std::cos(rotIncr*PI/180);
  } else {
    double z = p.z;
    double y = p.y;
    p.z  = z * std::cos(-rotIncr*PI/180) - y * std::sin(-rotIncr*PI/180);
    p.y = z * std::sin(-rotIncr*PI/180) + y * std::cos(-rotIncr*PI/180);
  }
  
}