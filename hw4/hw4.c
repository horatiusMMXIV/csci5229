/*
 * Robert Werthman
 * CSCI 5229
 * HW 4
 *
 * hw4.c
 *
 * Requirements:
 * 1) GL_CULL_FACE
 * 2) Zoom capabilities
 * 3) 3 modes:
 * 		i) Orthogonal
 * 		ii) Perspective
 * 		iii) First Person Navigation with cursor keys
 * 4) View scene from multiple eye positions
 * 5) View scene from multiple view points using cursor keys
 *
 */


#define GL_GLEXT_PROTOTYPES

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
double zh=0;	// Angle incrementer

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Check for OpenGL errors
 */
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Draw vertex in polar coordinates
 */
void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

/*
*  Draw a sphere
*	at (x,y,z)
* 	radius (r)
*/
void sphere(double x, double y, double z, double r)
{
	const int d=5;
	int th,ph;
	//  Save transformation
	glPushMatrix();
	//  Offset and scale
	glTranslatef(x, y, z);
	glScalef(r, r, r);

	//  South pole cap
	glBegin(GL_TRIANGLE_FAN);
	Vertex(0,-90);
	for (th=0;th<=360;th+=d)
	{
	  Vertex(th,d-90);
	}
	glEnd();

	//  Latitude bands
	for (ph=d-90;ph<=90-2*d;ph+=d)
	{
	  glBegin(GL_QUAD_STRIP);
	  for (th=0;th<=360;th+=d)
	  {
		 Vertex(th,ph);
		 Vertex(th,ph+d);
	  }
	  glEnd();
	}

	//  North pole cap
	glBegin(GL_TRIANGLE_FAN);
	Vertex(0,90);
	for (th=0;th<=360;th+=d)
	{
	  Vertex(th,90-d);
	}
	glEnd();
	//  Undo transformations
	glPopMatrix();
}

/*
 * Draw a triangle
 * 	at (x,y,z)
 * 	with height (h), length (l), width (w)
 * 	at angle (angle) on x (ax), y (ay), or z (az)
 */
