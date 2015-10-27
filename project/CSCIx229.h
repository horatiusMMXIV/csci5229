#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
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
int  LoadOBJ(const char* file);
void special(int key,int x,int y);
void key(unsigned char ch,int x,int y);
void idle();
void reshape(int width,int height);

// Global Variables
extern int axes;       //  Display axes

extern double yaw;
extern double pitch;
extern double roll;
extern double strafe;
extern double fly;

extern int fov;       //  Field of view (for perspective)
extern int light;      //  Lighting
extern double asp;     //  Aspect ratio
extern double dim;   //  Size of world
// Light values
extern int distance;  // Light distance
extern int smooth;  // Smooth/Flat shading
extern int emission;  // Emission intensity (%)
extern int ambient;  // Ambient intensity (%)
extern int diffuse;  // Diffuse intensity (%)
extern int specular;  // Specular intensity (%)
extern int shininess;  // Shininess (power of two)
extern float shinyvec[1];    // Shininess (value)
extern int zh;  // Light azimuth
extern float ylight;  // Elevation of light

extern double cameraPos[3];
extern double cameraFront[3];
extern double cameraRight[3];
extern double cameraUp[3];

extern double littleBirdPosition[3];


#ifdef __cplusplus
}
#endif

#endif
