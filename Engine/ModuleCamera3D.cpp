#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "_Model.h"
#include "ImGui\imgui.h"


ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 3.0f, 10.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	camera_pos = vec3(0.0f, 0.0f, 0.0f);
	vec_view = vec3(0.0f, 0.0f, 0.0f);

	name = "Camera";
	haveConfig = true;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	freecam = false;
	moveWithScroll = 30.0f;
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
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	if (io.WantTextInput == false && isMouseOnWindow())
	{
		vec3 newPos(0, 0, 0);
		float speed = 3.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 28.0f * dt;
		// Mouse motion ----------------
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
		{
			if (1)
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
			if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) newPos.y -= speed;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			LookAndMoveToObject();
		}

		//Camera Zoom ---------------------------
		if (App->input->GetMouseZ() == 1)
		{
			newPos -= Z * (moveWithScroll * dt);
		}
		if (App->input->GetMouseZ() == -1)
		{
			newPos += Z * (moveWithScroll * dt);
		} 
		// -------------------------------------
		//Mouse Middle ------------------------
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
		{
			if (App->input->GetMouseXMotion() > 0)
			{
				newPos -= X * (App->input->GetMouseXMotion()* dt);
			}
			if (App->input->GetMouseXMotion() < 0)
			{
				newPos -= X * (App->input->GetMouseXMotion() * dt);
			}
			if (App->input->GetMouseYMotion() > 0)
			{
				newPos += Y * (App->input->GetMouseYMotion() * dt);
			}
			if (App->input->GetMouseYMotion() < 0)
			{
				newPos += Y * (App->input->GetMouseYMotion() * dt);
			}			
		}
		// -------------------------------


		Position += newPos;
		Reference += newPos;

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			BaseGeometry* geo = App->geometry_manager->geometry;
			if (geo != NULL)
			{
				float3 geoPosition = ((_Model*)(geo))->GetPosition();
				LookObject(vec3(geoPosition.x, geoPosition.y, geoPosition.z));//Need pass the positon of object
			}
			else
				LookObject(vec3(0, 0, 0));
		}
	}
	// Recalculate matrix -------------
	CalculateViewMatrix();

	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera3D::UpdateConfig(float dt)
{
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
	BaseGeometry* geo = App->geometry_manager->geometry;
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

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::ShowPerformance(int ms_index)
{
	if (ImGui::CollapsingHeader("CAMERA"))
	{
		ImGui::Text("Pre-Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%.4f", pre_log[ms_index - 1]);
		ImGui::Text("Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%.4f", up_log[ms_index - 1]);
		ImGui::Text("Post-Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%.4f", post_log[ms_index - 1]);
	}
}