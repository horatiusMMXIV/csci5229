#include "CSCIx229.h"


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	const double speed = .5;
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
   		yaw += 5;
   	}
   	else if (ch=='a'){
   		yaw -= 5;
   	}
   	else if (ch=='e'){
   		pitch += 5;
   	}
   	else if (ch=='d'){
   		pitch -= 5;
   	}
	else if(ch=='s'){
		roll += 5;
	}
	else if(ch=='f'){
		roll -= 5;
	}
   else if(ch =='i'){
	// Fly up
	cameraPos[0] += cameraUp[0]*speed;
	cameraPos[1] += cameraUp[1]*speed;
	cameraPos[2] += cameraUp[2]*speed;
	littleBirdPosition[0] += cameraUp[0]*speed;
	littleBirdPosition[1] += cameraUp[1]*speed;
	littleBirdPosition[2] += cameraUp[2]*speed;
   }
   else if(ch=='k'){
	// Fly down
	cameraPos[0] -= cameraUp[0]*speed;
	cameraPos[1] -= cameraUp[1]*speed;
	cameraPos[2] -= cameraUp[2]*speed;
	littleBirdPosition[0] -= cameraUp[0]*speed;
	littleBirdPosition[1] -= cameraUp[1]*speed;
	littleBirdPosition[2] -= cameraUp[2]*speed;
   }
   else if(ch=='j'){
	// Strafe left
	cameraPos[0] -= cameraRight[0]*speed;
	cameraPos[1] -= cameraRight[1]*speed;
	cameraPos[2] -= cameraRight[2]*speed;
	littleBirdPosition[0] -= cameraRight[0]*speed;
	littleBirdPosition[1] -= cameraRight[1]*speed;
	littleBirdPosition[2] -= cameraRight[2]*speed;
   }
   else if(ch=='l'){
	// Strafe right
	cameraPos[0] += cameraRight[0]*speed;
	cameraPos[1] += cameraRight[1]*speed;
	cameraPos[2] += cameraRight[2]*speed;
	littleBirdPosition[0] += cameraRight[0]*speed;
	littleBirdPosition[1] += cameraRight[1]*speed;
	littleBirdPosition[2] += cameraRight[2]*speed;
   }
   yaw %= 360;
   pitch %= 360;
   roll %= 360;
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
