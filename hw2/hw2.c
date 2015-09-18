#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*	Most of the work is not my own.  Most of it comes from the Professor's code provided in the examples.
 *
 * Requirements:
 * 1) Display the lorenz attractor on screen with vertexes as GL_LINE_STRIP *Completed*
 * 2) Display the lorenz attractor on screen with different colors *Completed*
 * 3) Put x, y, z axis into graphic *Completed*
 * 4) The lorenz attractor should not be distorted when resizing the window *Completed*
 * 5) Add ability to rotate object *Completed*
 * 6) Allow one or more parameters from the lorenz equations to be modified *Completed*
 * 7) Provide a line trace of the sequence of the attractor
 */

// Global variables
int th = 0;	// Azimuth of view angle
int ph = 0;	// Elevation of view angle
int mode = 0; // Projection mode
double dim = 60; // Dimension of the viewing volume or what can be seen on the screen
int n = 50000; // Number of data points
int l = 0; // Movement
int axes = 0; // Display axes
/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;
float vertices[50000][3]; // Array that stores x, y, z values from the lorenz equation
int vertexColors[50000][3]; // Array that stores the colors for the x, y, z values from the lorenz equation
char* text[] = {"Trace Mode","Draw Mode"};


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
 * Function that draws the x, y, z axes
 */
void drawAxes(){
	double len = 50;  //  Length of axes
	// Set the color to white
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	// Draw the positive x axis
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(len, 0.0, 0.0);
	// Draw the positive y axis
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, len, 0.0);
	// Draw the positive z axis
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, len);
	glEnd();
	//  Label axes
	glRasterPos3d(len, 0, 0);
	Print("X");
	glRasterPos3d(0, len, 0);
	Print("Y");
	glRasterPos3d(0, 0, len);
	Print("Z");
}

/*
 * Function that creates the points for the lorenz attractor
 */
void createLorenzPoints(){
		int i;
		/*  Coordinates  */
		double x = 1;
		double y = 1;
		double z = 1;
		/*  Time step  */
		double dt = 0.001;
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
			// Add the x, y, z values to an array
			vertices[i][0] = x;
			vertices[i][1] = y;
			vertices[i][2] = z;
			//Set the color of the vertices to either red, green, or blue
			if(i < 16666){
				// Red
				vertexColors[i][0] = 1;
				vertexColors[i][1] = 0;
				vertexColors[i][2] = 0;
			}else if(i >= 16666 && i < 33333){
				// Green
				vertexColors[i][0] = 0;
				vertexColors[i][1] = 1;
				vertexColors[i][2] = 0;
			}else{
				// Blue
				vertexColors[i][0] = 0;
				vertexColors[i][1] = 0;
				vertexColors[i][2] = 1;
			}
		}
}

/*
 * Function that traces the lorenz attractor
 */
void traceLorenz(){
	int i;
	// Draw from vertex to vertex of the lorenz attractor
	glBegin(GL_LINE_STRIP);
	// Draw the previous vertices
	for(i = 0;i < l;i++){
		glColor3d(vertexColors[i][0], vertexColors[i][1], vertexColors[i][2]);
		glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
	}
	// Connect the new vertex to the previous vertices to make it seem like it is tracing the lorenz
	glColor3d(vertexColors[l][0], vertexColors[l][1], vertexColors[l][2]);
	glVertex3f(vertices[l][0], vertices[l][1], vertices[l][2]);
	glEnd();
}

/*
 * Function that draws the lorenz attractor
 */
void drawLorenz(){
	int i;
	glBegin(GL_LINE_STRIP);
	for(i = 0;i < n;i++){
		glColor3d(vertexColors[i][0], vertexColors[i][1], vertexColors[i][2]);
		glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
	}
	glEnd();
}
/*
 * This function is called by GLUT to display the scene
 */
