#include "stdafx.h"

#include "CoordSystems.h"

Vector3 operator + (const Vector3 v1, const Vector3 v2) {

	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator - (const Vector3 v1, const Vector3 v2) {

	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 operator / (const Vector3 v1, float amount) {

	return Vector3(v1.x / amount, v1.y / amount, v1.z / amount);
}

Vector3 operator * (const Vector3 v1, float amount) {

	return Vector3(v1.x * amount, v1.y * amount, v1.z * amount);
}

Vector3 operator * (const Vector3 v1, const Vector3 v2) {

	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}