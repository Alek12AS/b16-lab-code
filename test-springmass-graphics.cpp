
#include "graphics.h"
#include "GL/glut.h"
#include "springmass.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int main(int argc, char** argv)
{
  // Physical constants for the springs and masses
  const double mass = 0.1 ;
  const double radius = 0.05 ;
  const double naturalLength = 1.5 ;
  const double stiffness = 0.01 ;
  const double damping = 0.005;

  // Instantiating the mass objects
  Mass m1(Vector3(0,0,-5), Vector3(), mass, radius) ;
  Mass m2(Vector3(-0.5,0,-5.8660), Vector3(), mass, radius) ;
  Mass m3(Vector3(0.5,0,-5.8660), Vector3(), mass, radius) ;
  Mass m4(Vector3(0,0.8165,-5.4330), Vector3(), mass, radius) ;
  Mass m5(Vector3(0,-0.8165,-5.4330), Vector3(), mass, radius) ;
  Mass m6(Vector3(0.5,0), Vector3(), mass, radius) ;
  Mass m7(Vector3(-0.5,0), Vector3(), mass, radius) ;

  Mass * masses[] = {&m6, &m7} ;             // Create an array of pointers to the mass


  //Instantiate a bunch of spring objects
  Spring spring1(&m1, &m2, naturalLength, stiffness, damping) ;
  Spring spring2(&m2, &m3, naturalLength, stiffness, damping) ;
  Spring spring3(&m1, &m3, naturalLength, stiffness, damping) ;
  Spring spring4(&m4, &m1, naturalLength, stiffness, damping) ;
  Spring spring5(&m4, &m2, naturalLength, stiffness, damping) ;
  Spring spring6(&m4, &m3, naturalLength, stiffness, damping) ;
  Spring spring7(&m5, &m1, naturalLength, stiffness, damping) ;
  Spring spring8(&m5, &m2, naturalLength, stiffness, damping) ;
  Spring spring9(&m5, &m3, naturalLength, stiffness, damping) ;
  Spring spring10(&m6, &m7, naturalLength, stiffness, damping) ;

  Spring * springs[] = {&spring10} ;


  const int m = sizeof(masses)/sizeof(*masses);   // Work out the number of masses
  const int n = sizeof(springs)/sizeof(*springs); // Work out the number of springs

  SpringMass springmass(masses, springs, m, n) ;

  
  glutInit(&argc,argv) ;                          //Initialise glut

  Figure2D figure(springs, masses, &springmass, m, n, 30, 1, 10); 
  // Figure3D figure(springs, masses, &springmass, m, n, 30, 1); 
  

  
  return 0 ;
}
