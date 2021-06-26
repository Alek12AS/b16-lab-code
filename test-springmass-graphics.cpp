
#include "graphics.h"
#include "GL/glut.h"
#include "springmass.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int main(int argc, char** argv)
{

  const double mass = 0.5 ;
  const double radius = 0.02 ;
  const double naturalLength = 0.95 ;
  const double stiffness = 0.1 ;

  Mass m1(Vector2(-.5,0), Vector2(-0.1,0), mass, radius) ;
  Mass m2(Vector2(+.5,0), Vector2(0.1,0), mass, radius) ;

  Spring spring1(&m1, &m2, naturalLength, stiffness) ;

  Mass * masses[] = {&m1, &m2};
  Spring * springs[] = {&spring1};
  SpringMass springmass(&spring1, &m1, &m2,0);

  glutInit(&argc,argv) ;

  Figure figure(springs, masses, &springmass, 2 , 1, 30); 
  
  return 0 ;
}
