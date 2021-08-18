/*
 * Robert Werthman
 * CSCI 5229
 * Project
 *
 *
 * Sources:
 * https://stackoverflow.com/questions/26536570/how-do-i-texture-a-cylinder-in-opengl-created-with-triangle-strip
 * - How to add textures to the cylinder
 *
 *
 * https://stackoverflow.com/questions/16571981/gluperspective-parameters-what-do-they-mean
 * - How gluPerspective works with a world that is huge (large dim)
 *
 *
 */

#include "CSCIx229.h"
#include "Vector.h"

int mode=0;    //  Display mode
char* text[] = {"Flight", "Sky Box", "Ground", "Collision Detection",
					"Weapon", "Sound"};
						

// Sound that will perpectaully play
Mix_Music* music = NULL;

//The sound effects that will be used
Mix_Chunk *shot = NULL;
Mix_Chunk *bomb = NULL;

/* Variables for helicopter flight */
int yaw=0;
int pitch=0;
int roll=0;
int strafe=0;
int fly=0;
int bankAngle = 0;
int bankFactor = 0;
int speed=0;

/* Variable for the rotation of the helicopter
 * blades.
 */
int bladeRotation = 0;

/* Variables used when the helicopter explodes */
double explosion = 0;
int e = 0;



int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=500.0;   //  Size of world

/* Position array of the helicopter */
double littleBirdPosition[3];

/* Arrays for the textures of each object */
int littlebird[10];
int sky[6];
int tree[2];
int building[4];

/* Vectors for the helicopter flight */
Vector* directionVec = new Vector(1,0,0);
Vector* upVec =        new Vector(0,1,0);
Vector* rightVec =     new Vector(0,0,1);

/* Arrays for the tree, building, bullets, and
 * heights in the scene.
 */
int numBuildings = 250;
int numTrees = 1000;

double bullets[10][7];
int bulletIndex = 0;

double trees[1000][6];
double buildings[250][6];
int heightMap[65][65]={
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,1,1,1,1,1,1,1,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,2,2,2,2,2,2,2,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,2,3,3,3,3,3,2,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,2,3,8,8,8,3,2,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,2,3,8,5,8,3,2,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,2,3,8,10,8,3,2,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,2,3,8,8,8,3,2,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,27,27,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,2,2,2,2,2,2,2,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,27,30,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 1,1,1,1,1,1,1,1,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,21,20,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 15,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,15,15,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 50,50,50,50,50,50,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,20,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 50,100,100,100,50,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,15,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 50,100,105,100,50,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 50,100,100,100,50,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 50,50,50,50,50,50,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,5,5,5,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,5,9,5,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,20,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,5,5,5,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,18,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,17,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,16,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,5,9,12,9,5,0, 0,0,0,0,0,15,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,5,9,12,9,5,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,5,9,12,9,5,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,5,9,12,9,5,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,5,9,12,9,5,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,5,5,5,5,5,5,5,5, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,20,20,20,20,20,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,15,15,15,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,15,20,15,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,15,15,15,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		};

/*
 * This function sets the little bird position
 * and randomly generates the coordinates for the
 * trees and buildings.
 */
void init(){
	littleBirdPosition[0] = 0; littleBirdPosition[1] = 2; littleBirdPosition[2] = 0;
	// Randomly generate the position of the trees
	int i,r,rot;
	for(i=0;i<numTrees;i++){
		rot = rand()%360;
		r = rand() % 1000 + (-500);
		trees[i][0] = r;
		r = rand() % 1000 + (-500);
		trees[i][1] = r;
		trees[i][2] = 0;
		trees[i][3] = 0;
		trees[i][4] = 1;
		trees[i][5] = rot;

	}

	// Randomly generate the position of the buildings
	for(i=0;i<numBuildings;i++){
		rot = rand()%360;
		r = rand() % 1000 + (-500);
		buildings[i][0] = r;
		r = rand() % 1000 + (-500);
		buildings[i][1] = r;
		buildings[i][2] = 0;
		buildings[i][3] = 0;
		buildings[i][4] = .5;
		buildings[i][5] = rot;
	}

	// Initialize the bullets array
	for(i=0;i<9;i++){
		bullets[i][6] = 0;
	}

	//  Initialize audio
	if (Mix_OpenAudio(44100,AUDIO_S16SYS,2,4096)) Fatal("Cannot initialize audio\n");	
	//  Load the helicopter sound
	music = Mix_LoadMUS("helicopter.wav");
	if (!music) Fatal("Cannot load helicopter.wav\n");
	// Load the shooting bullet sound
	shot = Mix_LoadWAV( "shot.wav" );
	if (!music) Fatal("Cannot load shot.wav\n");
	// Load the sound of an explosion
	bomb = Mix_LoadWAV( "bomb.wav" );
	if (!music) Fatal("Cannot load bomb.wav\n");
	//  Play (looping)
	Mix_PlayMusic(music,-1);
	Mix_PauseMusic();
}

/*
 * This functions rolls the helicopter.
 */
void HelicopterRoll(){

	if(roll > 0){
		//bankAngle++;
		bankAngle += bankFactor;
	}else if(roll < 0){
		//bankAngle--;
		bankAngle += bankFactor;
	}

	directionVec->x = Cos(bankAngle);
	directionVec->z = -Sin(bankAngle);
	directionVec->normalize();

	rightVec->crossProduct(directionVec, upVec);
	rightVec->normalize();

	yaw = bankAngle;
}

/*
 * This function rotates the helicopter left or right.
 */
