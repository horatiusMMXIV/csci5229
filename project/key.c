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
	else if(ch=='s'){
		// Only allow roll if the helicopter is moving and there is no strafe
		if(speed != 0 && strafe ==0){
			roll += 5;
			bankFactor += 1;
			HelicopterRoll();
		}
	}
	else if(ch=='f'){
		// Only allow roll if the helicopter is moving and there is no strafe
		if(speed != 0 && strafe == 0){
			roll += -5;
			bankFactor -= 1;
			HelicopterRoll();
		}
	}
	else if(ch =='i'){
	// Elevate the helicopter up
		fly += 5;
		HelicopterFly();
	}
	else if(ch=='k'){
	// Elevate the helicopter down
		fly -= 5;
		HelicopterFly();
	}
	else if(ch=='j'){
	// Strafe left
		if(roll == 0){
			strafe -= 5;
			HelicopterStrafe();
		}
	}
	else if(ch=='l'){
	// Strafe right
		if(roll == 0){
			strafe += 5;
			HelicopterStrafe();
		}
	}
	//  Reproject
	Project(fov,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}
