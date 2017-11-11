#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Component.h"
#include "CompCamera.h"
#include "ResourceMesh.h"
#include "CompTransform.h"
#include "WindowInspector.h"
#include "ImGui/imgui.h"
#include "Geometry/Frustum.h"
#include <map>

#define ASPECT_RATIO 16/9

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	Start_enabled = true;
	Update_enabled = true;

	cam = new CompCamera(C_CAMERA, nullptr);
	cam->frustum.Translate(float3(10, 10,-10));
	LookAt(float3::zero);

	haveConfig = true;
	name = "Camera";
}

ModuleCamera3D::~ModuleCamera3D()
{
	RELEASE(cam);
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	perf_timer.Start();

	LOG("Setting up the camera");
	
	bool ret = true;

	//Send the renderer ehis cam to draw 
	App->renderer3D->SetSceneCamera(cam);
	App->renderer3D->SetActiveCamera(cam);

	Start_t = perf_timer.ReadMs();

	return ret;
}

bool ModuleCamera3D::Init(JSON_Object * node)
{
	//Load render config info -------
	move_speed = json_object_get_number(node, "Movement Speed");
	rotate_speed = json_object_get_number(node, "Rotation Speed");
	scroll_speed = json_object_get_number(node, "Zoom Speed");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	perf_timer.Start();

	if (App->engineState == EngineState::STOP) //Block camera movement while game is executing
	{
		ImGuiIO& io = ImGui::GetIO();

		//Check mouse if is out of the Scene Window
		CheckOut();

		// Only modify camera when it's possible
		if (io.WantTextInput == false && isMouseOnWindow() || canOut)
		{
			/* ------------------- CENTER CAMERA TO OBJECT ----------------- */
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
			{
				CenterToObject();
			}

			/* ------------------- MOUSE MOVEMENT ----------------- */
			int motion_x = App->input->GetMouseXMotion();
			int	motion_y = App->input->GetMouseYMotion();
			if ((motion_x != 0 || motion_y != 0))
			{
				MoveWithMouse(motion_x, motion_y, dt);
			}

			/* ------------------- WHEEL ZOOM ----------------- */
			if (App->input->GetMouseZ() != 0)
			{
				int zoom = App->input->GetMouseZ();
				Zoom(zoom * dt);
			}

			/* ------------------- KEYBOARD MOVEMENT ----------------- */
			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				MoveWithKeyboard(dt);
			}
		}

		Update_t = perf_timer.ReadMs();
	}
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::UpdateConfig(float dt)
{
	ImGui::BulletText("Camera Position"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.0f, 0.58f, 1.0f, 1.0f), "(%.2f, %.2f, %.2f)", cam->frustum.pos.x, cam->frustum.pos.x, cam->frustum.pos.x);
	ImGui::BulletText("Movement Speed"); ImGui::SameLine();
	ImGui::SliderFloat("##speedMove", &move_speed, 1.0f, 300.0f, "Speed = %.1f");
	ImGui::BulletText("Rotation Speed"); ImGui::SameLine();
	ImGui::SliderFloat("##speedRotate", &rotate_speed, 1.0f, 5.0f, "Speed = %.1f");
	ImGui::BulletText("Zoom Speed"); ImGui::SameLine();
	ImGui::SliderFloat("##speedScroll", &scroll_speed, 1.0f, 300.0f, "Speed = %.1f");

	return UPDATE_CONTINUE;
}

bool ModuleCamera3D::SaveConfig(JSON_Object * node)
{
	json_object_set_number(node, "Movement Speed", move_speed);
	json_object_set_number(node, "Rotation Speed", rotate_speed);
	json_object_set_number(node, "Zoom Speed", scroll_speed);
	return true;
}