void HelicopterYaw(){

	directionVec->x = Cos(yaw);
	directionVec->z = -Sin(yaw);
	directionVec->normalize();

	rightVec->crossProduct(directionVec, upVec);
	rightVec->normalize();

	bankAngle = yaw;
}

/*
 * This funciton moves the helicopter up or down.
 */
void HelicopterFly(){
	littleBirdPosition[0] += upVec->x*(fly/20.0);
	littleBirdPosition[1] += upVec->y*(fly/20.0);
	littleBirdPosition[2] += upVec->z*(fly/20.0);
}

/*
 * This function moves the helicopter left or right.
 */
void HelicopterStrafe(){
	littleBirdPosition[0] += rightVec->x*(strafe/20.0);
	littleBirdPosition[1] += rightVec->y*(strafe/20.0);
	littleBirdPosition[2] += rightVec->z*(strafe/20.0);
}

/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(int th,int ph, double rep)
{
   double x = Sin(th)*Cos(ph);
   double y =  Cos(th)*Cos(ph);
   double z =          Sin(ph);
   glNormal3d(x,y,z);
   glTexCoord2d(rep*th/360.0,rep*ph/180.0+.5);
   glVertex3d(x,y,z);
}

/*
*  This funciton draws a sphere
* 	with color red, green, blue
* 	and texture repetition rep.
*/
void sphere(float red, float green, float blue, double rep)
{
	int th,ph;
	float color[] = {red,green,blue,1.0};
	float Emission[]  = {0.0,0.0,0.0,1.0};


	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT,GL_SPECULAR,color);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

	for (ph=-90;ph<90;ph+=5)
	{
		glBegin(GL_QUAD_STRIP);
		for (th=0;th<=360;th+=5)
		{
			Vertex(th,ph, rep);
			Vertex(th,ph+5, rep);
		}
		glEnd();
	}
}

/*
 * This function draws a triangle wedge
 * 	with color red, green, blue
 * 	and texture repetition rep.
 */
void triangle(float red, float green, float blue, double rep)
{
	float color[] = {red, green, blue, 1};
	float Emission[]  = {0.0,0.0,0.0,1.0};

	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

	glColor3f(red, green, blue);

	// Front
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0 ,0); glVertex3f(1, 0, 0);
	glTexCoord2f(rep/2,rep); glVertex3f(0, 1, 0);
	glTexCoord2f(rep,0); glVertex3f(0, 0, 0);
	glEnd();

	// Back
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0 ,0); glVertex3f(0, 0, -1);
	glTexCoord2f(rep/2,rep); glVertex3f(0, 1, -1);
	glTexCoord2f(rep,0); glVertex3f(1, 0, -1);
	glEnd();

	// Adjacent edge
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(0,0); glVertex3f(0, 0, 0);
	glTexCoord2f(rep,0); glVertex3f(0, 0, -1);
	glTexCoord2f(rep,rep); glVertex3f(1, 0, -1);
	glTexCoord2f(0,rep); glVertex3f(1, 0, 0);
	glEnd();

	// Opposite edge
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0,0); glVertex3f(0, 0, 0);
	glTexCoord2f(rep,0); glVertex3f(0, 1, 0);
	glTexCoord2f(rep,rep); glVertex3f(0, 1, -1);
	glTexCoord2f(0,rep); glVertex3f(0, 0, -1);
	glEnd();

	// Hypotenuse edge
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 0);
	glTexCoord2f(0,0); glVertex3f(0, 1, 0);
	glTexCoord2f(rep,0); glVertex3f(1, 0, 0);
	glTexCoord2f(rep,rep); glVertex3f(1, 0, -1);
	glTexCoord2f(0,rep); glVertex3f(0, 1, -1);
	glEnd();
}

/*
 *  This function draws a cube
 *  with color red, green, blue
 *  and texture repetition rep.
 *
 */
void cube(float red, float green, float blue, double rep)
{
	float color[] = {red, green, blue, 1};
	float Emission[]  = {0.0,0.0,0.0,1.0};

	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

	glColor3f(red, green, blue);

	//  Cube
	// Front
	glBegin(GL_QUADS);
	glNormal3f(0, 0, +1);
	glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
	glTexCoord2f(rep,0); glVertex3f(+1,-1, 1);
	glTexCoord2f(rep,rep); glVertex3f(+1,+1, 1);
	glTexCoord2f(0,rep); glVertex3f(-1,+1, 1);
	//  Back
	glNormal3f(0, 0, -1);
	glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
	glTexCoord2f(rep,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(rep,rep); glVertex3f(-1,+1,-1);
	glTexCoord2f(0,rep); glVertex3f(+1,+1,-1);
	//  Right
	glNormal3f(+1, 0, 0);
	glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
	glTexCoord2f(rep,0); glVertex3f(+1,-1,-1);
	glTexCoord2f(rep,rep); glVertex3f(+1,+1,-1);
	glTexCoord2f(0,rep); glVertex3f(+1,+1,+1);
	//  Left
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(rep,0); glVertex3f(-1,-1,+1);
	glTexCoord2f(rep,rep); glVertex3f(-1,+1,+1);
	glTexCoord2f(0,rep); glVertex3f(-1,+1,-1);
	//  Top
	glNormal3f(0, 1, 0);
	glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
	glTexCoord2f(rep,0); glVertex3f(+1,+1,+1);
	glTexCoord2f(rep,rep); glVertex3f(+1,+1,-1);
	glTexCoord2f(0,rep); glVertex3f(-1,+1,-1);
	//  Bottom
	glNormal3f(0, -1, 0);
	glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(rep,0); glVertex3f(+1,-1,-1);
	glTexCoord2f(rep,rep); glVertex3f(+1,-1,+1);
	glTexCoord2f(0,rep); glVertex3f(-1,-1,+1);
	//  End
	glEnd();
}

/*
 *  This function draws a cylinder
 *  with color red, green, blue
 *  and texture repetition rep.
 */
void cylinder(float red, float green, float blue, double rep)
{
	int th;
	float color[] = {red, green, blue, 1};
	float Emission[]  = {0.0,0.0,0.0,1.0};


	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

	glColor3f(red, green, blue);

	//  Cap
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0,0,1);
	glTexCoord2f(0.5,0.5);
	glVertex3f(0,0,1);
	for (th=0;th<=360;th+=10){
		glTexCoord2f(rep/2*Cos(th)+0.5,rep/2*Sin(th)+0.5);
		glVertex3f(Cos(th),Sin(th),1);
	}
	glEnd();

	//  Sides
	glBegin(GL_QUAD_STRIP);
	for (th=0;th<=360;th+=15)
	{
		const float tc = th/(float)360;
		glNormal3f(Cos(th),Sin(th),0);
		glTexCoord2f(rep*tc, 0); glVertex3f(Cos(th),Sin(th),1);
		glTexCoord2f(rep*tc, rep); glVertex3f(Cos(th),Sin(th),0);
	}
	glEnd();
}

