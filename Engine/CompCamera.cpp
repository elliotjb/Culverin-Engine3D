#include "CompCamera.h"
#include "CompMesh.h"
#include "CompTransform.h"
#include "MathGeoLib.h"
#include "Application.h"
#include "Scene.h"
#include "ModuleFS.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"

#include "SDL\include\SDL_opengl.h"
#include <math.h>

CompCamera::CompCamera(Comp_Type t, GameObject* parent) : Component(t, parent)
{
	nameComponent = "Camera";
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
	if (culling && App->engineState != EngineState::STOP)
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
	// Only draw frustums on editor mode, not in game mode
	if (App->engineState == EngineState::STOP)
	{
		DebugDraw();
	}
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

void CompCamera::ShowOptions()
{
	//ImGui::MenuItem("CREATE", NULL, false, false);
	if (ImGui::MenuItem("Reset"))
	{
		// Not implmented yet.
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Move to Front", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Move to Back", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Remove Component"))
	{
		toDelete = true;
	}
	if (ImGui::MenuItem("Move Up", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Move Down", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Copy Component"))
	{
		// Component* Copy = this;
	}
	if (ImGui::MenuItem("Paste Component", NULL, false, false))
	{
		//parent->AddComponent(App->scene->copyComponent->GetType())
		// Create contructor Component Copy or add funtion to add info
	}
}

void CompCamera::ShowInspectorInfo()
{
	// Reset Values Button -------------------------------------------
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 0));
	ImGui::SameLine(ImGui::GetWindowWidth() - 26);
	if (ImGui::ImageButton((ImTextureID*)App->scene->icon_options_transform, ImVec2(13, 13), ImVec2(-1, 1), ImVec2(0, 0)))
	{
		// Open Options
	}
	ImGui::PopStyleVar();

	if (ImGui::Checkbox("Main Camera", &is_main))
	{
		SetMain(is_main);
	}

	if (showPopup)
	{
		ShowCameraPopup();
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

void CompCamera::ShowCameraPopup()
{
	ImGui::OpenPopup("Main Camera Problem");
	if (ImGui::BeginPopupModal("Main Camera Problem", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("There is an active Game Camera already.");
		float width = ImGui::GetItemRectSize().x * 0.5;
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text(""); ImGui::SameLine(width - 20);
		if (ImGui::Button("OK", ImVec2(40, 20)))
		{
			showPopup = false;
		}
	}
	ImGui::EndPopup();
}

void CompCamera::DoCulling()
{
	// First check culling with static objects (optimized with quadtree)
	CullStaticObjects();

	// Then check dynamic objects
	CullDynamicObjects();
}

void CompCamera::CullStaticObjects()
{
	// First, set all static objects invisible
	for (uint i = 0; i < App->scene->static_objects.size(); i++)
	{
		App->scene->static_objects[i]->SetVisible(false);
	}

	// Get all static objects that are inside the frustum (accelerated with quadtree)
	App->scene->quadtree.CollectCandidates(candidates_to_cull, frustum);

	// Set visible only these static objects
	while (!candidates_to_cull.empty())
	{
		candidates_to_cull.front()->SetVisible(true); // INSIDE CAMERA VISION
		candidates_to_cull.pop();
	}
}

void CompCamera::CullDynamicObjects()
{
	AABB* box = nullptr;

	// Push all active elements that are root & active
	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		if (App->scene->gameobjects[i]->isActive())
		{
			candidates_to_cull.push(App->scene->gameobjects[i]);
		}
	}

	// Check candidates_to_cull vector until it's empty
	while (candidates_to_cull.empty() == false)
	{
		// If it's not static, check if it's inside the vision of the camera to set 
		if (!candidates_to_cull.front()->isStatic())
		{
			if (candidates_to_cull.front()->bounding_box != nullptr) // Check if it has AABB and it's not the camera itself
			{
				box = &candidates_to_cull.front()->box_fixed;
				if (ContainsAABox(*box) == CULL_OUT)
				{
					candidates_to_cull.front()->SetVisible(false); // OUTSIDE CAMERA VISION
				}
				else
				{
					candidates_to_cull.front()->SetVisible(true); // INSIDE CAMERA VISION
				}
			}
		}

		//Push all the active childs to the candidates vector
		for (std::vector<GameObject*>::iterator it = candidates_to_cull.front()->GetChildsPtr()->begin(); it != candidates_to_cull.front()->GetChildsPtr()->end(); it++)
		{
			if ((*it)->isActive())
			{
				candidates_to_cull.push((*it));
			}
		}

		// Delete from vector the object already checked
		candidates_to_cull.pop();
	}
}

void CompCamera::UnCull()
{
	// Push all active elements that are root & active
	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		if (App->scene->gameobjects[i]->isActive())
		{
			candidates_to_cull.push(App->scene->gameobjects[i]);
		}
	}

	// Check candidates_to_cull vector until it's empty
	while (candidates_to_cull.empty() == false)
	{
		candidates_to_cull.front()->SetVisible(true);

		//Push all childs that are active to the candidates vector
		for (std::vector<GameObject*>::iterator it = candidates_to_cull.front()->GetChildsVec().begin(); it != candidates_to_cull.front()->GetChildsVec().end(); it++)
		{
			if ((*it)->isActive())
			{
				candidates_to_cull.push((*it));
			}
		}

		// Delete from vector the object already checked
		candidates_to_cull.pop();
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
		if (App->renderer3D->game_camera == nullptr)
		{
			App->renderer3D->SetGameCamera(this);
			is_main = true;
		}
		else
		{
			// Enable Pop Up of the camera
			showPopup = true;
			is_main = false;
		}
	}
	else 
	{
		App->renderer3D->SetGameCamera(nullptr);
		is_main = false;
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

void CompCamera::Save(JSON_Object * object, std::string name, bool saveScene, uint& countResources) const
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
