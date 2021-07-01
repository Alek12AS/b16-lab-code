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
    static void specialKeys(int key, int x, int y);
    static void keyboard(unsigned char key, int x, int y);

protected:
    virtual void rightOrLeftKey(bool b);
    virtual void upOrDownKey(bool b);
    virtual void qOrEKey(bool q);
};

#endif