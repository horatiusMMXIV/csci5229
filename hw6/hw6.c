/*
 * Robert Werthman
 * CSCI 5229
 * hw6
 *
 * hw6.c
 *
 * Sources:
 * https://stackoverflow.com/questions/26536570/how-do-i-texture-a-cylinder-in-opengl-created-with-triangle-strip
 * - How to add textures to the cylinder
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

unsigned int littlebird[10];

/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(int th,int ph)
{
   double x = Sin(th)*Cos(ph);
   double y =  Cos(th)*Cos(ph);
   double z =          Sin(ph);
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+.5);
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

	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT,GL_SPECULAR,color);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

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
	glTranslatef(x, y, z);
	glScalef(l, h, w);
	glRotatef(angle, ax, ay, az);
	glColor3f(red, green, blue);

	// Draw a triangle clockwise
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0 ,0); glVertex3f(1, 0, 0);
	glTexCoord2f(1/2,1); glVertex3f(0, 1, 0);
	glTexCoord2f(1,0); glVertex3f(0, 0, 0);
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
	glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
	glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
	glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
	glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
	//  Back
	glNormal3f(0, 0, -1);
	glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
	glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
	glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
	//  Right
	glNormal3f(+1, 0, 0);
	glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
	glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
	glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
	glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
	//  Left
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
	glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
	glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
	//  Top
	glNormal3f(0, 1, 0);
	glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
	glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
	glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
	glTexCoord2f(1,0); glVertex3f(-1,+1,-1);
	//  Bottom
	glNormal3f(0, -1, 0);
	glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
	glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
	glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
	//  End
	glEnd();
	//  Undo transformations
	glPopMatrix();
}

/*
 *  Draw a cylinder
 */
static void cylinder(double x, double y, double z, double l, double h, double w, double angle, double ax, double ay, double az,
		double red, double green, double blue)
{
glDisable(GL_CULL_FACE);
   int th;

   float color[] = {red, green, blue, 1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,color);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   glPushMatrix();

   glColor3f(red, green, blue);
   glTranslatef(x, y, z);
   glRotatef(angle, ax, ay, az);
   glScalef(l, h, w);

   //  Cap
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,0,1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0,0,1);
	for (th=0;th<=360;th+=10){
		glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
		glVertex3f(Cos(th),Sin(th),1);
	}
	glEnd();

   //  Sides
	glBegin(GL_QUAD_STRIP);
	for (th=0;th<=360;th+=15)
	{
	const float tc = th/(float)360;
	 glNormal3f(Cos(th),Sin(th),0);
	 glTexCoord2f(tc, 0); glVertex3f(Cos(th),Sin(th),1);
	 glTexCoord2f(tc, 1); glVertex3f(Cos(th),Sin(th),0);
	}
	glEnd();

   glPopMatrix();
   glEnable(GL_CULL_FACE);

}

/*
 * Draw entire helicopter
 *	with blade rotation (br)
 */
