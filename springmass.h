#ifndef __springmass__
#define __springmass__

#include "simulation.h"

#include <cmath>
#include <vector>

#define MOON_GRAVITY 1.62
#define EARTH_GRAVITY 9.82

/* ---------------------------------------------------------------- */
// class Vector3
/* ---------------------------------------------------------------- */

class Vector3
{
public:
  double x ;
  double y ;
  double z ;

  Vector3() : x(0), y(0), z(0) { }
  Vector3(double _x, double _y, double _z = 0) : x(_x), y(_y), z(_z) { }
  double norm2() const { return x*x + y*y + z*z ; }
  double norm() const { return std::sqrt(norm2()) ; }
} ;

inline Vector3 operator+ (Vector3 a, Vector3 b) { return Vector3(a.x+b.x, a.y+b.y, a.z+b.z); }
inline Vector3 operator- (Vector3 a, Vector3 b) { return Vector3(a.x-b.x, a.y-b.y, a.z-b.z) ; }
inline Vector3 operator* (double a, Vector3 b)  { return Vector3(a*b.x, a*b.y, a*b.z) ; }
inline Vector3 operator* (Vector3 a, double b)  { return Vector3(a.x*b, a.y*b, a.z*b) ; }
inline Vector3 operator/ (Vector3 a, double b)  { return Vector3(a.x/b, a.y/b, a.z/b) ; }
inline double dot(Vector3 a, Vector3 b) { return a.x*b.x + a.y*b.y + a.z*b.z ; }

/* ---------------------------------------------------------------- */
// class Mass
/* ---------------------------------------------------------------- */

class Mass

{

/*
 * Constructor Parameters:
 * Position = position of the mass in space
 * Velocity = velocity of the mass
 * mass     = physical mass of the object
 * radius   = radius of the mass
 * 
 * Overview:
 * Class represents a circular/spherical mass with attributes relevant to its dynamics
 */

public:
  Mass() ;
  Mass(Vector3 position, Vector3 velocity, double mass, double radius) ;
  void setForce(Vector3 f) ;
  void addForce(Vector3 f) ;
  Vector3 getForce() const ;
  Vector3 getPosition() const ;
  Vector3 getVelocity() const ;
  double getMass() const ;
  double getRadius() const ;
  double getEnergy(double gravity) const ;
  void step(double dt) ;

protected:
  Vector3 position ;
  Vector3 velocity ;
  Vector3 force ;
  double mass ;
  double radius ;

  double xmin ;
  double xmax ;
  double ymin ;
  double ymax ;
} ;

/* ---------------------------------------------------------------- */
// class Spring
/* ---------------------------------------------------------------- */

class Spring
{

/*
 * Constructor Parameters:
 * Mass1 = First mass connected to the spring
 * Mass2 = Second mass connected
 * naturalLength = natural length of spring
 * stiffness = Hooke's consant or elastic modulus
 * damping = Damping factor
 * 
 * Overview: Spring class allowing for calculation of the forces
 */

public:
  Spring(Mass * mass1, Mass * mass2, double naturalLength, double stiff, double damping = 0) ;
  Mass * getMass1() const ;
  Mass * getMass2() const ;
  Vector3 getForce() const ;
  double getLength() const ;
  double getEnergy() const ;

protected:
  double stiffness ;
  double damping ;
  double naturalLength ;
  Mass * mass1 ;
  Mass * mass2 ;

} ;

/* ---------------------------------------------------------------- */
// class SpringMass : public Simulation
/* ---------------------------------------------------------------- */


class SpringMass : public Simulation

{

/*
 * Constructor Parameters:
 * Spring1  = the spring connecting the two masses
 * Mass1    = first mass
 * Mass2    = second mass
 * gravity  = gravitational acceleration, set as earth graphity by default
 * 
 * Overview:
 * Reprents the universe comprising of two masses and one spring
*/
public:
  SpringMass(Mass **masses, Spring **springs,int m ,int n , double gravity = MOON_GRAVITY) ;
  void step(double dt) ;
  void display() ;
  double getEnergy() const ;

protected:
  Spring **springs ;
  Mass **masses ; 
  int m ;
  int n ;
  double gravity ; 


} ;

#endif /* defined(__springmass__) */

