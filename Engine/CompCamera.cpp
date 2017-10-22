#include "CompCamera.h"
#include <math.h>

CompCamera::CompCamera(Comp_Type t) :Component(t)
{
	/* Set camera vars*/
	width = 16;
	height = 9;
	aspect_ratio = width / height;

	near_plane = 0.2;
	far_plane = 500;
	vertical_fov = 60;

	/* Set frustum vars */
	cam_frustum.type = PerspectiveFrustum;
	cam_frustum.nearPlaneDistance = near_plane;
	cam_frustum.farPlaneDistance = far_plane;
	cam_frustum.verticalFov = vertical_fov;
	cam_frustum.horizontalFov = atan2(tan(vertical_fov / 2), aspect_ratio);
}

CompCamera::~CompCamera()
{
}

void CompCamera::Update()
{

}

void CompCamera::DebugDraw()
{

}

void CompCamera::ShowInspectorInfo()
{
}

void CompCamera::SetNear(float near_p)
{
	near_plane = near_p;
	cam_frustum.nearPlaneDistance = near_p;
}

void CompCamera::SetFar(float far_p)
{
	far_plane = far_p;
	cam_frustum.farPlaneDistance = far_p;
}

void CompCamera::SetFov(float vertical)
{
	vertical_fov = vertical;
	cam_frustum.horizontalFov = atan2(tan(vertical_fov / 2), ASPECT_RATIO);
}
