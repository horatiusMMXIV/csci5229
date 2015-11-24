#include "CSCIx229.h"


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	//  Exit on ESC
	if (ch == 27){
		exit(0);
	}
	//  Reset view angle
	else if (ch == '0'){
		e = 0;
		yaw=pitch=roll=strafe=fly=bankFactor=bankAngle=speed=0;
		littleBirdPosition[0]=littleBirdPosition[1]=littleBirdPosition[2]=0;
		ph=th=0;
	}
	//  Change field of view angle
	else if (ch == '-' && ch>1){
		fov--;
	}
	else if (ch == '+' && ch<179){
		fov++;
	}
	else if (ch=='g'){
		if(flight){
			// Turn the helicopter right
			// This can only occur when it isn't moving
			if(speed == 0){
				yaw += -5;
				HelicopterYaw();
			}
		}
	}
	else if (ch=='a'){
		if(flight){
			// Turn the helicopter left
			// This can only occur when it isn't moving
			if(speed == 0){
				yaw += 5;
				HelicopterYaw();
			}
		}
	}
	else if (ch=='e'){
		if(flight){
			// Pitch the helicopter nose down
			// Increase the forward speed
			if((speed-1)==0 && roll !=0){
			// Don't do anything because it makes the helicopter stick
			}else{
				pitch += -5;
				speed += 1;
			}
		}
	}
	else if (ch=='d'){
		if(flight){
			// Pitch the helicopter nose up
			// Increase the reverse speed
			if((speed-1)==0 && roll !=0){
				// Don't do anything because it makes the helicopter stick
			}else{
				pitch += 5;
				speed -= 1;
			}
		}
	}
	else if(ch=='s'){
		if(flight == 1){
			// Roll the helicopter left
			// Only allow roll if the helicopter is moving and there is no strafe
			if(speed != 0 && strafe ==0){
				roll += 5;
				bankFactor += 1;
				HelicopterRoll();
			}
		}
	}
	else if(ch=='f'){
		if(flight == 1){
			// Roll the helicopter right
			// Only allow roll if the helicopter is moving and there is no strafe
			if(speed != 0 && strafe == 0){
				roll += -5;
				bankFactor -= 1;
				HelicopterRoll();
			}
		}
	}
	else if(ch =='i'){
		if(flight){
			// Elevate the helicopter up
			fly += 5;
			HelicopterFly();
		}
	}
	else if(ch=='k'){
		if(flight){
			// Elevate the helicopter down
			fly -= 5;
			HelicopterFly();
		}
	}
	else if(ch=='j'){
		if(flight){
			// Strafe left
			if(roll == 0){
				strafe -= 5;
				HelicopterStrafe();
			}
		}
	}
	else if(ch=='l'){
		if(flight){
			// Strafe right
			if(roll == 0){
				strafe += 5;
				HelicopterStrafe();
			}
		}
	}
	else if(ch == 32){
		flight = 1-flight;
	}
	//  Reproject
	Project(fov,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}
