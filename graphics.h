#ifndef __graphics__
#define __graphics__

#include "simulation.h"
#include "springmass.h"
#include "Figure.h"
#include <string>
#include <vector>
#include <windows.h>
#include "GL/glut.h"


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
  virtual void draw();
  virtual void updateAnimation();
  virtual void updateClip(GLfloat width, GLfloat height);
  virtual GLuint getDt();
  void initGL();

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

class Figure3D : public Figure {

};

#endif
