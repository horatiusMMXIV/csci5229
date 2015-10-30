#include "Vector.h"

Vector::Vector(double x, double y, double z){
	this->x=x;
	this->y=y;
	this->z=z;
}

Vector Vector::dotProduct(Vector one, Vector two){

}

Vector Vector::crossProduct(Vector three, Vector one, Vector two){
	three.x = one.y*two.z-one.z*two.y;
	three.y = one.z*two.x-one.x*two.z;
	three.z = one.x*two.y-one.y*two.x;
	return three;
}

Vector Vector::normalize(Vector one){
	double vectorLength = 0;
	vectorLength = sqrt(one.x*one.x+one.y*one.y+one.z*one.z);
	one.x = one.x/vectorLength;
	one.y = one.y/vectorLength;
	one.z = one.z/vectorLength;
	return one;
}
