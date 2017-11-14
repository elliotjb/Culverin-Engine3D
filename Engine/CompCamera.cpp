#include "CompCamera.h"
#include "CompMesh.h"
#include "CompTransform.h"
#include "MathGeoLib.h"
#include "Application.h"
#include "Scene.h"
#include "GameObject.h"

#include "SDL\include\SDL_opengl.h"
#include <math.h>

CompCamera::CompCamera(Comp_Type t, GameObject* parent) : Component(t, parent)
{
	//uid = App->random->Int();
	/* Set camera vars*/
	width = 16;
	height = 9;
	aspect_ratio = width / height; // We set aspect ratio 16:9 by now

	near_plane = 0.2;
	far_plane = 1000;
	vertical_fov = 60; /* In degrees */

	/* Set frustum vars */
	frustum.type = PerspectiveFrustum;
	frustum.pos.Set(0, 0, 0);
	frustum.front.Set(0, 0, 1);
	frustum.up.Set(0, 1, 0);

	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;
	frustum.verticalFov = vertical_fov * DEGTORAD;
	frustum.horizontalFov = Atan(aspect_ratio*Tan(frustum.verticalFov / 2)) * 2;
}

CompCamera::~CompCamera()
{
}

void CompCamera::preUpdate(float dt)
{
	if (culling)
	{
		// Iterate All GameObjects and apply culling
		DoCulling();
	}
}

void CompCamera::Update(float dt)
{
	UpdateFrustum();
}

void CompCamera::UpdateFrustum()
{
	const CompTransform* transform = parent->GetComponentTransform();
	
	float4x4 trans = transform->GetGlobalTransform();

	frustum.pos = trans.TranslatePart();
	frustum.front = trans.WorldZ();
	frustum.up = frustum.front.Cross(-frustum.WorldRight());
}

void CompCamera::Draw()
{
	// if(parent->selected == true)
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
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
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

		if (ImGui::Checkbox("Main Camera", &is_main))
		{
			SetMain(is_main);
		}

		if (ImGui::Checkbox("Culling", &culling))
		{
			if (!culling)
			{
				UnCull();
			}
		}

		ImGui::PushItemWidth(80);
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

void CompCamera::DoCulling()
{
	AABB* box = nullptr;
	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		if (App->scene->gameobjects[i]->isActive() && App->scene->gameobjects[i] != parent)
		{
			// Check its bounding box
			box = &App->scene->gameobjects[i]->box_fixed;
			if (box != nullptr)
			{
				// We set visible varialbe True / False to know if the GameObject will be drawn(or not) in renderer PostUpdate
				if (ContainsAABox(*box) == CULL_OUT)
				{
					App->scene->gameobjects[i]->SetVisible(false);
				}
				else
				{
					App->scene->gameobjects[i]->SetVisible(true);
				}
			}
		}
	}
}

void CompCamera::UnCull()
{
	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		if (App->scene->gameobjects[i]->isActive())
		{
			App->scene->gameobjects[i]->SetVisible(true);
		}
	}
}

void CompCamera::LookAt(const float3& position)
{
	float3 direction = position - frustum.pos;
	float3x3 matrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3(0, 1, 0));

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();
}

Culling CompCamera::ContainsAABox(const AABB& refBox) const
{
	float3 corner[8];
	int totalIn = 0;
	refBox.GetCornerPoints(corner); // get the corners of the box into corner array
	
	// Test all 8 corners against the 6 sides of the frustum.
	// If all of them are behinf one specific plane = OUT.
	// If one of them is inside all the planes = IN or INTERSECT
	for (uint p = 0; p < 6; p++)
	{
		int In_count = 8;
		int point_In = 1;

		for(uint i = 0; i < 8; i++)
		{
			//Test all points against plane[p]
			if (frustum.GetPlane(p).IsOnPositiveSide(corner[i]))
			{
				// The point is behind the plane
				point_In = 0;
				In_count--;
			}
		}

		// If all points are out of the plane
		if (In_count == 0)
		{
			return CULL_OUT;
		}

		// Sum all points that are inside of the planes
		totalIn += point_In;
	}

	if (totalIn == 6)
	{
		return CULL_IN;
	}
	

	return CULL_INTERSECT;
}

void CompCamera::SetMain(bool isMain)
{
	if (isMain)
	{
		App->renderer3D->SetGameCamera(this);
	}
	else
	{
		App->renderer3D->SetGameCamera(nullptr);
	}
}

void CompCamera::SetPos(float3 pos)
{
	frustum.pos = pos;
}

void CompCamera::SetNear(float near_p)
{
	frustum.nearPlaneDistance = near_p;
}

void CompCamera::SetFar(float far_p)
{
	frustum.farPlaneDistance = far_p;
}

void CompCamera::SetFov(float vertical)
{
	frustum.verticalFov = vertical * DEGTORAD;
	frustum.horizontalFov = Atan(aspect_ratio*Tan(frustum.verticalFov / 2)) * 2;
}

void CompCamera::SetRatio(float ratio)
{
	aspect_ratio = ratio;
	frustum.horizontalFov = Atan(aspect_ratio*Tan(frustum.verticalFov / 2)) * 2;
}

float CompCamera::GetNear() const
{
	return frustum.nearPlaneDistance;
}

float CompCamera::GetFar() const
{
	return frustum.farPlaneDistance;
}

float CompCamera::GetFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

float CompCamera::GetRatio() const
{
	return frustum.AspectRatio();
}

float* CompCamera::GetViewMatrix() const
{
	static float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

float* CompCamera::GetProjectionMatrix() const
{
	static float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

void CompCamera::Save(JSON_Object * object, std::string name) const
{
	// TRANSFORM-----------
	json_object_dotset_number_with_std(object, name + "Type", C_CAMERA);
	// Position
	App->fs->json_array_dotset_float3(object, name + "Position", frustum.pos);
	App->fs->json_array_dotset_float3(object, name + "Front", frustum.front);
	App->fs->json_array_dotset_float3(object, name + "Up", frustum.up);

	json_object_dotset_number_with_std(object, name + "Near Plane", frustum.nearPlaneDistance);
	json_object_dotset_number_with_std(object, name + "Far Plane", frustum.farPlaneDistance);
	json_object_dotset_number_with_std(object, name + "Vertical Pov", frustum.verticalFov);

	json_object_dotset_boolean_with_std(object, name + "Main Camera", is_main);
}

void CompCamera::Load(const JSON_Object * object, std::string name)
{
	frustum.pos = App->fs->json_array_dotget_float3_string(object, name + "Position");
	frustum.front = App->fs->json_array_dotget_float3_string(object, name + "Front");
	frustum.up = App->fs->json_array_dotget_float3_string(object, name + "Up");
	frustum.nearPlaneDistance = json_object_dotget_number_with_std(object, name + "Near Plane");
	frustum.farPlaneDistance = json_object_dotget_number_with_std(object, name + "Far Plane");
	frustum.verticalFov = json_object_dotget_number_with_std(object, name + "Vertical Pov");

	near_plane = frustum.nearPlaneDistance;
	far_plane = frustum.farPlaneDistance;
	vertical_fov = frustum.verticalFov;

	is_main = json_object_dotget_boolean_with_std(object, name + "Main Camera");
	SetMain(is_main);

	Enable();
}
