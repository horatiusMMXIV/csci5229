#include <math.h>

class Vector{
public:
	double x;
	double y;
	double z;
	Vector(double x, double y, double z);
	void dotProduct(Vector* one, Vector* two);
	void crossProduct(Vector* one, Vector* two);
	void normalize();
};
