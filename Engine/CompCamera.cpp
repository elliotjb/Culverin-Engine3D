#include "CompCamera.h"
#include "MathGeoLib.h"
#include "SDL\include\SDL_opengl.h"
#include <math.h>

CompCamera::CompCamera(Comp_Type t) :Component(t)
{
	/* Set camera vars*/
	width = 16;
	height = 9;
	aspect_ratio = width / height; // We set aspect ratio 16:9 by now

	near_plane = 0.2;
	far_plane = 500;
	vertical_fov = 60;

	/* Set frustum vars */
	cam_frustum.type = PerspectiveFrustum;
	cam_frustum.nearPlaneDistance = near_plane;
	cam_frustum.farPlaneDistance = far_plane;
	cam_frustum.verticalFov = vertical_fov;
	cam_frustum.horizontalFov = atan2(tan(vertical_fov / 2), aspect_ratio);

	/* Direction vectors (orthonormals) */
	cam_frustum.front.Set(0, -1, 0);
	cam_frustum.up.Set(0, 1, 0);
}

CompCamera::~CompCamera()
{
}

void CompCamera::Update()
{

}

void CompCamera::DebugDraw()
{
	glBegin(GL_LINES);
	glLineWidth(3.0f);
	glColor4f(0.25f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(cam_frustum.Edge(i).a.x, cam_frustum.Edge(i).a.y, cam_frustum.Edge(i).a.z);
		glVertex3f(cam_frustum.Edge(i).b.x, cam_frustum.Edge(i).b.y, cam_frustum.Edge(i).b.z);
	}

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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
	cam_frustum.horizontalFov = atan2(tan(vertical_fov / 2), aspect_ratio);
}