void display()
{
	// Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset previous transforms
	glLoadIdentity();
	// Set view angle
	glRotated(ph,1,0,0);
	glRotated(th,0,1,0);
	// Enable Z-buffering
	glEnable(GL_DEPTH_TEST);
	// Call the function that draws the lorenz attractor
	if(mode == 1){
		// Reset the trace to the first lorenz point
		l = 0;
		drawLorenz();
	}else{
		traceLorenz();
	}
	// Call the function to draw the x, y, z axes
	if(axes == 1){
		drawAxes();
	}
	// Set the color to white
	glColor3f(1.0, 1.0, 1.0);
	// Specify the coordinates for the raster position
	// Also keeps the bitmap characters on the screen while resizing the window
	glWindowPos2i(5,5);
	// Display text
	Print("View Angle = %d, %d %s b = %f r = %f s = %f", th, ph, text[mode], b, r, s);
	// Render the scene using double buffering (display doesn't stutter after using this)
	glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   // Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   // Set the viewport to the entire window
   glViewport(0,0, width,height);
   // Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   // Undo previous transformations
   glLoadIdentity();
   // Orthogonal projection box adjusted for the
   // aspect ratio of the window
   glOrtho(-dim * w2h, +dim * w2h, -dim, +dim, -dim, +dim);
   // Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   // Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	// Exit on ESC
	if(ch == 27){
		exit(0);
	}
	// Reset view angle and lorenz parameters
	else if(ch == '0'){
		th = ph = 0;
		s  = 10;
		b  = 2.6666;
		r  = 28;
		// Recreate the lorenz points after changing the parameter value
		createLorenzPoints();
	}
	// Switch display mode
	else if (ch == 'm'){
		mode = 1 - mode;
	}
	// Toggle axes
	else if (ch == 'a'){
		axes = 1-axes;
	}
	// View down X-axis
	else if(ch == 'x'){
		ph = 0;
		th = -90;
	}
	// View down Y-axis
	else if(ch == 'y'){
		th = -90;
		ph = 90;
	}
	// View down Z-axis
	else if(ch == 'z'){
		th = ph = 0;
	}
	// Increase the value of the s parameter of the Lorenz Attractor
	else if(ch == 'S'){
		l = 0;
		s += 1;
		// Recreate the lorenz points after changing the parameter value
		createLorenzPoints();
	}
	// Decrease the value of the s parameter of the Lorenz Attractor
	else if(ch == 's'){
		l = 0;
		s -= 1;
		// Recreate the lorenz points after changing the parameter value
		createLorenzPoints();
	}
	// Increase the value of the r parameter of the Lorenz Attractor
	else if(ch == 'R'){
		l = 0;
		r += 1;
		// Recreate the lorenz points after changing the parameter value
		createLorenzPoints();
	}
	// Decrease the value of the r parameter of the Lorenz Attractor
	else if(ch == 'r'){
		l = 0;
		r -= 1;
		// Recreate the lorenz points after changing the parameter value
		createLorenzPoints();
	}
	// Increase the value of the b parameter of the Lorenz Attractor
	else if(ch == 'B'){
		l = 0;
		b += 1;
		// Recreate the lorenz points after changing the parameter value
		createLorenzPoints();
	}
	// Decrease the value of the b parameter of the Lorenz Attractor
	else if(ch == 'b'){
		l = 0;
		b -= 1;
		// Recreate the lorenz points after changing the parameter value
		createLorenzPoints();
	}
	// Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 *
 */
void special(int key,int x,int y)
{
	// Right arrow key - increase azimuth by 5 degrees
	if (key == GLUT_KEY_RIGHT)
	th += 5;
	// Left arrow key - decrease azimuth by 5 degrees
	else if (key == GLUT_KEY_LEFT)
	th -= 5;
	// Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
	ph += 5;
	// Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
	ph -= 5;
	// Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	// Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
	// If in the right mode for tracing increment l to go through the lorenz attractor points
	if(mode == 0){
		if(l < n){
			l += 1;
		}else{
			l = 0;
		}
	}
	// Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
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
	// Request double buffered, true color window with Z buffering
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//Create the points need to draw the lorenz attractor
	createLorenzPoints();
	// Initialize GLUT
	glutInit(&argc,argv);
	// Request 500 x 500 pixel window
	glutInitWindowSize(600,600);
	// Create window
	glutCreateWindow("Lorenz Attractor");
	// Tell GLUT to call "key" when a key is pressed
	glutKeyboardFunc(key);
	// Tell GLUT to call "special" when an arrow key is pressed
	glutSpecialFunc(special);
	// Register function used to display scene
	glutDisplayFunc(display);
	// Register the reshape function that handles window resizing
	glutReshapeFunc(reshape);
	// Tell GLUT to call "idle" when nothing else is going on
	glutIdleFunc(idle);
	// Set the initial opengl states
	init();
	// Pass control to GLUT for events
	glutMainLoop();
	// Return code
	return 0;
}
