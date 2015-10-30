#include <math.h>

class Vector{
public:
	double x;
	double y;
	double z;
	Vector(double x, double y, double z);
	Vector dotProduct(Vector one, Vector two);
	Vector crossProduct(Vector three, Vector one, Vector two);
	Vector normalize(Vector one);
};
