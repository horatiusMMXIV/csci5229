#include "Vector.h"

Vector::Vector(double x, double y, double z){
	this->x=x;
	this->y=y;
	this->z=z;
}

void Vector::dotProduct(Vector* one, Vector* two){

}

void Vector::crossProduct(Vector* one, Vector* two){
	this->x = one->y*two->z-one->z*two->y;
	this->y = one->z*two->x-one->x*two->z;
	this->z = one->x*two->y-one->y*two->x;
}

void Vector::normalize(){
	double vectorLength = 0;
	vectorLength = sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
	this->x = this->x/vectorLength;
	this->y = this->y/vectorLength;
	this->z = this->z/vectorLength;
}
