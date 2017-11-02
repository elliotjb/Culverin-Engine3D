#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "PhysBody3D.h"
#include "GameObject.h"
#include "Component.h"
#include "CompCamera.h"
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

	scroll_speed = 30.0f;
	move_speed = 1.0f;
	rotate_speed = 1.0f;

	//Send the renderer ehis cam to draw 
	App->renderer3D->SetActiveCamera(cam);

	Start_t = perf_timer.ReadMs();

	return ret;
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

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::UpdateConfig(float dt)
{
	ImGui::BulletText("Camera Position"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.0f, 0.58f, 1.0f, 1.0f), "(%.2f, %.2f, %.2f)", cam->frustum.pos.x, cam->frustum.pos.x, cam->frustum.pos.x);
	ImGui::BulletText("Scroll Speed"); ImGui::SameLine();
	ImGui::SliderFloat("##speedScroll", &scroll_speed, 0.0f, 300.0f, "Speed = %.1f");

	return UPDATE_CONTINUE;
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

	float norm_x = (2.0f * x) / w - 1.0f;
	float norm_y = 1.0f - (2.0f * y) / h;

	// Generate camera ray
	ray = cam->frustum.UnProjectLineSegment(norm_x, norm_y);

	// Iterate all AABB of gameobjects
	bool hit = false;
	float entry_dist = 0.0f;
	float3 hit_point = float3::zero;
	float exit_dist = ray.Length();

	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		if (App->scene->gameobjects[i]->isActive())
		{
			const AABB* box = App->scene->gameobjects[i]->bounding_box;
			if (box != nullptr)
			{
				hit = ray.Intersects(*box, entry_dist, exit_dist);
				if (hit)
				{
					// Set a list of possible intersections (sorted from closest to farthest)
					possible_intersections.insert(std::pair<float, GameObject*>(entry_dist, App->scene->gameobjects[i]));
				}
			}
		}
	}

	if (possible_intersections.size() > 0)
	{
		float min_distance = INFINITY;
		LineSegment ray_local_space = ray;
		GameObject* best_candidate = nullptr;
		std::map<float, Triangle> tris_map;
		std::map<float, GameObject*>::iterator it;

		for (it = possible_intersections.begin(); it != possible_intersections.end(); ++it)
		{
			if (it->second->GetComponentTransform() != nullptr && it->second->FindComponentByType(C_MESH))
			{
				Triangle tri;
				const CompTransform* trans = it->second->GetComponentTransform();
				const CompMesh* mesh = (CompMesh*)it->second->FindComponentByType(C_MESH);

				// Transform ray coordinates into local space coordinates of the object
				float4x4 object_transform = it->second->GetComponentTransform()->GetTransform();
				ray_local_space.Transform(object_transform.Transposed());

				for (uint i = 0; i < mesh->num_indices; i += 3)
				{
					// Set Triangle vertices
					tri.a = mesh->vertices[mesh->indices[i]].pos;
					tri.b = mesh->vertices[mesh->indices[i + 1]].pos;
					tri.c = mesh->vertices[mesh->indices[i + 2]].pos;
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
}

float3 ModuleCamera3D::IntersectionPoint(const AABB* box)
{
	float3 point = float3::zero;

	return point;
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
		AABB* box = focus->bounding_box;
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
}