
#include "graphics.h"
#include "GL/glut.h"
#include "springmass.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int main(int argc, char** argv)
{
  // Physical constants for the springs and masses
  const double mass = 0.05 ;
  const double radius = 2 ;
  const double naturalLength = 60 ;
  const double stiffness = 0.1 ;
  const double damping = 0.01;

  // Instantiating the mass objects
  Mass m1(Vector2(-50,50), Vector2(), mass, radius) ;
  Mass m2(Vector2(50,50), Vector2(), mass, radius) ;
  Mass m3(Vector2(-50,-50), Vector2(), mass, radius) ;
  Mass m4(Vector2(+50,-50), Vector2(), mass, radius) ;
  Mass m5(Vector2(0,.6), Vector2(), mass*2, radius) ;

  Mass * masses[] = {&m1, &m2, &m3, &m4} ;   // Create an array of pointers to the mass


  //Instantiate a bunch of spring objects
  Spring spring1(&m1, &m2, naturalLength, stiffness, damping) ;
  Spring spring2(&m1, &m3, naturalLength, stiffness, damping) ;
  Spring spring3(&m2, &m4, naturalLength, stiffness, damping) ;
  Spring spring4(&m3, &m4, naturalLength, stiffness, damping) ;
  Spring spring5(&m1, &m4, naturalLength, stiffness, damping) ;
  Spring spring6(&m1, &m5, naturalLength, stiffness, damping) ;
  Spring spring7(&m2, &m5, naturalLength, stiffness, damping) ;

  Spring * springs[] = {&spring1, &spring2, &spring3, &spring4, &spring5} ;


  const int m = sizeof(masses)/sizeof(*masses);   // Work out the number of masses
  const int n = sizeof(springs)/sizeof(*springs); // Work out the number of springs

  SpringMass springmass(masses, springs, m, n) ;

  
  glutInit(&argc,argv) ;                          //Initialise glut

  Figure2D figure(springs, masses, &springmass, m, n, 30, 100, 10); 
  
  return 0 ;
}
