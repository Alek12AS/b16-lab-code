#ifndef __graphics__
#define __graphics__

#include "simulation.h"
#include "springmass.h"
#include "Figure.h"
#include <string>
#include <vector>
#include <windows.h>
#include <optional>
#include "GL/glut.h"

/* ---------------------------------------------------------------- */
// class Figure2D : public Figure
/* ---------------------------------------------------------------- */


class Figure2D : public Figure
{

/*
 * Constructor Parameters:
 * springs  = a pointer to an array of pointers to spring objects 
 * masses   = a pointer to an array of pointers to mass objects
 * SM       = a springmass object
 * m        = the number of masses
 * n        = the number of springs
 * dt       = the time interval between updates of the simulation
 * 
 * Overview:
 * Class for creating an animation of a network of m circular masses
 * and n springs.
 */

public:
 
  Figure2D(Spring ** springs, Mass ** masses, SpringMass * SM, int m, int n, GLuint dt, double maxG, int divs); 
  void setTimeInterval(GLuint dt);
  void initGL();
  virtual void draw();
  virtual void updateAnimation();
  virtual void updateClip(GLfloat width, GLfloat height);
  virtual GLuint getDt();

protected:

  double maxG;
  double pixelSize;
  int divs;
  int m;
  int n;
  GLuint dt;
  GLuint glCircle;
  GLuint glGrid;


  Spring ** springs;
  Mass ** masses;
  SpringMass * springmass;
  void drawCircle(GLfloat x, GLfloat y, GLfloat radius);
  void drawLine(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat * color);
  void drawGrid();
  void drawString(GLfloat x, GLfloat y, std::string val);

};

/* ---------------------------------------------------------------- */
// class Figure3D : public Figure
/* ---------------------------------------------------------------- */

class Figure3D : public Figure {

public:

  Figure3D(Spring ** springs, Mass ** masses, SpringMass * SM, int m, int n, GLuint dt, double maxG);
  void setTimeInterval(GLuint dt);
  void initGL();
  virtual void draw();
  virtual void updateAnimation();
  virtual void updateClip(GLfloat width, GLfloat height);
  virtual GLuint getDt();

protected:

  double maxG;
  double rotIncr;
  double moveIncr;
  int m;
  int n;
  Vector3 centroid;             // Centre of Mass
  Vector3 CE;                   // Vector from the centroid to the eye
  Vector3 up;
  Vector3 planeVec;             // Used to obtain the up vector
  GLuint dt;
  GLuint glSphere;


  Spring ** springs;
  Mass ** masses;
  SpringMass * springmass;
  void drawSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
  void drawLine(GLfloat * coords, GLfloat * color);
  void rotate(Vector3 & p, bool cc, bool hp);
  void findCentroid();
  virtual void rightOrLeftKey(bool r);
  virtual void upOrDownKey(bool u);
  virtual void qOrEKey(bool q);

};

#endif
