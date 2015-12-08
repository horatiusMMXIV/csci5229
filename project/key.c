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
		e=explosion= 0;
		yaw=pitch=roll=strafe=fly=bankFactor=bankAngle=speed=0;
		littleBirdPosition[0]=littleBirdPosition[2]=0;
		littleBirdPosition[1]=2;
		fov=55;
		if(mode==5){
			// resume music playback
			Mix_ResumeMusic();
		}
	}
	//  Change field of view angle
	else if (ch == '-' && ch>1){
		fov--;
	}
	else if (ch == '+' && ch<179){
		fov++;
	}
	else if (ch=='g'){
		// Turn the helicopter right
		// This can only occur when it isn't moving
		if(speed == 0){
			yaw += -5;
			HelicopterYaw();
		}
	}
	else if (ch=='a'){
		// Turn the helicopter left
		// This can only occur when it isn't moving
		if(speed == 0){
			yaw += 5;
			HelicopterYaw();
		}
	}
	else if (ch=='e'){
		// Pitch the helicopter nose down
		// Increase the forward speed
		if((speed-1)==0 && roll !=0){
		// Don't do anything because it makes the helicopter stick
		}else{
			pitch += -5;
			speed += 1;
		}
	}
	else if (ch=='d'){
		// Pitch the helicopter nose up
		// Increase the reverse speed
		if((speed-1)==0 && roll !=0){
			// Don't do anything because it makes the helicopter stick
		}else{
			pitch += 5;
			speed -= 1;
		}
	}
	else if(ch=='s'){
		// Roll the helicopter left
		// Only allow roll if the helicopter is moving and there is no strafe
		if(speed != 0 && strafe ==0){
			roll += 5;
			bankFactor += 1;
			HelicopterRoll();
		}
	}
	else if(ch=='f'){
		// Roll the helicopter right
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
	else if(ch == 32){
		if(mode==5){
			Mix_PlayChannel(-1,shot,0);
		}
		if(bulletIndex < 9){
			CreateBullet();
			bulletIndex += 1;
		}else{
			bulletIndex = 0;
			CreateBullet();
		}
	}
	else if(ch =='m'){
	//  Toggle display modes
      mode = (mode+1)%MODE; 
	}
	//  Reproject
	Project(fov,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}
