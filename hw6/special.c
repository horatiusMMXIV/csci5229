#include "CSCIx229.h"

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
	//  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_RIGHT){
		th -= 5;
	}
	//  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_LEFT){
		th += 5;
	}
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP){
		ph -= 5;
	}
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN){
		ph += 5;
	}
	//  PageUp key - increase dim
	else if (key == GLUT_KEY_PAGE_UP){
		dim += 0.1;
	}
	//  PageDown key - decrease dim
	else if (key == GLUT_KEY_PAGE_DOWN && dim>1){
		dim -= 0.1;
	}
	else if (key == GLUT_KEY_F3){
		distance = (distance==1) ? 5 : 1;
	}
	//  Keep angles to +/-360 degrees
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	// Reproject
	Project(fov, asp, dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}
