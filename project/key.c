#include "CSCIx229.h"


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
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
	   //cameraPos[0] += cameraFront[0]*.1;
	   //cameraPos[1] += cameraFront[1]*.1;
	   //cameraPos[2] += cameraFront[2]*.1;
	   cameraPos[0] += cameraUp[0]*.1;
	   cameraPos[1] += cameraUp[1]*.1;
	   cameraPos[2] += cameraUp[2]*.1;
   }
   else if(ch=='s'){
	   //cameraPos[0] -= cameraFront[0]*.1;
	   //cameraPos[1] -= cameraFront[1]*.1;
	   //cameraPos[2] -= cameraFront[2]*.1;
	   cameraPos[0] -= cameraUp[0]*.1;
	   cameraPos[1] -= cameraUp[1]*.1;
	   cameraPos[2] -= cameraUp[2]*.1;
   }
   else if(ch=='a'){
	   cameraPos[0] -= cameraRight[0]*.1;
	   cameraPos[1] -= cameraRight[1]*.1;
	   cameraPos[2] -= cameraRight[2]*.1;
   }
   else if(ch=='d'){
   	   cameraPos[0] += cameraRight[0]*.1;
   	   cameraPos[1] += cameraRight[1]*.1;
   	   cameraPos[2] += cameraRight[2]*.1;
      }
   // Spacebar makes the helicopter move in the direction of the up vector
	else if(ch == 32){
		// Move in direction of up vector
	}
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
