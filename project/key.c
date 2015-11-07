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
		if(speed == 0){
			yaw += -5;
			HelicopterYaw();
		}
	}
	else if (ch=='a'){
		if(speed == 0){
			yaw += 5;
			HelicopterYaw();
		}
	}
	else if (ch=='e'){
		pitch += -5;
		speed += 1;
	}
	else if (ch=='d'){
		pitch += 5;
		speed -= 1;
	}
	else if(ch=='s' && strafe == 0){
		if(speed != 0){
			roll += 5;
			HelicopterRoll();
		}
	}
	else if(ch=='f'){
		if(speed != 0 && strafe == 0){
			roll += -5;
			HelicopterRoll();
		}
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
		if(speed == 0 && roll == 0){
			strafe -= 1;
			HelicopterStrafe();
		}
	}
	else if(ch=='l'){
	// Strafe right
		if(speed == 0 && roll == 0){
			strafe += 1;
			HelicopterStrafe();
		}
	}
	//  Reproject
	Project(fov,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}
