#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "imgui.h"
#include "Gl3W\include\glew.h"
#include "imgui_impl_sdl_gl3.h"

Scene::Scene(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

Scene::~Scene()
{
}

bool Scene::Start()
{
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	Sphere_A = (Sphere(float3(0, 0, 0), 10));
	Sphere_B = (Sphere(float3(0, 0, 5), 20));
	Capsule_A = (Capsule(float3(200, 0, 0), float3(200, 0, 3), 1));
	Capsule_B = (Capsule(float3(0, -3, 0), float3(0, 3, 0), 1));
	line = (Line(float3(0, 0, 0), float3(3, 1, 2)));
	plane = (Plane(float3(3, 2, 1), float3(-3, 0, -2)));

	return true;
}

update_status Scene::Update(float dt)
{
	//IMGUI----------------------------------------------------
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	ImGui::ShowTestWindow();

	//bool Intersects(const Sphere &sphere) const;


	// Intersects test WINDOW --------
	ImGui::Begin("INTERSECTIONS TEST");

	ImGui::Text("Sphere A - Position: (%.2f, %.2f, %.2f)  Radius: %.2f", Sphere_A.pos.x, Sphere_A.pos.y, Sphere_A.pos.z, Sphere_A.r);
	ImGui::Text("Sphere B - Position: (%.2f, %.2f, %.2f)  Radius: %.2f", Sphere_B.pos.x, Sphere_B.pos.y, Sphere_B.pos.z, Sphere_B.r);
	static int clicked_2 = 0;
	if (ImGui::Button("CHECK SPHERES"))
		clicked_2++;
	if (clicked_2 & 1)
	{
		if (Sphere_B.Intersects(Sphere_B))
		{
			ImGui::Text("YEEES!");
		}
		else
		{
			ImGui::Text("NOOO!");
		}
	}

	ImGui::Text("Capsule A - Bottom point: (%.2f, %.2f, %.2f)  Top point: (%.2f, %.2f, %.2f)  Radius: %.2f", Capsule_A.l.a.x, Capsule_A.l.a.y, Capsule_A.l.a.z, Capsule_A.l.b.x, Capsule_A.l.b.y, Capsule_A.l.b.z, Capsule_A.r);
	ImGui::Text("Capsule B - Bottom point: (%.2f, %.2f, %.2f)  Top point: (%.2f, %.2f, %.2f)  Radius: %.2f", Capsule_B.l.a.x, Capsule_B.l.a.y, Capsule_B.l.a.z, Capsule_B.l.b.x, Capsule_B.l.b.y, Capsule_B.l.b.z, Capsule_B.r);
	static int clicked_3 = 0;
	if (ImGui::Button("CHECK CAPSULES"))
		clicked_3++;
	if (clicked_3 & 1)
	{
		if (Capsule_A.Intersects(Capsule_B))
		{
			ImGui::Text("YEEES!");
		}
		else
		{
			ImGui::Text("NOOO!");
		}
	}
	ImGui::End();
	// --------------------------

	// CLOSE APP WINDOW --------
	ImGui::Begin("CLOSE APP"); 

	static int clicked = 0;
	if (ImGui::Button("CLICK HERE"))
		clicked++;
	if (clicked & 1)
	{
		return UPDATE_STOP;
	}

	static int i1 = 0;
	ImGui::SliderInt("slider int", &i1, 0, 5);
	static float f1 = 0.123f, f2 = 0.0f;
	ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "%.3f");

	ImGui::End();
	// --------------------------

	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	return true;
}