void helicopter(double br){
	glEnable(GL_TEXTURE_2D);

	// Helicopter body
	glBindTexture(GL_TEXTURE_2D,littlebird[2]);
	glPushMatrix();
	glScalef(1.5,1,1);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	sphere(0,0,0,1,1,1,1);
	glPopMatrix();

	// Engine
	glBindTexture(GL_TEXTURE_2D,littlebird[3]);
	cylinder(1.1, -.2, 0, .2, .2, .5, 90, 0, 1, 0, 1, 1, 1);

	// Tail Bloom
	glBindTexture(GL_TEXTURE_2D,littlebird[1]);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(.1,.1);
	cylinder(.8, .6, 0, .2, .2, 3, 90, 0, 1, 0, 1, 1, 1);
	//glDisable(GL_POLYGON_OFFSET_FILL);
	triangle(1.5, .06, 0, 1, 1, 1, 45, 0, 0, 1, 1, 1, 1);

	// Tail fins
	glBindTexture(GL_TEXTURE_2D,littlebird[1]);
	triangle(3.5, .6, -.2, .7, 1.5, .6, -45, 0, 0, 1, 1, 1, 1);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(.1,.1);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	triangle(3.5, .2, 1.65, 1, 1, 1, -45, 0, 0, 1, 1, 1, 1);
	glPopMatrix();
	//glDisable(GL_POLYGON_OFFSET_FILL);
	triangle(4, 1.65, -.9, .4, .4, .4, -45, 0, 0, 1, 1, 1, 1);
	triangle(4, 1.65, .5, .4, .4, .4, -45, 0, 0, 1, 1, 1, 1);

	// Tail Gearbox
	glBindTexture(GL_TEXTURE_2D,littlebird[7]);
	cube(3.6, .6, .3, .07, .07, .1, 0, 0, 0, 0, 1, 1, 1);

	//Tail Rotor Blades
	glBindTexture(GL_TEXTURE_2D,littlebird[6]);
	cube(3.6, .6, .4, .4, .05, .08, br + 45, 0, 0, 1, 1, 1, 1);
	cube(3.6, .6, .4, .4, .05, .08, br - 45, 0, 0, 1, 1, 1, 1);

	// Main rotor mast
	glBindTexture(GL_TEXTURE_2D,littlebird[0]);
	cube(.4, .8, 0, .8, .3, .3, 0, 0, 0, 0, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D,littlebird[7]);
	cube(.2, 1.1, 0, .1, .18, .1, 0 ,0, 0, 0, 1, 1, 1);

	// Main rotor blades
	glBindTexture(GL_TEXTURE_2D,littlebird[6]);
	cube(.2, 1.3, 0, 2, .05, .2, br + 0, 0, 1, 0, 1, 1, 1);
	cube(.2, 1.3, 0, 2, .05, .2, br + 90, 0, 1, 0, 1, 1, 1);

	// Skids
	// Yellow
	// Left skid mounts
	glBindTexture(GL_TEXTURE_2D,littlebird[4]);
	glPushMatrix();
	glRotatef(-30, 1, 0, 0);
	cube(-.5, -1.1, .2, .3, .06, .06, 75, 0, 0, 1, 1, 1, 1);
	cube(.5, -1.1, .2, .3, .06, .06, 75, 0, 0, 1, 1, 1, 1);
	glPopMatrix();

	// Left skid
	glBindTexture(GL_TEXTURE_2D,littlebird[5]);
	cube(-.4, -1.1, .9, 1, .06, .06, 0, 0, 0, 0, 1, 1, 1);

	// Right skid mounts
	glBindTexture(GL_TEXTURE_2D,littlebird[4]);
	glPushMatrix();
	glRotatef(30, 1, 0, 0);
	cube(-.5, -1.1, -.2, .3, .06, .06, 75, 0, 0, 1, 1, 1, 1);
	cube(.5, -1.1, -.2, .3, .06, .06, 75, 0, 0, 1, 1, 1, 1);
	glPopMatrix();

	// Right skid
	glBindTexture(GL_TEXTURE_2D,littlebird[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	cube(-.4, -1.1, -.9, 1, .06, .06, 0, 0, 0, 0, 1, 1, 1);

	glDisable(GL_TEXTURE_2D);

	// Purple Cockpit
	//sphere(-.4, .07, 0, .45, 0.5, 0.0, 0.5);
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
	glTranslated(Cos(zh)*3, 1, -Sin(zh)*3);
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
	glutCreateWindow("Robert Werthman Assignment 6");
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	//  Tell GLUT to call "idle" when there is nothing else to do
	glutIdleFunc(idle);

	// Load the textures for the helicopter
	littlebird[0] = LoadTexBMP("littlebirdenginetank.bmp");
	littlebird[1] = LoadTexBMP("littlebirdenginetank.bmp");
	littlebird[2] = LoadTexBMP("cockpitfull.bmp");
	littlebird[3] = LoadTexBMP("littlebirdengine.bmp");
	littlebird[4] = LoadTexBMP("littlebirdskidmount.bmp");
	littlebird[5] = LoadTexBMP("littlebirdskid.bmp");
	littlebird[6] = LoadTexBMP("littlebirdrotor.bmp");
	littlebird[7] = LoadTexBMP("littlebirdgear.bmp");

	//  Check if any errors have occurred
	ErrCheck("init");
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	return 0;
}


