#ifndef __Figure__
#define __Figure__

#include "GL/glut.h"
#include "springmass.h"

class Figure
{
public:
    virtual void initGL() = 0;
    virtual void draw() = 0;
    virtual void updateAnimation() = 0;
    virtual void updateClip(GLfloat width, GLfloat height) = 0;
    virtual GLuint getDt() = 0;
    static void display();
    static void Timer(int value);
    static void reshape(GLsizei width, GLsizei height);

protected:

};

#endif