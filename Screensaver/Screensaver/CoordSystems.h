#ifndef CoordSystemsH
#define CoordSystemsH

#include <math.h>
#include <time.h>
#include <random>

struct Vector3 {
	//a vector has x y and z components
	float x, y, z;

	//constructor to give Vector3.zero
	Vector3() {

		x = y = z = 0;
	}

	//user defined vector
	Vector3(float inX, float inY, float inZ) {

		x = inX;
		y = inY;
		z = inZ;
	}

	void normalise() {

		float length = sqrt((x * x) + (y * y) + (z * z));
		x /= length;
		y /= length;
		z /= length;
	}

	//returns a random vector with values ranging from 0 -> max
	void random(float max) {

		x = (float)rand()/((float)RAND_MAX/max);
		y = (float)rand()/((float)RAND_MAX/max);
		z = (float)rand()/((float)RAND_MAX/max);
	}
};

Vector3 operator + (const Vector3 v1, const Vector3 v2);
Vector3 operator - (const Vector3 v1, const Vector3 v2);
Vector3 operator / (const Vector3 v1, float amount);
Vector3 operator * (const Vector3 v1, float amount);
Vector3 operator * (const Vector3 v1, const Vector3 v2);

#endif