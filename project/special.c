#include "CSCIx229.h"

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
	/*
	//  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_RIGHT){
		yaw += 5;
	}
	//  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_LEFT){
		yaw -= 5;
	}
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP){
		pitch += 5;
	}
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN){
		pitch -= 5;
	}
	//  PageUp key - increase dim
	else if (key == GLUT_KEY_PAGE_UP){
		dim += 0.1;
	}
	//  PageDown key - decrease dim
	else if (key == GLUT_KEY_PAGE_DOWN && dim>1){
		dim -= 0.1;
	}
	else if(key == GLUT_KEY_F3){
		roll += 5;
	}
	else if(key==GLUT_KEY_F4){
		roll -= 5;
	}
	//  Keep angles to +/-360 degrees
	//  Keep angles to +/-360 degrees
	yaw %= 360;
	pitch %= 360;
	roll %= 360;
	*/
	/*
	double radius = 10;
	cameraPos[0] = radius*Sin(yaw)*Cos(pitch);
	cameraPos[1] = -radius*Sin(pitch);
	cameraPos[2] = radius*Cos(yaw)*Cos(pitch);
	*/

	// Reproject
	Project(fov,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}
