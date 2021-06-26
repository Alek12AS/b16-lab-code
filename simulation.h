#ifndef __simulation__
#define __simulation__

class Simulation
/*
 * Interface for a simulation
*/ 

{
public:
  virtual void step(double dt) = 0 ;
  virtual void display() = 0 ;
} ;

#endif /* defined(__simulation__) */
