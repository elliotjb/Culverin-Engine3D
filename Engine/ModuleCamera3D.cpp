#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "PhysBody3D.h"
#include "GameObject.h"
#include "Component.h"
#include "CompCamera.h"
#include "CompTransform.h"
#include "ImGui/imgui.h"
#include "Geometry/Frustum.h"

#define ASPECT_RATIO 16/9

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	Start_enabled = true;
	Update_enabled = true;

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 3.0f, 10.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	cam = new CompCamera(C_CAMERA, nullptr);

	name = "Camera";
	haveConfig = true;
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
update_status ModuleCamera3D::UpdateNew(float dt)
{
	perf_timer.Start();

	ImGuiIO& io = ImGui::GetIO();

	//Check mouse if is out of the Scene Window
	CheckOut();

	// Only modify camera when it's possible
	if (io.WantTextInput == false && isMouseOnWindow() || canOut)
	{
		/* Keyboard movement */
		if (/*Keyboard()*/1)
		{
			MoveWithKeyboard(dt);
		}

		/* Mouse movement */
		if (/*Mouse()*/1)
		{
			int motion_x = App->input->GetMouseXMotion();
			int	motion_y = App->input->GetMouseYMotion();

			if ((motion_x != 0 || motion_y != 0) )
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
				else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT &&
					App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
				{
					float dx = (float)-motion_x * rotate_speed * dt;
					float dy = (float)-motion_y * rotate_speed * dt;

					Look(dx, dy);

				}
			}
			

		}
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	perf_timer.Start();

	ImGuiIO& io = ImGui::GetIO();

	//Check out mouse
	CheckOut();

	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	if (io.WantTextInput == false && isMouseOnWindow() || canOut)
	{
		vec3 newPos(0, 0, 0);
		float speed = 3.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 28.0f * dt;
		// Mouse motion ----------------
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
		{
			{
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				float Sensitivity = 0.25f;

				//Position -= Reference;
				if (dx != 0)
				{
					float DeltaX = (float)dx * Sensitivity;

					X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				}

				if (dy != 0)
				{
					float DeltaY = (float)dy * Sensitivity;

					Y = rotate(Y, DeltaY, X);
					Z = rotate(Z, DeltaY, X);

					if (Y.y < 0.0f)
					{
						Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						Y = cross(Z, X);
					}
				}
				//Position = Reference + Z * length(Position);
			}
			//Mode Camera with keyboard
			if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT)
			{
				needReajust = true;
				newPos.y += speed;
			}
			if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
			{
				needReajust = true;
				newPos.y -= speed;
			}

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				needReajust = true;
				newPos -= Z * speed;
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				needReajust = true;
				newPos += Z * speed;
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				needReajust = true;
				newPos -= X * speed;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				needReajust = true;
				newPos += X * speed;
			}
		}
		
		//Set position and view camera to object in scene (Like Unity...)
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			LookAndMoveToObject();
		}

		//Camera Zoom ---------------------------
		if (App->input->GetMouseZ() == 1)
		{
			newPos -= Z * (scroll_speed * dt);
			needReajust = true;

		}
		if (App->input->GetMouseZ() == -1)
		{
			newPos += Z * (scroll_speed * dt);
			needReajust = true;
		} 
		// -------------------------------------
		//Mouse Middle ------------------------
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
		{
			if (App->input->GetMouseXMotion() > 0)
			{
				newPos -= X * (App->input->GetMouseXMotion() * dt) * move_speed;
			}
			if (App->input->GetMouseXMotion() < 0)
			{
				newPos -= X * (App->input->GetMouseXMotion() * dt) * move_speed;
			}
			if (App->input->GetMouseYMotion() > 0)
			{
				newPos += Y * (App->input->GetMouseYMotion() * dt) * move_speed;
			}
			if (App->input->GetMouseYMotion() < 0)
			{
				newPos += Y * (App->input->GetMouseYMotion() * dt) * move_speed;
			}			
		}
		// -------------------------------


		Position += newPos;
		Reference += newPos;

		//Rotate camera by object
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			if (needReajust)
			{
				ReajustLook();
				needReajust = false;
			}
			//BaseGeometry* geo = App->geometry_manager->geometry;
			//if (geo != NULL)
			//{
			//	AABB* box = &((_Model*)(App->geometry_manager->geometry))->GetBoundingBox();
			//	float3 center = box->Centroid();
			//	LookObject(vec3(center.x, center.y, center.z));
			//}
			//else
			//	LookObject(vec3(0, 0, 0));
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);

			Rotate(Reference.x, Reference.y);
		}
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera3D::UpdateConfig(float dt)
{
	ImGui::BulletText("Camera Position"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.0f, 0.58f, 1.0f, 1.0f), "(%.2f, %.2f, %.2f)", Position.x, Position.y, Position.z);
	ImGui::BulletText("Scroll Speed"); ImGui::SameLine();
	ImGui::SliderFloat("##speedScroll", &scroll_speed, 0.0f, 300.0f, "Speed = %.1f");

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::Orbit(float dx, float dy)
{
	float3 point_to_look;
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

	Quat quat_y(cam->frustum.up, dx);
	Quat quat_x(cam->frustum.WorldRight(), dy);

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
		Quat quat;
		quat.RotateY(dx); // Rotate around Y World Axis.
		cam->frustum.front = quat.Mul(cam->frustum.front).Normalized();
		cam->frustum.up = quat.Mul(cam->frustum.up).Normalized();
	}

	// ROTATE VERTICAL -------------------
	if (dy != 0.0f)
	{
		Quat q;
		q.RotateAxisAngle(cam->frustum.WorldRight(), dy);
		float3 up_modified = q.Mul(cam->frustum.up).Normalized();

		if (up_modified.y > 0.0f)
		{
			cam->frustum.up = up_modified;
			cam->frustum.front = q.Mul(cam->frustum.front).Normalized();
		}
	}

}

