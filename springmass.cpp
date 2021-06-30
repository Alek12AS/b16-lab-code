
#include "springmass.h"
#include <cmath>
#include <iostream>

/* ---------------------------------------------------------------- */
// class Mass
/* ---------------------------------------------------------------- */

Mass::Mass()
: position(), velocity(), force(), mass(1), radius(1)
{ }

Mass::Mass(Vector3 position, Vector3 velocity, double mass, double radius)
: position(position), velocity(velocity), force(), mass(mass), radius(radius),
xmin(-1),xmax(1),ymin(-1),ymax(1)
{ }

void Mass::setForce(Vector3 f)
{
  force = f ;
}

void Mass::addForce(Vector3 f)
{
  force = force + f ;
}

Vector3 Mass::getForce() const
{
  return force ;
}

Vector3 Mass::getPosition() const
{
  return position ;
}

Vector3 Mass::getVelocity() const
{
  return velocity ;
}

double Mass::getRadius() const
{
  return radius ;
}

double Mass::getMass() const
{
  return mass ;
}

double Mass::getEnergy(double gravity) const
{
  double energy = 0 ;

  energy = mass*gravity*position.y + 0.5*mass*std::pow(velocity.norm2(),2) ;

  return energy ;
}

void Mass::step(double dt)
{

position = position + velocity*dt ;

velocity = velocity + force/mass*dt ;

}

/* ---------------------------------------------------------------- */
// class Spring
/* ---------------------------------------------------------------- */

Spring::Spring(Mass * mass1, Mass * mass2, double naturalLength, double stiffness, double damping)
: mass1(mass1), mass2(mass2),
naturalLength(naturalLength), stiffness(stiffness), damping(damping)
{ }

Mass * Spring::getMass1() const
{
  return mass1 ;
}

Mass * Spring::getMass2() const
{
  return mass2 ;
}

Vector3 Spring::getForce() const
{
  double ell = getLength() ;

  Vector3 u_12 = 1/ell*(mass2->getPosition() - mass1->getPosition()) ;
  Vector3 v_12 = u_12 * dot((mass2->getVelocity() - mass1->getVelocity()),u_12);
  
  Vector3 F_k = u_12 * stiffness * (ell - naturalLength) ;

  Vector3 F_d = v_12 * damping ;

  return (F_k + F_d) ;
}

double Spring::getLength() const
{
  Vector3 u = mass2->getPosition() - mass1->getPosition() ;
  return u.norm() ;
}

double Spring::getEnergy() const {
  double length = getLength() ;
  double dl = length - naturalLength;
  return 0.5 * stiffness * dl * dl ;
}


std::ostream& operator << (std::ostream& os, const Mass& m)
{
  os<<"("
  <<m.getPosition().x<<","
  <<m.getPosition().y<<")" ;
  return os ;
}

std::ostream& operator << (std::ostream& os, const Spring& s)
{
  return os<<"$"<<s.getLength() ;
}

/* ---------------------------------------------------------------- */
// class SpringMass : public Simulation
/* ---------------------------------------------------------------- */

SpringMass::SpringMass(Mass **masses, Spring **springs,int m, int n, double gravity):
springs(springs), masses(masses), m(m), n(n)
{}

void SpringMass::display()
{

for (int i(0); i < n; ++i) {

  std::cout << **(masses + i) << " " ;

} ;

std::cout << std::endl ;


}

double SpringMass::getEnergy() const
{
  double energy = 0 ;

// Left to implement

  return energy ;
}

void SpringMass::step(double dt)
{
  Vector3 g(0.0,-gravity) ;

  for (int i(0); i < n; ++i) {

    Mass *mass1 = (*(springs + i)) -> getMass1() ;
    Mass *mass2 = (*(springs + i)) -> getMass2() ;

    mass1 -> setForce(g) ;
    mass2 -> setForce(g) ;

    Vector3 F_plus = (*(springs+i))->getForce() ;

    mass1 -> addForce(F_plus)  ;
    mass2 -> addForce(F_plus*-1)  ;

    mass1->step(dt) ;
    mass2->step(dt) ;

  }

}



