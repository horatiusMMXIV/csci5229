/*
 * Robert Werthman
 * CSCI 5229
 * hw6
 *
 * hw6.c
 *
 */

#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int smooth    =   1;  // Smooth/Flat shading
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov, asp, dim);
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z = Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
*  Draw a sphere
*	at (x,y,z)
* 	radius (r)
* 	with color red, green, blue
*/
void sphere(double x, double y, double z, double r, double red, double green, double blue)
{
	const int d=5;
	int th,ph;
	float color[] = {red,green,blue,1.0};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
	//  Save transformation
	glPushMatrix();
	//  Offset and scale
	glTranslatef(x, y, z);
	glScalef(r, r, r);

	//  Purple ball
	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT,GL_SPECULAR,color);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

	//  Latitude bands
	for (ph=-90;ph<90;ph+=d)
	{
		glBegin(GL_QUAD_STRIP);
		for (th=0;th<=360;th+=d)
		{
		 Vertex(th,ph);
		 Vertex(th,ph+d);
		}
		glEnd();
	}

	//  Undo transformations
	glPopMatrix();
}

/*
 * Draw a triangle
 * 	at (x,y,z)
 * 	with height (h), length (l), width (w)
 * 	at angle (angle) on x (ax), y (ay), or z (az)
 * 	with color red, green, blue
 */
void triangle(double x, double y, double z, double l, double h, double w, double angle, double ax, double ay, double az,
		double red, double green, double blue)
{
	// Have opengl correctly calculate the shading for both sides of the triangle
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glDisable(GL_CULL_FACE);
	float color[] = {red, green, blue, 1};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
	//  Save transformation
	glPushMatrix();
	// Offset, scale, and rotate
	glTranslatef(x, y, z);
	glScalef(l, h, w);
	glRotatef(angle, ax, ay, az);
	glColor3f(red, green, blue);
	// Draw a triangle clockwise
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, 1);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	//  Undo transformations
	glPopMatrix();
	glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     with height (h), length (l), width (w)
 *     at angle (angle) on x (ax), y (ay), or z (az)
 *     with color red, green, blue
 *
 */
