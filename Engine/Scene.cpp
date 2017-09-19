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
	//ShowTest -----------------------
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		ShowTest = !ShowTest;
	}
	if (ShowTest)
	{
		ImGui::ShowTestWindow();
	}

	// Windows to Create Objects ----------------------
	ImGui::Begin("Create Objects");
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Sphere"))
	{
		ImGui::Text("Position Sphere:");
		static float sphere_pos_x = 0.0f;
		ImGui::PushItemWidth(150);
		ImGui::InputFloat("x", &sphere_pos_x, 0.01f, 0, 3);
		static float sphere_pos_y = 0.0f;
		ImGui::InputFloat("y", &sphere_pos_y, 0.01f, 0, 3);
		static float sphere_pos_z = 0.0f;
		ImGui::InputFloat("z", &sphere_pos_z, 0.01f, 0, 3);
		ImGui::Spacing();
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(100);
		ImGui::Text("Radius Sphere");
		static float sphere_radius = 1.0f;
		ImGui::InputFloat("", &sphere_radius, 0.05f, 0, 3);
		static int create_sphere = 0;
		ImGui::SameLine(220); if (ImGui::Button("Create Sphere"))
			create_sphere++;
		if (create_sphere & 1)
		{
			create_sphere++;
			list_Sphere.add(Sphere(float3(sphere_pos_x, sphere_pos_y, sphere_pos_z), sphere_radius));
		}
		ImGui::PopItemWidth();
	}
	if (ImGui::CollapsingHeader("Capsule"))
	{
		ImGui::Text("Position Capsule");
		ImGui::PushItemWidth(150);
		ImGui::BulletText("Position A:");
		ImGui::SameLine(180); ImGui::BulletText("Position B:");
		static float Capsule_pos_A_x = 0.0f;
		ImGui::InputFloat("A_x", &Capsule_pos_A_x, 0.01f, 0, 3);
		static float Capsule_pos_B_x = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("B_x", &Capsule_pos_B_x, 0.01f, 0, 3);

		static float Capsule_pos_A_y = 0.0f;
		ImGui::InputFloat("A_y", &Capsule_pos_A_y, 0.01f, 0, 3);

		static float Capsule_pos_B_y = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("B_y", &Capsule_pos_B_y, 0.01f, 0, 3);

		static float Capsule_pos_A_z = 0.0f;
		ImGui::InputFloat("A_z", &Capsule_pos_A_z, 0.01f, 0, 3);

		static float Capsule_pos_B_z = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("B_z", &Capsule_pos_B_z, 0.01f, 0, 3);

		ImGui::Spacing();


		ImGui::PopItemWidth();
		//Radius
		ImGui::Spacing();
		ImGui::PushItemWidth(100);
		ImGui::Text("Radius Capsule");
		static float capsule_radius = 1.0f;
		ImGui::InputFloat("", &capsule_radius, 0.05f, 0, 3);
		ImGui::PopItemWidth();

		static int create_capsule = 0;
		ImGui::SameLine(220); if (ImGui::Button("Create Capsule"))
			create_capsule++;
		if (create_capsule & 1)
		{
			create_capsule++;
			list_Capsule.add(Capsule(float3(Capsule_pos_A_x, Capsule_pos_A_y, Capsule_pos_A_z), float3(Capsule_pos_A_x, Capsule_pos_A_y, Capsule_pos_A_z), capsule_radius));
		}
	}
	if (ImGui::CollapsingHeader("Plane"))
	{
		ImGui::PushItemWidth(150);

		ImGui::Text("Position Point in Plane");
		ImGui::BulletText("Position Point");
		ImGui::SameLine(220); ImGui::BulletText("Position Normal");

		static float plane_point_pos_x = 0.0f;
		ImGui::InputFloat("point_x", &plane_point_pos_x, 0.01f, 0, 3);
		static float plane_normal_pos_x = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("normal_x", &plane_normal_pos_x, 0.01f, 0, 3);


		static float plane_point_pos_y = 0.0f;
		ImGui::InputFloat("point_y", &plane_point_pos_y, 0.01f, 0, 3);
		static float plane_normal_pos_y = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("normal_y", &plane_normal_pos_y, 0.01f, 0, 3);


		static float plane_point_pos_z = 0.0f;
		ImGui::InputFloat("point_z", &plane_point_pos_z, 0.01f, 0, 3);
		static float plane_normal_pos_z = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("normal_z", &plane_normal_pos_z, 0.01f, 0, 3);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::PopItemWidth();
		static int create_plane = 0;
		ImGui::SameLine(220); if (ImGui::Button("Create Plane"))
			create_plane++;
		if (create_plane & 1)
		{
			create_plane++;
			list_Plane.add(Plane(float3(plane_point_pos_x, plane_point_pos_y, plane_point_pos_z), float3(plane_normal_pos_x, plane_normal_pos_y, plane_normal_pos_z)));
		}
	}
	if (ImGui::CollapsingHeader("Ray"))
	{
		ImGui::PushItemWidth(150);

		ImGui::Text("Position Ray");
		ImGui::BulletText("Position Line");
		ImGui::SameLine(180); ImGui::BulletText("Direction");

		static float ray_pos_x = 0.0f;
		ImGui::InputFloat("position_x", &ray_pos_x, 0.01f, 0, 3);
		static float ray_dir_x = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("direction_x", &ray_dir_x, 0.01f, 0, 3);


		static float ray_pos_y = 0.0f;
		ImGui::InputFloat("position_y", &ray_pos_y, 0.01f, 0, 3);
		static float ray_dir_y = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("direction_y", &ray_dir_y, 0.01f, 0, 3);


		static float ray_pos_z = 0.0f;
		ImGui::InputFloat("position_z", &ray_pos_z, 0.01f, 0, 3);
		static float ray_dir_z = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("direction_z", &ray_dir_z, 0.01f, 0, 3);
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		static int create_ray = 0;
		ImGui::SameLine(220); if (ImGui::Button("Create Ray"))
			create_ray++;
		if (create_ray & 1)
		{
			list_Ray.add(Ray(Line(float3(ray_pos_x, ray_pos_y, ray_pos_z), float3(ray_dir_x, ray_dir_y, ray_dir_z))));
			create_ray++;
		}
	}

	ImGui::End();

	// Windows See Objects in Scene ----------------------
	ImGui::Begin("Objects in Scene");
	// Spheres --------
	if (ImGui::TreeNode("Spheres"))
	{
		static p2List<bool> checkers;
		static int num_spheres = 0;
		if (num_spheres < list_Sphere.count())
		{
			for (int i = num_spheres; i < list_Sphere.count(); i++)
			{
				checkers.add(bool(false));
			}
			num_spheres = list_Sphere.count();
			LOG("%i", num_spheres);
		}

		p2List_item<Sphere>* item = list_Sphere.getFirst();
		p2List_item<bool>* check = checkers.getFirst();
		for (int i = 0; i < list_Sphere.count(); i++)
		{
			ImGui::Text("Sphere %i", i + 1);
			ImGui::PushID(i);
			ImGui::SameLine(); ImGui::Checkbox("Select", &check->data);
			ImGui::PopID();
			ImGui::BulletText("Position: (%.2f, %.2f, %.2f)  Radius: %.2f", item->data.pos.x, item->data.pos.y, item->data.pos.z, item->data.r);
			item = item->next;
			check = check->next;
		}
		ImGui::TreePop();
	}
	// Capsules --------
	if (ImGui::TreeNode("Capsules"))
	{
		static p2List<bool> checkers;
		static int num_capsules = 0;
		if (num_capsules < list_Capsule.count())
		{
			for (int i = num_capsules; i < list_Capsule.count(); i++)
			{
				checkers.add(bool(false));
			}
			num_capsules = list_Capsule.count();
		}

		p2List_item<Capsule>* item = list_Capsule.getFirst();
		p2List_item<bool>* check = checkers.getFirst();
		for (int i = 0; i < list_Capsule.count(); i++)
		{
			ImGui::Text("Capsule %i", i + 1);
			ImGui::PushID(i);
			ImGui::SameLine(); ImGui::Checkbox("Select", &check->data);
			ImGui::PopID();
			ImGui::BulletText("Position A (Down): (%.2f, %.2f, %.2f)", item->data.l.a.x, item->data.l.a.y, item->data.l.a.z);
			ImGui::BulletText("Position A (UP): (%.2f, %.2f, %.2f)", item->data.l.b.x, item->data.l.b.y, item->data.l.b.z);
			ImGui::BulletText("Radius: %.2f", item->data.r);
			item = item->next;
			check = check->next;
		}
		ImGui::TreePop();
	}
	// Planes --------
	if (ImGui::TreeNode("Planes"))
	{
		static p2List<bool> checkers;
		static int num_planes = 0;
		if (num_planes < list_Plane.count())
		{
			for (int i = num_planes; i < list_Plane.count(); i++)
			{
				checkers.add(bool(false));
			}
			num_planes = list_Plane.count();
		}

		p2List_item<Plane>* item = list_Plane.getFirst();
		p2List_item<bool>* check = checkers.getFirst();
		for (int i = 0; i < list_Plane.count(); i++)
		{
			ImGui::Text("Plane %i", i + 1);
			ImGui::PushID(i);
			ImGui::SameLine(); ImGui::Checkbox("Select", &check->data);
			ImGui::PopID();
			ImGui::BulletText("Position Normal: (%.2f, %.2f, %.2f)", item->data.normal.x, item->data.normal.y, item->data.normal.z);
			item = item->next;
			check = check->next;
		}
		ImGui::TreePop();
	}
	//Rays -----------------
	if (ImGui::TreeNode("Rays"))
	{
		static p2List<bool> checkers;
		static int num_rays = 0;
		if (num_rays < list_Ray.count())
		{
			for (int i = num_rays; i < list_Ray.count(); i++)
			{
				checkers.add(bool(false));
			}
			num_rays = list_Ray.count();
		}

		p2List_item<Ray>* item = list_Ray.getFirst();
		p2List_item<bool>* check = checkers.getFirst();
		for (int i = 0; i < list_Ray.count(); i++)
		{
			ImGui::Text("Ray %i", i + 1);
			ImGui::PushID(i);
			ImGui::SameLine(); ImGui::Checkbox("Select", &check->data);
			ImGui::PopID();
			ImGui::BulletText("Position: (%.2f, %.2f, %.2f)", item->data.pos.x, item->data.pos.y, item->data.pos.z);
			ImGui::BulletText("Direction: (%.2f, %.2f, %.2f)", item->data.dir.x, item->data.dir.y, item->data.dir.z);
			item = item->next;
			check = check->next;
		}
		ImGui::TreePop();
	}

	ImGui::End();
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