/*
 * This function draws entire helicopter
 *	with blade rotation (br).
 */
void helicopter(double br){
	glPushMatrix();
	glTranslatef(0,1,0);
	glRotated(180,0,1,0);
	glScaled(.5,.5,.5);

	glEnable(GL_TEXTURE_2D);

	/* Helicopter Body */
	glBindTexture(GL_TEXTURE_2D,littlebird[2]);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1,1.5,1);
	sphere(1,1,1,1);
	glPopMatrix();

	/* Engine */
	glBindTexture(GL_TEXTURE_2D,littlebird[3]);
	glPushMatrix();
	glTranslatef(1.1, -.2, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.2, .2, .5);
	cylinder(1, 1, 1, 2);
	glPopMatrix();

	/* Tail Bloom */
	glBindTexture(GL_TEXTURE_2D,littlebird[1]);
	glPushMatrix();
	glTranslatef(.8, .6, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.2, .2, 3);
	cylinder(1,1,1,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, .06, .05);
	glRotatef(45, 0, 0, 1);
	glScalef(.5, 1, .1);
	triangle(1, 1, 1, 2);
	glPopMatrix();

	/* Tail Fins */
	glBindTexture(GL_TEXTURE_2D,littlebird[1]);
	glPushMatrix();
	glTranslatef(3.5, .6, -.2);
	glRotatef(-45, 0, 0, 1);
	glScalef(1, 1, .1);
	triangle(1, 1, 1, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.7, 1.4, -.2);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-45, 0, 0, 1);
	glScalef(1,1,.1);
	triangle(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.2, 1.4, -.9);
	glRotatef(-45, 0, 0, 1);
	glScalef(.4, .4, .1);
	triangle(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.2, 1.4, 0.6);
	glRotatef(-45, 0, 0, 1);
	glScalef(.4, .4, .1);
	triangle(1, 1, 1,2);
	glPopMatrix();

	/* Tail Gearbox */
	glBindTexture(GL_TEXTURE_2D,littlebird[7]);
	glPushMatrix();
	glTranslatef(3.6, .6, .3);
	glScalef(.07, .07, .1);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Tail Rotor Blades */
	glBindTexture(GL_TEXTURE_2D,littlebird[6]);
	glPushMatrix();
	glTranslatef(3.6, .6, .4);
	glRotatef(br + 45, 0, 0, 1);
	glScalef(.4, .05, .08);
	cube(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.6, .6, .4);
	glRotatef(br - 45, 0, 0, 1);
	glScalef(.4, .05, .08);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Main Rotor Mast */
	glBindTexture(GL_TEXTURE_2D,littlebird[1]);
	glPushMatrix();
	glTranslatef(.4, .8, 0);
	glScalef(.8, .3, .3);
	cube(1, 1, 1,2);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D,littlebird[7]);
	glPushMatrix();
	glTranslatef(.2, 1.1, 0);
	glScalef(.1, .18, .1);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Main Rotor Blades */
	glBindTexture(GL_TEXTURE_2D,littlebird[6]);
	glPushMatrix();
	glTranslatef(.2, 1.3, 0);
	glRotatef(br + 0, 0, 1, 0);
	glScalef(2, .05, .2);
	cube(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.2, 1.3, 0);
	glRotatef(br + 90, 0, 1, 0);
	glScalef(2, .05, .2);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Skids */
	/* Left Skid Mounts */
	glBindTexture(GL_TEXTURE_2D,littlebird[4]);
	glPushMatrix();
	glTranslatef(-.5, -.9, .5);
	glRotatef(-30, 1, 0, 0);
	glRotatef(75, 0, 0, 1);
	glScalef(.3, .06, .06);
	cube(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.5, -.9, .5);
	glRotatef(-30, 1, 0, 0);
	glRotatef(75, 0, 0, 1);
	glScalef(.3, .06, .06);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Right Skid Mounts */
	glBindTexture(GL_TEXTURE_2D,littlebird[4]);
	glPushMatrix();
	glTranslatef(-.5, -.9, -.5);
	glRotatef(30, 1, 0, 0);
	glRotatef(75, 0, 0, 1);
	glScalef(.3, .06, .06);
	cube(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.5, -.9, -.5);
	glRotatef(30, 1, 0, 0);
	glRotatef(75, 0, 0, 1);
	glScalef(.3, .06, .06);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Right Skid */
	glBindTexture(GL_TEXTURE_2D,littlebird[5]);
	glPushMatrix();
	glTranslatef(-.4, -1.15, .65);
	glScalef(1, .06, .06);
	cube(1, 1, 1,3);
	glPopMatrix();

	/* Left Skid */
	glBindTexture(GL_TEXTURE_2D,littlebird[5]);
	glPushMatrix();
	glTranslatef(-.4, -1.15, -.65);
	glScalef(1, .06, .06);
	cube(1, 1, 1,3);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void ExplodedTree(double d, double height){
	glEnable(GL_TEXTURE_2D);
	// Trunk
	glBindTexture(GL_TEXTURE_2D,tree[0]);
	glPushMatrix();
	glRotated(-90,1,0,0);
	glScalef(.1,.1,1);
	cylinder(1,1,1,1);
	glPopMatrix();

	//Branches
	glBindTexture(GL_TEXTURE_2D,tree[1]);
	glPushMatrix();
	glTranslatef(0*d,height+1,-1*d);
	glRotated(-135,0,0,1);
	glScalef(1.2,1.2,.3);
	triangle(1,1,1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1*d,height+1,0*d);
	glRotated(90,0,1,0);
	glRotated(-135,0,0,1);
	glScalef(1.2,1.2,.3);
	triangle(1,1,1,1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void ExplodedBuilding(double d, double height){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,building[0]);
	//Left
	glPushMatrix();
	glTranslatef(-1*d, height,0);
	glScalef(1,1,.2);
	cube(1,1,1,2);
	glPopMatrix();

	//Front
	glPushMatrix();
	glTranslatef(1*d, height,0);
	glRotated(-90,0,1,0);
	glScalef(1,1,.2);
	cube(1,1,1,2);
	glPopMatrix();

	//Back
	glPushMatrix();
	glTranslatef(0,height,1*d);
	glRotated(-90,0,1,0);
	glScalef(1,1,.2);
	cube(1,1,1,2);
	glPopMatrix();

	//Right
	glPushMatrix();
	glTranslatef(0,height,-1*d);
	glScalef(1,1,.2);
	cube(1,1,1,2);
	glPopMatrix();

	//Roof
	glBindTexture(GL_TEXTURE_2D,building[1]);
	glPushMatrix();
	glTranslatef(1*d,height+.1,-1*d);
	glScalef(1.2,.1,1.2);
	cube(1,1,1,1);
	glPopMatrix();

	//Door
	glBindTexture(GL_TEXTURE_2D,building[2]);
	glPushMatrix();
	glTranslatef(-1*d,height,1*d);
	glScalef(.1,.5,.3);
	cube(1,1,1,1);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	//Back Window
	glPushMatrix();
	glTranslatef(-1*d,height,-1*d);
	glScalef(.05,.2,.2);
	cube(0,0,0,1);
	glPopMatrix();

	//Right Window
	glPushMatrix();
	glTranslatef(1*d,height,1*d);
	glRotated(-90,0,1,0);
	glScalef(.05,.2,.2);
	cube(0,0,0,1);
	glPopMatrix();

	//Left Window
	glPushMatrix();
	glTranslatef(0,height,0);
	glRotated(-90,0,1,0);
	glScalef(.05,.2,.2);
	cube(0,0,0,1);
	glPopMatrix();
}

/*
 * This function draws pieces of the helicopter
 * moving in random directions after it hits
 * something.
 */
void explodedHelicopter(double d){
	glPushMatrix();
	glTranslatef(0,1,0);
	glRotated(180,0,1,0);
	glScaled(.5,.5,.5);

	glEnable(GL_TEXTURE_2D);

	/* Helicopter Body */
	glBindTexture(GL_TEXTURE_2D,littlebird[2]);
	glPushMatrix();
	srand(1);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1,1.5,1);
	sphere(1,1,1,1);
	glPopMatrix();

	/* Engine */
	glBindTexture(GL_TEXTURE_2D,littlebird[3]);
	glPushMatrix();
	srand(2);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(90, 0, 1, 0);
	glScalef(.2, .2, .5);
	cylinder(1, 1, 1, 2);
	glPopMatrix();

	/* Tail Bloom */
	glBindTexture(GL_TEXTURE_2D,littlebird[1]);
	glPushMatrix();
	srand(3);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(90, 0, 1, 0);
	glScalef(.2, .2, 3);
	cylinder(1,1,1,2);
	glPopMatrix();

	glPushMatrix();
	srand(4);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(45, 0, 0, 1);
	glScalef(.5, 1, .1);
	triangle(1, 1, 1, 2);
	glPopMatrix();

	/* Tail Fins */
	glBindTexture(GL_TEXTURE_2D,littlebird[1]);
	glPushMatrix();
	srand(5);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(-45, 0, 0, 1);
	glScalef(1, 1, .1);
	triangle(1, 1, 1, 2);
	glPopMatrix();

	glPushMatrix();
	srand(6);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-45, 0, 0, 1);
	glScalef(1,1,.1);
	triangle(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	srand(7);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(-45, 0, 0, 1);
	glScalef(.4, .4, .1);
	triangle(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	srand(8);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(-45, 0, 0, 1);
	glScalef(.4, .4, .1);
	triangle(1, 1, 1,2);
	glPopMatrix();

	/* Tail Gearbox */
	glBindTexture(GL_TEXTURE_2D,littlebird[7]);
	glPushMatrix();
	srand(9);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(.07, .07, .1);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Tail Rotor Blades */
	glBindTexture(GL_TEXTURE_2D,littlebird[6]);
	glPushMatrix();
	srand(10);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(.4, .05, .08);
	cube(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	srand(11);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(.4, .05, .08);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Main Rotor Mast */
	glBindTexture(GL_TEXTURE_2D,littlebird[1]);
	glPushMatrix();
	srand(12);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(.8, .3, .3);
	cube(1, 1, 1,2);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D,littlebird[7]);
	glPushMatrix();
	srand(13);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(.1, .18, .1);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Main Rotor Blades */
	glBindTexture(GL_TEXTURE_2D,littlebird[6]);
	glPushMatrix();
	srand(14);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(2, .05, .2);
	cube(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	srand(15);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(2, .05, .2);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Skids */
	/* Left Skid Mounts */
	glBindTexture(GL_TEXTURE_2D,littlebird[4]);
	glPushMatrix();
	srand(16);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(-30, 1, 0, 0);
	glRotatef(75, 0, 0, 1);
	glScalef(.3, .06, .06);
	cube(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	srand(17);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(-30, 1, 0, 0);
	glRotatef(75, 0, 0, 1);
	glScalef(.3, .06, .06);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Right Skid Mounts */
	glBindTexture(GL_TEXTURE_2D,littlebird[4]);
	glPushMatrix();
	srand(18);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(30, 1, 0, 0);
	glRotatef(75, 0, 0, 1);
	glScalef(.3, .06, .06);
	cube(1, 1, 1,2);
	glPopMatrix();

	glPushMatrix();
	srand(19);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glRotatef(30, 1, 0, 0);
	glRotatef(75, 0, 0, 1);
	glScalef(.3, .06, .06);
	cube(1, 1, 1,2);
	glPopMatrix();

	/* Right Skid */
	glBindTexture(GL_TEXTURE_2D,littlebird[5]);
	glPushMatrix();
	srand(21);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(1, .06, .06);
	cube(1, 1, 1,3);
	glPopMatrix();

	/* Left Skid */
	glBindTexture(GL_TEXTURE_2D,littlebird[5]);
	glPushMatrix();
	srand(22);
	glTranslatef(((rand()%10+(-5))*d),((rand()%10+(-5))*d),((rand()%10+(-5)))*d);
	glScalef(1, .06, .06);
	cube(1, 1, 1,3);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

/*
 * This creates a single bullet and stores its starting position
 * and its direction vector at the time of creation.
 */
void CreateBullet(){
	// Store the position of the bullet which is
	// the position of the helicopter when the
	// bullet was created.
	bullets[bulletIndex][0] = littleBirdPosition[0];
	bullets[bulletIndex][1] = littleBirdPosition[1]+1;
	bullets[bulletIndex][2] = littleBirdPosition[2];
	// Find what direction the helicopter was facing at the time.
	double Ex = Cos(yaw)*Cos(pitch);
	double Ey = Sin(pitch);
	double Ez = -Sin(yaw)*Cos(pitch);
	// Store the direction vector for later use
	// to move bullet along that path
	bullets[bulletIndex][3] = Ex;
	bullets[bulletIndex][4] = Ey;
	bullets[bulletIndex][5] = Ez;
	// Enable the bullet to be drawn
	bullets[bulletIndex][6] = 1;
}

/*
 * This function iterates through the bullets array and
 * redraws the bullets so it looks like they are moving.
 */
void ShootCannon(){
	int i;
	for(i=0;i<9;i++){
		if(bullets[i][6]>0){
			bullets[i][0] += bullets[i][3]*5;
			bullets[i][1] += bullets[i][4]*5;
			bullets[i][2] += bullets[i][5]*5;
			glPushMatrix();
			glTranslatef(bullets[i][0], bullets[i][1], bullets[i][2]);
			glScalef(.2,.2,.2);
			sphere(0,0,0,0);
			glPopMatrix();
		}
	}
}

/*
 * This function draws a sky box.
 */
void DrawSky(){
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	// Left
	glBindTexture(GL_TEXTURE_2D,sky[0]);
	glBegin(GL_QUADS);
	glTexCoord2d(0,1);glVertex3f(-512,-150,-512);
	glTexCoord2d(1,1);glVertex3f(512,-150,-512);
	glTexCoord2d(1,0);glVertex3f(512,362,-512);
	glTexCoord2d(0,0);glVertex3f(-512,362,-512);
	glEnd();
	// Right
	glBindTexture(GL_TEXTURE_2D,sky[1]);
	glBegin(GL_QUADS);
	glTexCoord2d(0,1);glVertex3f(512,-150,512);
	glTexCoord2d(1,1);glVertex3f(-512,-150,512);
	glTexCoord2d(1,0);glVertex3f(-512,362,512);
	glTexCoord2d(0,0);glVertex3f(512,362,512);
	glEnd();
	// Front
	glBindTexture(GL_TEXTURE_2D,sky[2]);
	glBegin(GL_QUADS);
	glTexCoord2d(0,1);glVertex3f(511,-150,-513);
	glTexCoord2d(1,1);glVertex3f(511,-150,513);
	glTexCoord2d(1,0);glVertex3f(511,362,513);
	glTexCoord2d(0,0);glVertex3f(511,362,-513);
	glEnd();
	// Back
	glBindTexture(GL_TEXTURE_2D,sky[3]);
	glBegin(GL_QUADS);
	glTexCoord2d(0,1);glVertex3f(-511,-150,513);
	glTexCoord2d(1,1);glVertex3f(-511,-150,-513);
	glTexCoord2d(1,0);glVertex3f(-511,362,-513);
	glTexCoord2d(0,0);glVertex3f(-511,362,513);
	glEnd();
	// Top
	glBindTexture(GL_TEXTURE_2D,sky[4]);
	glBegin(GL_QUADS);
	glTexCoord2d(0,1);glVertex3f(513,361,-513);
	glTexCoord2d(1,1);glVertex3f(513,361,513);
	glTexCoord2d(1,0);glVertex3f(-513,361,513);
	glTexCoord2d(0,0);glVertex3f(-513,361,-513);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

/*
 * This funciton detects collisions with trees, buildings,
 * and sky box limits.
 */
void checkCollision(){
	// Collision with sides or top of scene
	double xH = littleBirdPosition[0];
	double yH = littleBirdPosition[1];
	double zH = littleBirdPosition[2];
	//Collision with min or max y-axis
	if(yH>350.0 || yH<1){
		// Stop the helicopter from moving upwards
		e = 1;
		pitch=roll=strafe=fly=bankFactor=bankAngle=speed=0;
		if(mode==5){
			Mix_PlayChannel(-1,bomb,0);
			// Pause music playback
			Mix_PauseMusic();
		}
		return;
	}
	// Collision with min or max of x-axis
	if(xH<-500 || xH>500){
		e = 1;
		pitch=roll=strafe=fly=bankFactor=bankAngle=speed=0;
		if(mode==5){
			Mix_PlayChannel(-1,bomb,0);
			// Pause music playback
			Mix_PauseMusic();
		}
		return;
	}
	// Collision with min or max of z-axis
	if(zH<-500 || zH>500){
		e = 1;
		pitch=roll=strafe=fly=bankFactor=bankAngle=speed=0;
		if(mode==5){
			Mix_PlayChannel(-1,bomb,0);
			// Pause music playback
			Mix_PauseMusic();
		}
		return;
	}

	// Detect a collision with a tree by helicopter
	double x,z;
	int i,j;
	double y = 5.0;
	for(i=0;i<numTrees;i++){
		x = trees[i][0];
		z = trees[i][1];
		if(xH<=(x+2)&&xH>=(x-2)){
			if(zH<=(z+2)&&zH>=(z-2)){
				if(yH<=y){
					trees[i][2] = 1;
					e = 1;
					pitch=roll=strafe=fly=bankFactor=bankAngle=speed=0;
					if(mode==5){
						Mix_PlayChannel(-1,bomb,0);
						// Pause music playback
						Mix_PauseMusic();
					}
					return;
				}
			}
		}
	}

	// Detect a collision with a house by helicopter
	y = 2.5;
	for(i=0;i<numBuildings;i++){
		x = buildings[i][0];
		z = buildings[i][1];
		if(xH<=(x+2)&&xH>=(x-2)){
			if(zH<=(z+2)&&zH>=(z-2)){
				if(yH<=y){
					buildings[i][2] = 1;
					e = 1;
					pitch=roll=strafe=fly=bankFactor=bankAngle=speed=0;
					if(mode==5){
						Mix_PlayChannel(-1,bomb,0);
						// Pause music playback
						Mix_PauseMusic();
					}
					return;
				}
			}
		}
	}

	for(j=0;j<9;j++){
		if(bullets[j][6]>00){
			double xH = bullets[j][0];
			double yH = bullets[j][1];
			double zH = bullets[j][2];
			// Detect a collision with a tree by bullet
			y = 5.0;
			for(i=0;i<numTrees;i++){
				x = trees[i][0];
				z = trees[i][1];
				if(xH<=(x+2)&&xH>=(x-2)){
					if(zH<=(z+2)&&zH>=(z-2)){
						if(yH<=y&&yH>= -1){
							trees[i][2] = 1;
							if(mode==5){
								// Pause music playback
								Mix_PlayChannel(-1,bomb,0);
							}
							return;
						}
					}
				}
			}

			// Detect a collision with a house by bullet
			y = 2.5;
			for(i=0;i<numBuildings;i++){
				x = buildings[i][0];
				z = buildings[i][1];
				if(xH<=(x+3)&&xH>=(x-3)){
					if(zH<=(z+3)&&zH>=(z-3)){
						if(yH<=y&&yH>=-2){
							buildings[i][2] = 1;
							if(mode==5){
								// Pause music playback
								Mix_PlayChannel(-1,bomb,0);
							}
							return;
						}
					}
				}
			}
		}
	}
}

/*
 * This function draws the trees, buildings, and
 * terrain made of squares.
 */
void DrawLand(){
	glPushMatrix();
	int i,j;
	double x,z;
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,sky[5]);
	// Draw the ground from -512,-512 (x,z) with
	// 16x16 squares.
	for(i=0;i<64;i++){
		x = 16*i-512;
		for(j=0;j<64;j++){
			z = 16*j-512;
			glBegin(GL_QUADS);
			glNormal3f(0, 1, 0);
			glTexCoord2f((j)/64.,(i)/64.);glVertex3d(x,heightMap[64-i][j],z);
			glTexCoord2f((j+1)/64.,(i)/64.);glVertex3d(x,heightMap[64-i][j+1],z+16);
			glTexCoord2f((j+1)/64.,(i+1)/64.);glVertex3d(x+16,heightMap[64-(i+1)][j+1],z+16);
			glTexCoord2f((j)/64.,(i+1)/64.);glVertex3d(x+16,heightMap[64-(i+1)][j],z);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Draw the trees in the scene
	for(i=0;i<numTrees;i++){
		if(trees[i][2]<1){
			glPushMatrix();
			glTranslatef(trees[i][0], 0, trees[i][1]);
			glRotatef(trees[i][5],0,1,0);
			glScaled(3,3,3);
			DrawTree();
			glPopMatrix();
		}else{
			if(trees[i][4] > 0){
				trees[i][3] += .1;
				trees[i][4] -= .1;
			}
			glPushMatrix();
			glTranslatef(trees[i][0], 0, trees[i][1]);
			glRotatef(trees[i][5],0,1,0);
			glScaled(3,3,3);
			ExplodedTree(trees[i][3], trees[i][4]);
			glPopMatrix();
		}
	}

	// Draw the houses in the scene
	for(i=0;i<numBuildings;i++){
		if(buildings[i][2]<1){
			glPushMatrix();
			glTranslatef(buildings[i][0], 0, buildings[i][1]);
			glRotatef(buildings[i][5],0,1,0);
			glScaled(2,2,2);
			DrawBuilding();
			glPopMatrix();
		}else{
			if(buildings[i][4] > 0){
				buildings[i][3] += .1;
				buildings[i][4] -= .1;
			}
			glPushMatrix();
			glTranslatef(buildings[i][0], 0, buildings[i][1]);
			glRotatef(buildings[i][5],0,1,0);
			glScaled(2,2,2);
			ExplodedBuilding(buildings[i][3], buildings[i][4]);
			glPopMatrix();
		}
	}

}

/*
 * This function draws a building in the landscape.
 */
void DrawBuilding(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,building[0]);
	//Left
	glPushMatrix();
	glScalef(1,1,.2);
	cube(1,1,1,2);
	glPopMatrix();

	//Front
	glPushMatrix();
	glTranslatef(-.85,0,.85);
	glRotated(-90,0,1,0);
	glScalef(1,1,.2);
	cube(1,1,1,2);
	glPopMatrix();

	//Back
	glPushMatrix();
	glTranslatef(.85,0,.85);
	glRotated(-90,0,1,0);
	glScalef(1,1,.2);
	cube(1,1,1,2);
	glPopMatrix();

	//Right
	glPushMatrix();
	glTranslatef(0,0,1.7);
	glScalef(1,1,.2);
	cube(1,1,1,2);
	glPopMatrix();

	//Roof
	glBindTexture(GL_TEXTURE_2D,building[1]);
	glPushMatrix();
	glTranslatef(0,1,.85);
	glScalef(1.2,.1,1.2);
	cube(1,1,1,1);
	glPopMatrix();

	//Door
	glBindTexture(GL_TEXTURE_2D,building[2]);
	glPushMatrix();
	glTranslatef(-1,.3,.85);
	glScalef(.1,.5,.3);
	cube(1,1,1,1);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	//Back Window
	glPushMatrix();
	glTranslatef(1.05,.5,.8);
	glScalef(.05,.2,.2);
	cube(0,0,0,1);
	glPopMatrix();

	//Right Window
	glPushMatrix();
	glTranslatef(0,.5,1.9);
	glRotated(-90,0,1,0);
	glScalef(.05,.2,.2);
	cube(0,0,0,1);
	glPopMatrix();

	//Left Window
	glPushMatrix();
	glTranslatef(0,.5,-.2);
	glRotated(-90,0,1,0);
	glScalef(.05,.2,.2);
	cube(0,0,0,1);
	glPopMatrix();
}

/*
 * This function draws a tree in the landscape.
 */
void DrawTree(){
	glEnable(GL_TEXTURE_2D);
	// Trunk
	glBindTexture(GL_TEXTURE_2D,tree[0]);
	glPushMatrix();
	glRotated(-90,1,0,0);
	glScalef(.1,.1,1);
	cylinder(1,1,1,1);
	glPopMatrix();

	//Branches
	glBindTexture(GL_TEXTURE_2D,tree[1]);
	glPushMatrix();
	glTranslatef(0,1.5,.15);
	glRotated(-135,0,0,1);
	glScalef(1.2,1.2,.3);
	triangle(1,1,1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.15,1.5,0);
	glRotated(90,0,1,0);
	glRotated(-135,0,0,1);
	glScalef(1.2,1.2,.3);
	triangle(1,1,1,1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

/*
 * This function draws the movement of the helicopter
 * relative to the camera.
 */
void DrawHelicopterFlight(){
	double mat[16];
	mat[0] = directionVec->x;   mat[4] = upVec->x;   mat[ 8] = rightVec->x;   mat[12] = 0;
	mat[1] = directionVec->y;   mat[5] = upVec->y;   mat[ 9] = rightVec->y;   mat[13] = 0;
	mat[2] = directionVec->z;   mat[6] = upVec->z;   mat[10] = rightVec->z;   mat[14] = 0;
	mat[3] =               0;   mat[7] =        0;   mat[11] =           0;   mat[15] = 1;

	glPushMatrix();
	glTranslated(littleBirdPosition[0],littleBirdPosition[1],littleBirdPosition[2]);
	glMultMatrixd(mat);
	glRotated(pitch,0,0,1);
	glRotated(-roll,1,0,0);
	glRotated(strafe,1,0,0);
	helicopter(bladeRotation);
	glPopMatrix();
}

/*
 * This function is used to keep the helicopter moving
 * when certain values are greater than or less
 * than one.
 */
void timer(int value){
	littleBirdPosition[0] += directionVec->x*(speed/10.0);
	littleBirdPosition[1] += directionVec->y*(speed/10.0);
	littleBirdPosition[2] += directionVec->z*(speed/10.0);
	// Since yaw and pitch manipulate the direction vector
	// They cannot be used at the same time
	if(speed == 0){
		HelicopterYaw();
	}else{
		HelicopterRoll();
	}
	HelicopterStrafe();
	HelicopterFly();
	glutTimerFunc(50,timer,0);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 * This function draws the sky and terrain.
 */
void drawScene(){
	DrawSky();
	DrawLand();

	if(e==0){
		// Blades rotate in a circle 15 degrees at a time
		bladeRotation += 15;
		bladeRotation %= 360;
		DrawHelicopterFlight();
	}else{
		// e will be set to 1 if a collision is detected
		explosion += .1;
		glTranslated(littleBirdPosition[0],littleBirdPosition[1],littleBirdPosition[2]);
		explodedHelicopter(explosion);
	}
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	//  Erase the window and the depth buffer
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glLoadIdentity();

	double behindX = 10*directionVec->x;
	double behindY = 10*directionVec->y;
	double behindZ = 10*directionVec->z;

	double heightX = 3*upVec->x;
	double heightY = 3*upVec->y;
	double heightZ = 3*upVec->z;
	// Fly the helicopter around the scene
	gluLookAt(littleBirdPosition[0]-behindX+heightX,littleBirdPosition[1]-behindY+heightY,littleBirdPosition[2]-behindZ+heightZ,
			  littleBirdPosition[0],littleBirdPosition[1],littleBirdPosition[2],
			  upVec->x,upVec->y,upVec->z);

	/* Code for where the light source will be.
	 * The light source acts like a sun and is outside
	 * of the sky box.
	 */
	//  Translate intensity to color vectors
	float Ambient[]   = {0.01*30,0.01*30,0.01*30,1.0};
	float Diffuse[]   = {0.01*100,0.01*100,0.01*100,1.0};
	float Specular[]  = {0.0,0.0,0.0,1.0};
	//  Light position
	float Position[]  = {-225,400,0,1.0};
	//  Draw light position as ball (still no lighting here)
	glColor3f(1,1,1);
	glPushMatrix();
	glTranslatef(Position[0],Position[1],Position[2]);
	sphere(1.0, 1.0, .0, 1);
	glPopMatrix();
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
	
	glDisable(GL_LIGHTING);
	
	if(mode==0){
		const double len=2.0; // Length of axes
		// Pause music playback
		Mix_PauseMusic();
		// Blades rotate in a circle 15 degrees at a time
		bladeRotation += 15;
		bladeRotation %= 360;
		DrawHelicopterFlight();
		glColor3f(1,1,1);
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
	}else if(mode==1){
		DrawSky();
		// Blades rotate in a circle 15 degrees at a time
		bladeRotation += 15;
		bladeRotation %= 360;
		DrawHelicopterFlight();
	}else if(mode==2){
		drawScene();
	}else if(mode==3){
		checkCollision();
		drawScene();
	}else if(mode==4){
		ShootCannon();
		checkCollision();
		drawScene();
	}else if(mode==5){
		ShootCannon();
		checkCollision();
		drawScene();
		Mix_ResumeMusic();
	}
	
	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	//  Display parameters
	glColor3f(1,1,1);
	glWindowPos2i(5,5);
	Print("X=%f Y=%f Z=%f Mode=%d %s",littleBirdPosition[0],littleBirdPosition[1],littleBirdPosition[2],mode,text[mode]);
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
	init();
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 800x800
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800,800);
	glutCreateWindow("Robert Werthman Project");
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);

	// Load the textures for the helicopter
	littlebird[1] = LoadTexBMP("littlebirdenginetank.bmp");
	littlebird[2] = LoadTexBMP("littlebirdcockpit.bmp");
	littlebird[3] = LoadTexBMP("littlebirdengine.bmp");
	littlebird[4] = LoadTexBMP("littlebirdskidmount.bmp");
	littlebird[5] = LoadTexBMP("littlebirdskid.bmp");
	littlebird[6] = LoadTexBMP("littlebirdrotor.bmp");
	littlebird[7] = LoadTexBMP("littlebirdgear.bmp");

	// Load the textures for the sky
	sky[0] = LoadTexBMP("left.bmp");
	sky[1] = LoadTexBMP("right.bmp");
	sky[2] = LoadTexBMP("front.bmp");
	sky[3] = LoadTexBMP("back.bmp");
	sky[4] = LoadTexBMP("top.bmp");
	sky[5] = LoadTexBMP("bottom.bmp");

	// Load the textures for the trees
	tree[0] = LoadTexBMP("bark.bmp");
	tree[1] = LoadTexBMP("tree.bmp");

	// Load the textures for the buildings
	building[0] = LoadTexBMP("wall.bmp");
	building[1] = LoadTexBMP("ceiling.bmp");
	building[2] = LoadTexBMP("door.bmp");

	//  Check if any errors have occurred
	ErrCheck("init");
	timer(0);
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	//  Shut down SDL_mixer
	Mix_CloseAudio();
	return 0;
}


