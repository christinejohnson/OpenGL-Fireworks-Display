#include "stdafx.h"
#include "ColourSystems.h"

RGBA operator + (const RGBA v1, const RGBA v2) {

	return RGBA(v1.r + v2.r, v1.g + v2.g, v1.b + v2.b, v1.a + v2.a);
}

RGBA operator - (const RGBA v1, const RGBA v2) {

	return RGBA(v1.r - v2.r, v1.g - v2.g, v1.b - v2.b, v1.a - v2.a);
}

RGBA operator * (const RGBA v1, const RGBA v2) {

	return RGBA(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b, v1.a * v2.a);
}

RGBA operator * (const RGBA v1, float amount) {

	return RGBA(v1.r * amount, v1.g * amount, v1.b * amount, v1.a * amount);
}

RGBA operator / (const RGBA v1, float amount) {

	return RGBA(v1.r / amount, v1.g / amount, v1.b / amount, v1.a / amount);
}