void cube(double x, double y, double z, double l, double h, double w, double angle, double ax, double ay, double az,
		double red, double green, double blue)
{
	float color[] = {red, green, blue, 1};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
	//  Save transformation
	glPushMatrix();
	// Offset, scale
	glTranslatef(x, y, z);
	glRotatef(angle, ax, ay, az);
	glScalef(l, h, w);
	//  Cube
	// Front
	glColor3f(red, green, blue);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, +1);
	glVertex3f(-1,-1, 1);
	glVertex3f(+1,-1, 1);
	glVertex3f(+1,+1, 1);
	glVertex3f(-1,+1, 1);
	//  Back
	glNormal3f(0, 0, -1);
	glVertex3f(+1,-1,-1);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,+1,-1);
	glVertex3f(+1,+1,-1);
	//  Right
	glNormal3f(+1, 0, 0);
	glVertex3f(+1,-1,+1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(+1,+1,+1);
	//  Left
	glNormal3f(-1, 0, 0);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1,+1);
	glVertex3f(-1,+1,+1);
	glVertex3f(-1,+1,-1);
	//  Top
	glNormal3f(0, 1, 0);
	glVertex3f(-1,+1,+1);
	glVertex3f(+1,+1,+1);
	glVertex3f(+1,+1,-1);
	glVertex3f(-1,+1,-1);
	//  Bottom
	glNormal3f(0, -1, 0);
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
	cube(0, 0, 0, .5, .3, .3, 0, 0, 0, 0, 0, 0, 1);

	// Tail Bloom
	// Green
	cube(.8, .1, 0, .4, .1, .1, 0, 0, 0, 0, 0, 1, 0);

	// Tail Fin
	// Red
	triangle(1.1, .1, 0, .6, .6, .6, -45, 0, 0, 1, 1, 0, 0);

	// Tail Gearbox
	// Blue
	cube(1.3, .1, 0, .1, .1, .1, 0, 0, 0, 0, 0, 0, 1);

	//Tail Rotor Blades
	// White
	cube(1.3, .1, .18, .3, .06, .08, br + 45, 0, 0, 1, 1, 1, 1);

	// Rotor mast
	// Red
	cube(.2, .45, 0, .1, .2, .1, 0, 0, 0, 0, 1, 0, 0);

	// Main rotor blades
	// White
	cube(.2, .7, 0, .9, .1, .1, br + 0, 0, 1, 0, 1, 1, 1);
	cube(.2, .7, 0, .9, .1, .1, br + 90, 0, 1, 0, 1, 1, 1);

	// Skids
	// Yellow
	// Left skid mounts
	cube(-.2, -.3, .2, .3, .06, .06, 45, 0, 0, 1, 1, 1, 0);
	cube(.2, -.3, .2, .3, .06, .06, 45, 0, 0, 1, 1, 1, 0);

	// Left skid
	cube(-.1, -.5, .2, .7, .06, .06, 0, 0, 0, 0, 1, 1, 0);

	// Right skid mounts
	cube(-.2, -.3, -.2, .3, .06, .06, 45, 0, 0, 1, 1, 1, 0);
	cube(.2, -.3, -.2, .3, .06, .06, 45, 0, 0, 1, 1,1, 0);

	// Right skid
	cube(-.1, -.5, -.2, .7, .06, .06, 0, 0, 0, 0, 1, 1, 0);

	// Purple Cockpit
	sphere(-.4, .07, 0, .45, 0.5, 0.0, 0.5);
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
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	const double len=2.0;  //  Length of axes
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	// Enable face culling in OpenGL
	glEnable(GL_CULL_FACE);
	//  Undo previous transformations
	glLoadIdentity();
	//  Perspective - set eye position
	if (mode == 1){
		double Ex = -2*dim*Sin(th)*Cos(ph);
		double Ey = +2*dim        *Sin(ph);
		double Ez = +2*dim*Cos(th)*Cos(ph);
		gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
	}
	//  Orthogonal - set world orientation
	else if(mode == 0){
		glRotatef(ph,1,0,0);
		glRotatef(th,0,1,0);
	}

	if(light){
		//  Translate intensity to color vectors
		float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
		float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
		float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
		//  Light position
		float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
		//  Draw light position as ball (still no lighting here)
		glColor3f(1,1,1);
		sphere(Position[0],Position[1],Position[2] , 0.1, 1.0, 1.0, 1.0);
		//  OpenGL should normalize normal vectors
		glEnable(GL_NORMALIZE);
		//  Enable lighting
		glEnable(GL_LIGHTING);
		//  glColor sets ambient and diffuse color materials
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		//  Enable light 0
		glEnable(GL_LIGHT0);
		//  Set ambient, diffuse, specular components and position of light 0
		glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
		glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
		glLightfv(GL_LIGHT0,GL_POSITION,Position);
	}else{
		glDisable(GL_LIGHTING);
	}

	glPushMatrix();
	//helicopter(0);
	movingHelicopter();
	glPopMatrix();
	glColor3f(1,1,1);

	//  Draw axes - no lighting from here on
	glDisable(GL_LIGHTING);
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
	Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
	     th,ph,dim,fov,mode?"Perspective":"Orthogonal",light?"On":"Off");
	if (light)
	   {
	      glWindowPos2i(5,45);
	      Print("Distance=%d Elevation=%.1f", distance, ylight);
	      glWindowPos2i(5,25);
	      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
	   }
	// Check for any errors that have occurred
	ErrCheck("display");
	//  Render the scene and make it visible
	glFlush();
	glutSwapBuffers();
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
	glutInitWindowSize(700,700);
	glutCreateWindow("Robert Werthman Assignment 5");
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


