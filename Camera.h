#ifndef CAMERA_H
#define CAMERA_H

#include "mathLib3D.h"

class Camera {
	public:
		Camera();
		Vec3D eye;
		Vec3D lookAt;
		Vec3D up;
		float fovY;
		void leftRotate();
		void rightRotate();
		void zoomIn();
		void zoomOut();
};

#endif