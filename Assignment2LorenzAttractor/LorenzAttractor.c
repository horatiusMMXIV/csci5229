#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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
 * This function is called by GLUT to display the scene
 */
void display()
{
	// Clear the window
	glClear(GL_COLOR_BUFFER_BIT);

	/*  Lorenz Parameters  */
	double s  = 10;
	double b  = 2.6666;
	double r  = 28;

	int i;
	/*  Coordinates  */
	double x = 1;
	double y = 1;
	double z = 1;
	/*  Time step  */
	double dt = 0.001;

	// Draw the points for the lorenz attractor
	glBegin(GL_POINTS);

	/*
	*  Integrate 50,000 steps (50 time units with dt = 0.001)
	*  Explicit Euler integration
	*/
	for (i=0;i<50000;i++)
	{
	double dx = s*(y-x);
	double dy = x*(r-z)-y;
	double dz = x*y - b*z;
	x += dt*dx;
	y += dt*dy;
	z += dt*dz;

	printf("%5d %8.3f %8.3f %8.3f\n",i+1,x,y,z);
	// Draw a vertex with the new lorenz points
	glVertex3f(x, y, z);

	}

	glEnd();

	// Render the scene
	glFlush();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char key, int x, int y){
	if(key == 27){
		exit(0);
	}
}

/*
 *  Function to print any errors encountered
 */
void ErrCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 * Create the initial states
 */
void init(){

	// Set the color of the points to white
	glColor3f(1.0, 1.0, 1.0);

}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   // Initialize GLUT
   glutInit(&argc,argv);
   // Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   // Create window
   glutCreateWindow("Lorenz Attractor");
   // Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   // Register function used to display scene
   glutDisplayFunc(display);
   // Set the initial states like drawing color
   init();
   // Pass control to GLUT for events
   glutMainLoop();
   return 0;
}
