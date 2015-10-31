#include "CSCIx229.h"


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	const double distance = 1;
	//  Exit on ESC
	if (ch == 27){
		exit(0);
	}
	//  Reset view angle
	else if (ch == '0'){
		yaw=pitch=roll=strafe=0;
		littleBirdPosition[0]=littleBirdPosition[1]=littleBirdPosition[2]=0;
	}
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
	HelicopterYaw(-5);
	}
	else if (ch=='a'){
	HelicopterYaw(5);
	}
	else if (ch=='e'){
	HelicopterPitch(-5);
	}
	else if (ch=='d'){
	HelicopterPitch(5);
	}
	else if(ch=='s'){
	HelicopterRoll(5);
	}
	else if(ch=='f'){
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