void triangle(double x, double y, double z, double l, double h, double w, double angle, double ax, double ay, double az)
{
	//  Save transformation
	glPushMatrix();
	// Offset, scale, and rotate
	glTranslatef(x, y, z);
	glScalef(l, h, w);
	glRotatef(angle, ax, ay, az);
	// Draw a triangle
	glBegin(GL_TRIANGLES);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	//  Undo transofrmations
	glPopMatrix();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     with height (h), length (l), width (w)
 *     at angle (angle) on x (ax), y (ay), or z (az)
 *
 *
 */
void cube(double x, double y, double z, double l, double h, double w, double angle, double ax, double ay, double az)
{
   //  Save transformation
   glPushMatrix();
   // Offset, scale
   glTranslatef(x, y, z);
   glRotatef(angle, ax, ay, az);
   glScalef(l, h, w);
   //  Cube
   glBegin(GL_QUADS);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

/*
 * Draw entire helicopter
 *	with blade rotation (br)
 */
void helicopter(double br){
	// Helicopter body
	// Blue
	glColor3f(0, 0, 1);
	cube(0, 0, 0, .5, .3, .3, 0, 0, 0, 0);

	// Tail Bloom
	// Green
	glColor3f(0, 1, 0);
	cube(.8, .1, 0, .4, .1, .1, 0, 0, 0, 0);

	// Tail Fin
	// Red
	glColor3f(1, 0, 0);
	triangle(1.1, .1, 0, .6, .6, .6, -45, 0, 0, 1);

	// Tail Gearbox
	// Blue
	glColor3f(0, 0, 1);
	cube(1.3, .1, 0, .1, .1, .1, 0, 0, 0, 0);

	//Tail Rotor Blades
	// White
	glColor3f(1, 1, 1);
	cube(1.3, .1, .18, .3, .06, .08, br + 45, 0, 0, 1);

	// Rotor mast
	// Red
	glColor3f(1, 0, 0);
	cube(.2, .45, 0, .1, .2, .1, 0, 0, 0, 0);

	// Main rotor blades
	// White
	glColor3f(1, 1, 1);
	cube(.2, .7, 0, .9, .1, .1, br + 0, 0, 1, 0);
	cube(.2, .7, 0, .9, .1, .1, br + 90, 0, 1, 0);

	// Skids
	// Yellow
	glColor3f(1, 1, 0);

	// Left skid mounts
	cube(-.2, -.3, .2, .3, .06, .06, 45, 0, 0, 1);
	cube(.2, -.3, .2, .3, .06, .06, 45, 0, 0, 1);

	// Left skid
	cube(-.1, -.5, .2, .7, .06, .06, 0, 0, 0, 0);

	// Right skid mounts
	cube(-.2, -.3, -.2, .3, .06, .06, 45, 0, 0, 1);
	cube(.2, -.3, -.2, .3, .06, .06, 45, 0, 0, 1);

	// Right skid
	cube(-.1, -.5, -.2, .7, .06, .06, 0, 0, 0, 0);

	// Cockpit
	sphere(-.4, .07, 0, .45);
}

/*
 * Create a moving helicopter
 *	at (x,y,z)
 *  nose towards (dx,dy,dz)
 *  up towards (ux,uy,uz)
 */
void movingHelicopter(){
	//  Save current transforms
	glPushMatrix();

	//  Translate the helicopter around the y-axis
	glTranslated(Cos(zh), 1, -Sin(zh));
	// Point the nose of the helicopter in the direction of travel
	glRotated(-90 + zh, 0, 1, 0);
	// Pitch the helicopter forward
	glRotated(45, 0, 0, 1);
	// Roll the helicopter towards the origin
	glRotated(25, 1, 0, 0);

	// Create the helicopter with rotating blades
	helicopter(5*zh);

	//  Undo transformations
	glPopMatrix();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	const double len=2.0;  //  Length of axes
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	//  Undo previous transformations
	glLoadIdentity();
	// Set view angle
	glRotated(ph,1,0,0);
	glRotated(th,0,1,0);

	glPushMatrix();
	movingHelicopter();
	glPopMatrix();

	// Draw another helicopter but use a different matrix to draw it on and apply the transformations to
	// so that it doesn't affect the how the axes are drawn later one
	glPushMatrix();
	glTranslatef(2, 2, -2);
	glRotatef(90, 0, 1, 0);
	glRotatef(45, 0, 0, 1);
	glScalef(.5, .5, .5);
	helicopter(0);
	glPopMatrix();

	// Draw another helicopter but use a different matrix to draw it on and apply the transformations to
	// so that it doesn't affect the how the axes are drawn later one
	glPushMatrix();
	glTranslatef(-2, 2, 2);
	glRotatef(-45, 1, 0, 1);
	glScalef(.3, .3, .3);
	helicopter(0);
	glPopMatrix();

	glColor3f(1,1,1);
	if(axes){
		//  Draw axes
		glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(len,0.0,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,len,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,0.0,len);
		glEnd();
		//  Label axes
		glRasterPos3d(len,0.0,0.0);
		Print("X");
		glRasterPos3d(0.0,len,0.0);
		Print("Y");
		glRasterPos3d(0.0,0.0,len);
		Print("Z");
	}
	//  Display parameters
	glWindowPos2i(5,5);
	Print("Angle=%d,%d", th, ph);
	// Check for any errors that have occurred
	ErrCheck("display");
	//  Render the scene and make it visible
	glFlush();
	glutSwapBuffers();

}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	//  Exit on ESC
	if (ch == 27){
	  exit(0);
	}
	// Reset everything
	else if (ch == '0'){
		axes = 1;
		th = ph = 0;
	}
	//  Toggle axes
	else if (ch == 'a'){
	      axes = 1-axes;
	}
	//  Reset view angle
	// Look down x-axis
	else if (ch == 'x')
	{
		th = -90;
		ph = 0;
	}
	// Look down y-axis
	else if (ch == 'y'){
		th = 0;
		ph = 90;
	}
	// Look down z-axis
	else if (ch == 'z')
	{
		th = ph = 0;

	}
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();
	glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph -= 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph += 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600,600);
	glutCreateWindow("Robert Werthman Assignment 3");
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	//  Tell GLUT to call "idle" when there is nothing else to do
	glutIdleFunc(idle);
	//  Check if any errors have occurred
	ErrCheck("init");
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	return 0;
}