void ModuleCamera3D::Orbit(float dx, float dy)
{
	float3 direction;

	if (focus != nullptr)
	{
		CompTransform* transform = (CompTransform*)focus->FindComponentByType(C_TRANSFORM);
		if (transform != nullptr)
		{
			point_to_look = transform->GetPos();
		}
		else
		{
			point_to_look = cam->frustum.pos + cam->frustum.front * 30.0f;
		}
	}

	else
	{
		point_to_look = cam->frustum.pos + cam->frustum.front * 30.0f;
	}

	direction = cam->frustum.pos - point_to_look;

	Quat quat_x(cam->frustum.WorldRight(), dy);
	Quat quat_y(cam->frustum.up, dx);
	direction = quat_x.Transform(direction);
	direction = quat_y.Transform(direction);

	cam->frustum.pos = direction + point_to_look;
	LookAt(point_to_look);
}

void ModuleCamera3D::LookAt(const float3& spot)
{
	cam->LookAt(spot);
}

void ModuleCamera3D::LookAround(float dx, float dy)
{
	// ROTATE HORIZONTAL --------------
	if (dx != 0.0f)
	{
		Quat quat = Quat::RotateY(dx); // Rotate around Y World Axis.
		cam->frustum.front = quat.Mul(cam->frustum.front).Normalized();
		cam->frustum.up = quat.Mul(cam->frustum.up).Normalized();
	}

	// ROTATE VERTICAL -------------------
	if (dy != 0.0f)
	{
		Quat quat = Quat::RotateAxisAngle(cam->frustum.WorldRight(), dy);
		float3 up_modified = quat.Mul(cam->frustum.up).Normalized();
		if (up_modified.y > 0.0f)
		{
			cam->frustum.up = up_modified;
			cam->frustum.front = quat.Mul(cam->frustum.front).Normalized();
		}
	}

}

void ModuleCamera3D::Zoom(float zoom)
{
	cam->frustum.pos += cam->frustum.front * zoom * scroll_speed;
	//LOG("Mouse Zoom (%f)", zoom);
}

void ModuleCamera3D::MousePick(float x, float y, float w, float h)
{
	possible_intersections.clear();

	norm_x = (2.0f * x) / w - 1.0f;
	norm_y = 1.0f - (2.0f * y) / h;

	// Generate camera ray
	ray = cam->frustum.UnProjectLineSegment(norm_x, norm_y);

	// Iterate all AABB of gameobjects
	entry_dist = 0.0f;
	exit_dist = ray.Length();

	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		// Check intersection ray-AABB
		CheckAABBIntersection(App->scene->gameobjects[i], entry_dist, exit_dist);
	}

	if (possible_intersections.size() > 0)
	{
		// Check intersection ray-traingles
		CheckGeometryIntersection();
	}
}

void ModuleCamera3D::CheckAABBIntersection(GameObject* candidate, float& entry_dist, float& exit_dist)
{
	if (candidate->isActive())
	{
		box = &candidate->box_fixed;
		if (box != nullptr)
		{
			bool hit = ray.Intersects(*box, entry_dist, exit_dist);
			if (hit)
			{
				// Set a list of possible intersections (sorted from closest to farthest)
				possible_intersections.insert(std::pair<float, GameObject*>(entry_dist, candidate));
			}
		}

		// Check intersection for childs
		for (uint i = 0; i < candidate->GetChildsVec().size(); i++)
		{
			CheckAABBIntersection(candidate->GetChildsVec()[i], entry_dist, exit_dist);
		}
	}
}

