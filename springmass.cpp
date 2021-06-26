
#include "springmass.h"
#include <cmath>
#include <iostream>

/* ---------------------------------------------------------------- */
// class Mass
/* ---------------------------------------------------------------- */

Mass::Mass()
: position(), velocity(), force(), mass(1), radius(1)
{ }

Mass::Mass(Vector2 position, Vector2 velocity, double mass, double radius)
: position(position), velocity(velocity), force(), mass(mass), radius(radius),
xmin(-1),xmax(1),ymin(-1),ymax(1)
{ }

void Mass::setForce(Vector2 f)
{
  force = f ;
}

void Mass::addForce(Vector2 f)
{
  force = force + f ;
}

Vector2 Mass::getForce() const
{
  return force ;
}

Vector2 Mass::getPosition() const
{
  return position ;
}

Vector2 Mass::getVelocity() const
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

Vector2 Spring::getForce() const
{
  double ell = getLength() ;

  Vector2 u_12 = 1/ell*(mass2->getPosition() - mass1->getPosition()) ;
  Vector2 v_12 = u_12 * dot((mass2->getVelocity() - mass1->getVelocity()),u_12);
  
  Vector2 F_k = u_12 * stiffness * (ell - naturalLength) ;

  Vector2 F_d = v_12 * damping ;

  return (F_k + F_d) ;
}

double Spring::getLength() const
{
  Vector2 u = mass2->getPosition() - mass1->getPosition() ;
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

SpringMass::SpringMass(Spring * spring1, Mass * mass1, Mass * mass2,double gravity)
: gravity(gravity), spring1(spring1), mass1(mass1), mass2(mass2)
{ }

void SpringMass::display()
{

/*
std::cout << *mass1 << " " << *mass2 << std::endl ;
*/



}

double SpringMass::getEnergy() const
{
  double energy = 0 ;

// Left to implement

  return energy ;
}

void SpringMass::step(double dt)
{
  Vector2 g(0,-gravity) ;

  mass1->setForce(g) ;
  mass2->setForce(g) ;

  Vector2 F_plus = spring1->getForce() ;

  mass1->addForce(F_plus) ;
  mass2->addForce(F_plus*-1) ;

  mass1->step(dt) ;
  mass2->step(dt) ;

}



