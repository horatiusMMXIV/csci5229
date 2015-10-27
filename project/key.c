#include "CSCIx229.h"


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	const int distance = 1;
	//  Exit on ESC
	if (ch == 27)
	exit(0);
	//  Reset view angle
	else if (ch == '0')
	yaw = pitch = roll= 0;
	//  Move light
	else if (ch == '<')
	zh += 1;
	else if (ch == '>')
	zh -= 1;
	//  Change field of view angle
	else if (ch == '-' && ch>1)
	fov--;
	else if (ch == '+' && ch<179)
	fov++;
	//  Light elevation
	else if (ch=='[')
	ylight -= 0.1;
	else if (ch==']')
	ylight += 0.1;
	else if (ch=='g'){
	//yaw -= 5;
	HelicopterYaw(-5);
	}
	else if (ch=='a'){
	//yaw += 5;
	HelicopterYaw(5);
	}
	else if (ch=='e'){
	//pitch += 5;
	HelicopterPitch(5);
	}
	else if (ch=='d'){
	//pitch -= 5;
	HelicopterPitch(-5);
	}
	else if(ch=='s'){
	//roll += 5;
	HelicopterRoll(5);
	}
	else if(ch=='f'){
	//roll -= 5;
	HelicopterRoll(-5);
	}
	else if(ch =='i'){
	// Fly up
	HelicopterFly(distance);
	}
	else if(ch=='k'){
	// Fly down
	HelicopterFly(-distance);
	}
	else if(ch=='j'){
	// Strafe left
	HelicopterStrafe(-distance);
	}
	else if(ch=='l'){
	// Strafe right
	HelicopterStrafe(distance);
	}
	//  Reproject
	Project(fov,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}
