/** file: test-srpingmass.cpp
 ** brief: Tests the spring mass simulation
 ** author: Andrea Vedaldi
 **/

#include "springmass.h"

int main(int argc, char** argv)
{

  const double mass = 0.05 ;
  const double radius = 0.02 ;
  const double naturalLength = 0.95 ;
  const double stiffness = 0.1 ;

  Mass m1(Vector2(-.5,.5), Vector2(), mass, radius) ;
  Mass m2(Vector2(+.5,.5), Vector2(), mass, radius) ;
  Mass m3(Vector2(-.5,-.5), Vector2(), mass, radius) ;
  Mass m4(Vector2(+.5,-.5), Vector2(), mass, radius) ;

  Mass * masses[] = {&m1, &m2, &m3, &m4} ;

  Spring spring1(&m1, &m2, naturalLength, stiffness) ;
  Spring spring2(&m1, &m3, naturalLength, stiffness) ;
  Spring spring3(&m2, &m4, naturalLength, stiffness) ;
  Spring spring4(&m3, &m4, naturalLength, stiffness) ;

  Spring * springs[] = {&spring1, &spring2, &spring3, &spring4} ;
  
  /* change  springmass to take an array of springs and masses*/
  SpringMass springmass(masses, springs,4 ,4) ;

  const double dt = 1.0/30 ;
  for (int i = 0 ; i < 100 ; ++i) {
    springmass.step(dt) ;
    springmass.display() ;
  }

  return 0 ;

}
