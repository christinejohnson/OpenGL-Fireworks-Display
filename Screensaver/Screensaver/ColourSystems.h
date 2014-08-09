#include <math.h>
#include <time.h>
#include <random>

struct RGBA {

	float r;	//The red colour component
	float g;	//The green colour component
	float b;	//The blue colour component
	float a;	//The red colour component

	RGBA() {

		r = g = b = a = 0;
	}

	RGBA(float red, float green, float blue, float alpha) {

		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	//Interpolates between 2 RGBAs
	RGBA(RGBA start, RGBA end, float time) {

		r = start.r + (end.r -  start.r) * time;
		g = start.g + (end.g -  start.g) * time;
		b = start.b + (end.b -  start.b) * time;
		a = start.a + (end.a -  start.a) * time;
	}

	//returns a random colour
	void random() {

		r = (float)rand()/((float)RAND_MAX);
		g = (float)rand()/((float)RAND_MAX);
		b = (float)rand()/((float)RAND_MAX);
		a = (float)rand()/((float)RAND_MAX);
	}
};

RGBA operator - (const RGBA a, const RGBA b);
RGBA operator + (const RGBA a, const RGBA b);
RGBA operator * (const RGBA v1, const RGBA v2);
RGBA operator * (const RGBA a, float amount);
RGBA operator / (const RGBA a, float amount);