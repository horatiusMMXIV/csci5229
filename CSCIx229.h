#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
//#include "SDL.h"
#include "SDL_mixer.h"
#else
#include <GL/glut.h>
//#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
void special(int key,int x,int y);
void key(unsigned char ch,int x,int y);
void reshape(int width,int height);

void HelicopterRoll();
void HelicopterYaw();
void HelicopterFly();
void HelicopterStrafe();
void DrawTree();
void DrawBuilding();
void checkCollision();
void timer(int value);
void DrawSky();
void DrawLand();
void ShootCannon();
void CreateBullet();

// Global Variables
#define MODE 6
extern int mode;
extern int yaw;
extern int pitch;
extern int roll;
extern int strafe;
extern int fly;
extern int e;
extern double explosion;
extern int flight;
extern int bankAngle;
extern int bankFactor;
extern int speed;
extern int fov;       //  Field of view (for perspective)
extern int light;      //  Lighting
extern double asp;     //  Aspect ratio
extern double dim;   //  Size of world
extern double littleBirdPosition[3];
extern double bullets[10][7];
extern int bulletIndex;
extern Mix_Chunk *shot;
extern Mix_Chunk *bomb;


#ifdef __cplusplus
}
#endif

#endif
