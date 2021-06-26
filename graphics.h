#ifndef __graphics__
#define __graphics__

#include "simulation.h"
#include "springmass.h"
#include <string>
#include <vector>
#include <windows.h>
#include "GL/glut.h"


class Figure {

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
  Figure(Spring ** springs, Mass ** masses, SpringMass * SM, int m, int n, GLuint dt);
  void setTimeInterval(GLuint dt);
  void run();
  static void display();
  static void reshape(GLsizei width, GLsizei height);
  static void Timer(int Value);
  void initGL();

protected:
  Spring ** springs;
  Mass ** masses;
  SpringMass * springmass;
  int m;
  int n;
  GLuint dt;
  void drawCircle(GLfloat x, GLfloat y, GLfloat radius);
  void drawLine(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2);

};

#endif
