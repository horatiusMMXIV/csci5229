/*
 *  Set projection
 */
#include "CSCIx229.h"

void Project(double fov,double asp,double dim)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   // 2 for znear clipping plane so it is only 2 units in front
   // 2*dim for the zfar clipping plane so it is 2*dim in front
   gluPerspective(fov,asp,2,3*dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