void ModuleCamera3D::Rotate(float dx, float dy)
{
	float Sensitivity = 0.25f;
	dx = -App->input->GetMouseXMotion();

	if (dx != 0 && dy != 0)
	{
		Position -= Reference;

		if (dx == Reference.x, dy == Reference.y)
		{

			float DeltaX = (float)dx * Sensitivity;
			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));


		}
		Position = Reference + Z * length(Position);
	}
}

void ModuleCamera3D::LookObject(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.25f;

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	if(Y.y > 0.0f)Position = Reference + Z * length(Position);
}

void ModuleCamera3D::LookAndMoveToObject()
{
	geo = App->geometry_manager->geometry;
	if (geo != NULL)
	{
		AABB* box = &((_Model*)(App->geometry_manager->geometry))->GetBoundingBox();
		float3 center = box->Centroid();
		float3 size = box->Size();
		Position.Set(center.x + size.x, center.y + size.y, center.z + size.z);
		LookObject(vec3(center.x, center.y, center.z));
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

void ModuleCamera3D::ReajustLook()
{
	geo = App->geometry_manager->geometry;
	float3 center;
	if (geo != NULL)
	{
		AABB* box = &((_Model*)(geo))->GetBoundingBox();
		center = box->Centroid();
	}
	else
	{
		center.Set(0, 0, 0);
	}
	Reference = vec3(center.x, center.y, center.z);

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.25f;

	Position -= Reference;
	if (Y.y > 0.0f)Position = Reference + Z * length(Position);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ModuleCamera3D::MoveAt(const vec3 &Movement)
{
	Position = Movement;
	Reference = Movement;

	CalculateViewMatrix();
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

void ModuleCamera3D::MoveWithMouse(float dt)
{

}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

//void ModuleCamera3D::SetRay(float mouseX, float mouseY, float screenW, float screenH)
//{
//	// Convert x, y from screen to normalized device coordinates [-1:1]
//	float x = (2.0f * mouseX) / screenW - 1.0f;
//	float y = 1.0f - (2.0f * mouseY) / screenH;
//
//	vec3 ray;
//	ray.Set(x, y, -1.0f);
//}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
