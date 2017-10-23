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
	far_plane = 10;
	vertical_fov = 60; /* In degrees */

	/* Set frustum vars */
	cam_frustum.type = PerspectiveFrustum;
	cam_frustum.front.Set(0, 0, -1);
	cam_frustum.up.Set(0, 1, 0);

	cam_frustum.nearPlaneDistance = near_plane;
	cam_frustum.farPlaneDistance = far_plane;
	cam_frustum.verticalFov = vertical_fov * DEGTORAD;
	cam_frustum.horizontalFov = Atan(aspect_ratio*Tan(cam_frustum.verticalFov / 2)) * 2;
}

CompCamera::~CompCamera()
{
}

void CompCamera::Init(float3 pos)
{
	cam_frustum.pos = pos;
}

void CompCamera::Update()
{
	DebugDraw();
}

void CompCamera::DebugDraw()
{
	glBindTexture(GL_TEXTURE_2D, 0);
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
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleColor();
		ImGui::PushItemWidth(60);
		if (ImGui::DragFloat("Near Plane", &near_plane, 0.5f, 0.01f, far_plane - 0.01f))
		{
			SetNear(near_plane);
		}
		if (ImGui::DragFloat("Far Plane", &far_plane, 0.5f, near_plane + 0.01f, 1000.0f))
		{
			SetFar(far_plane);
		}
		if (ImGui::SliderFloat("FOV", &vertical_fov, 1, 179))
		{
			SetFov(vertical_fov);
		}

		ImGui::PopItemWidth();
		ImGui::TreePop();

	}
	else
	{
		ImGui::PopStyleColor();
	}
}

void CompCamera::SetPos(float3 pos)
{
	cam_frustum.pos = pos;
}

void CompCamera::SetNear(float near_p)
{
	cam_frustum.nearPlaneDistance = near_p;
}

void CompCamera::SetFar(float far_p)
{
	cam_frustum.farPlaneDistance = far_p;
}

void CompCamera::SetFov(float vertical)
{
	cam_frustum.verticalFov = vertical * DEGTORAD;
	cam_frustum.horizontalFov = Atan(aspect_ratio*Tan(cam_frustum.verticalFov / 2)) * 2;
}