void ModuleCamera3D::CheckGeometryIntersection()
{
	// RESET VARIABLES
	hit = false;
	entry_dist = 0;
	hit_point = float3::zero;
	min_distance = INFINITY;
	ray_local_space = ray;
	best_candidate = nullptr;
	tris_map.clear();

	for (it = possible_intersections.begin(); it != possible_intersections.end(); ++it)
	{
		if (it->second->GetComponentTransform() != nullptr && it->second->FindComponentByType(C_MESH))
		{
			Triangle tri;
			const CompTransform* trans = it->second->GetComponentTransform();
			const CompMesh* mesh = (CompMesh*)it->second->FindComponentByType(C_MESH);

			// Transform ray coordinates into local space coordinates of the object
			float4x4 object_transform = it->second->GetComponentTransform()->GetGlobalTransform();
			ray_local_space.Transform(object_transform.Inverted());

			for (uint i = 0; i < mesh->resourceMesh->num_indices; i += 3)
			{
				// Set Triangle vertices
				tri.a = mesh->resourceMesh->vertices[mesh->resourceMesh->indices[i]].pos;
				tri.b = mesh->resourceMesh->vertices[mesh->resourceMesh->indices[i + 1]].pos;
				tri.c = mesh->resourceMesh->vertices[mesh->resourceMesh->indices[i + 2]].pos;
				hit = ray_local_space.Intersects(tri, &entry_dist, &hit_point);

				if (hit)
				{
					tris_map.insert(std::pair<float, Triangle>(entry_dist, tri));

					if (entry_dist < min_distance)
					{
						// Set the Game Objet to be picked
						min_distance = entry_dist;
						best_candidate = it->second;
					}
				}
			}
		}
	}

	if (best_candidate != nullptr)
	{
		//Set inspector window of this Game Object
		((Inspector*)App->gui->winManager[INSPECTOR])->LinkObject(best_candidate);
		App->camera->SetFocus(best_candidate);
	}
}


void ModuleCamera3D::SetFocus(const GameObject* selected)
{
	if (selected != nullptr)
	{
		focus = selected;
	}
}

void ModuleCamera3D::CenterToObject()
{
	if (focus != nullptr)
	{
		const AABB* box = &focus->box_fixed;
		float3 center = box->Centroid();
		float3 size = box->Size();
		cam->frustum.pos.Set(center.x + size.x, center.y + size.y, center.z + size.z);
		
		CompTransform* transform = (CompTransform*)focus->FindComponentByType(C_TRANSFORM);
		if (transform != nullptr)
		{
			point_to_look = transform->GetPos();
		}
		else
		{
			point_to_look = cam->frustum.pos + cam->frustum.front * 30.0f;
		}

		LookAt(point_to_look);
	}
}

bool ModuleCamera3D::isMouseOnWindow()
{
	return CanMoveCamera;
}

float * ModuleCamera3D::GetViewMatrix() const
{
	return cam->GetViewMatrix();
}

float * ModuleCamera3D::GetProjMatrix() const 
{
	return cam->GetProjectionMatrix();
}

float3 ModuleCamera3D::GetPos() const
{
	return cam->frustum.pos;
}

void ModuleCamera3D::CheckOut()
{
	if ((App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN ||
		App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN ||
		App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_DOWN) && isMouseOnWindow())
	{
		canOut = true;
	}
	if ((App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP ||
		App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP ||
		App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP) && isMouseOnWindow())
	{
		canOut = false;
	}
}

void ModuleCamera3D::MoveWithKeyboard(float dt)
{
	float final_speed = move_speed;
	cam_move.Set(0, 0, 0);

	// Increase cam velocity by pressing L-Shift
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		final_speed *= 5.0f;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		cam_move += cam->frustum.front;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		cam_move -= cam->frustum.WorldRight();
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		cam_move -= cam->frustum.front;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		cam_move += cam->frustum.WorldRight();
	}

	// If there is movement
	if (cam_move.LengthSq() > 0.0f)
	{
		cam->frustum.Translate(cam_move * final_speed * dt);
	}
}

void ModuleCamera3D::MoveWithMouse(int motion_x, int motion_y, float dt)
{
	// ORBIT
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT &&
		App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		float dx = (float)-motion_x * rotate_speed * dt;
		float dy = (float)-motion_y * rotate_speed * dt;

		Orbit(dx, dy);
	}

	// POINT
	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float dx = (float)-motion_x * rotate_speed * dt;
		float dy = (float)-motion_y * rotate_speed * dt;

		LookAround(dx, dy);
	}

	// TRANSLATE
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		float dx = (float)-motion_x * move_speed * dt;
		float dy = (float)motion_y * move_speed * dt;
		cam_move.Set(0, 0, 0);
		cam_move += cam->frustum.up * dy; // Move Axis Y of the camera
		cam_move -= cam->frustum.up.Cross(cam->frustum.front) * dx; // Move Axis X of the camera

		cam->frustum.Translate(cam_move);
	}
}