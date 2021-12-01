#include "Camera.h"
#include <math.h>

using namespace std;

//initial camera view
Camera::Camera()
{
	this->eye = Vec3D(250,70,250);
	this->lookAt = Vec3D(0,30,0);
	this->up = Vec3D(0, 1, 0);
	this->fovY = 80.0;
}

void Camera::leftRotate()
{

	if (eye.dx >= 20) {
        eye.dx -= 3;
	}
}

void Camera::rightRotate()
{
	if (eye.dx <= 250)
        eye.dx += 3;
}

void Camera::zoomIn()
{
	if (fovY >= 10.0) {
		fovY = fovY - 3.0;
	}
}

void Camera::zoomOut()
{
	if (fovY <= 90.0) {
		fovY = fovY + 3.0;
	}
}