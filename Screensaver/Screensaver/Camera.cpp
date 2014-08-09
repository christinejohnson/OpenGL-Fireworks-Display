#include "stdafx.h"

#include "Camera.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#include <gl\glut.h>
#include <gl\GL.h>
#include <list>
#include <string>
#include <vector>
#include <time.h>

//inits a new camera to alter the opengl view
Camera::Camera(ViewType viewType, float depth, float fov) {

	_viewType = viewType;
	_fov = fov;
	_depth = depth;

	Rotation = Vector3(0, 0, 0);
	Position = Vector3(0, 1, 0);
	_lookAt = Vector3(0, 0, -1);
	_upVector = Vector3(0, 1, 0);

	initView(viewType);
}

//modifys the camera's depth
void Camera::setDepthTo(float depth) {

	_depth = depth;
	initView(_viewType);
}

//modifys the camera's fov
void Camera::setFOVTo(float fov) {

	_fov = fov;
	initView(_viewType);
}

//inits the appropiate opengl view according to the view type
void Camera::initView(ViewType type) {

	_viewType = type;

	switch (_viewType) {

	case kPerspective: 
		{
			gluPerspective(_fov, GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN), 0, _depth);

		}
		break;

	case kOrthagraphic:
		{
			glOrtho(0, 1.0, 0, 1.0, -1, _depth);
		}
		break;
	}
}

//move the cameras position in the world by the parameter
void Camera::modifyPositionBy(Vector3 modifier) {

	//when moving forwards and backwards move relative to where camera is looking
	Position.x += modifier.z * _lookAt.x;
	Position.z += modifier.z * _lookAt.z;
	Position.y += modifier.z * _lookAt.y;

	//when strafing left or right, take into account current camera rotation
	Position.x += modifier.x * cos(Rotation.y);
	Position.z += modifier.x * sin(Rotation.y);
}

//rotate the camera axis' by angles dicated by the parameter
void Camera::modifyRotationBy(Vector3 angles) {

	//update the camera rotations
	Rotation.x += angles.x;
	Rotation.y += angles.y;
	Rotation.z += angles.z;

	//change our lookAt#s left/right facing vector components to look at the new rotation along the y axis
	_lookAt.x = sin(Rotation.y);
	_lookAt.z = -cos(Rotation.y);

	//change the up/down facing to match the new rotation
	_lookAt.y = Rotation.x;
}

//moves the view to match that of the camera's position and look to
void Camera::moveGLView() {

	//set camera position and orientation
	gluLookAt(Position.x, Position.y, Position.z, 
		Position.x + _lookAt.x, Position.y + _lookAt.y, Position.z + _lookAt.z,
		_upVector.x, _upVector.y, _upVector.z);
}