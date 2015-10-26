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
	   yaw = pitch = 0;
   //  Toggle axes
   else if (ch == 'x')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l')
      light = 1-light;
   //  Toggle light movement
   else if (ch == 'm')
      move = 1-move;
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
   else if (ch=='f'){
	   fly = 1-fly;
   }
   else if(ch =='w'){
	// Fly up
	cameraPos[0] += cameraUp[0]*speed;
	cameraPos[1] += cameraUp[1]*speed;
	cameraPos[2] += cameraUp[2]*speed;
	littleBirdPosition[0] += cameraUp[0]*speed;
	littleBirdPosition[1] += cameraUp[1]*speed;
	littleBirdPosition[2] += cameraUp[2]*speed;
   }
   else if(ch=='s'){
	// Fly down
	cameraPos[0] -= cameraUp[0]*speed;
	cameraPos[1] -= cameraUp[1]*speed;
	cameraPos[2] -= cameraUp[2]*speed;
	littleBirdPosition[0] -= cameraUp[0]*speed;
	littleBirdPosition[1] -= cameraUp[1]*speed;
	littleBirdPosition[2] -= cameraUp[2]*speed;
   }
   else if(ch=='a'){
	// Strafe left
	cameraPos[0] -= cameraRight[0]*speed;
	cameraPos[1] -= cameraRight[1]*speed;
	cameraPos[2] -= cameraRight[2]*speed;
	littleBirdPosition[0] -= cameraRight[0]*speed;
	littleBirdPosition[1] -= cameraRight[1]*speed;
	littleBirdPosition[2] -= cameraRight[2]*speed;
   }
   else if(ch=='d'){
	// Strafe right
	cameraPos[0] += cameraRight[0]*speed;
	cameraPos[1] += cameraRight[1]*speed;
	cameraPos[2] += cameraRight[2]*speed;
	littleBirdPosition[0] += cameraRight[0]*speed;
	littleBirdPosition[1] += cameraRight[1]*speed;
	littleBirdPosition[2] += cameraRight[2]*speed;

   }
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
