#ifndef CameraH
#define CameraH

#include "CoordSystems.h"

	enum ViewType {
		kPerspective, kOrthagraphic
	};

class Camera {

public:
	//constructors and methods
	Camera::Camera(ViewType viewType, float depth, float fov);
	void setDepthTo(float depth);
	void setFOVTo(float fov);
	void modifyPositionBy(Vector3 modifier);
	void modifyRotationBy(Vector3 angles);
	void moveGLView();
	void initView(ViewType type);


	//variables
public:
	Vector3 Position;
	Vector3 Rotation;
	float Angle;

private:
	ViewType _viewType;
	float _fov;
	float _depth;
	Vector3 _lookAt;
	Vector3 _upVector;
};

#endif