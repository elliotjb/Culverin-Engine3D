#include "Globals.h"
#include "ModuleGUI.h"
#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleHardware.h"
#include "imgui.h"
#include "Gl3W\include\glew.h"
#include "imgui_impl_sdl_gl3.h"
#include "Algorithm\Random\LCG.h"

ModuleGUI::ModuleGUI(Application * app, bool start_enabled): Module(app, start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

static void ShowHelpMarker(const char* desc, const char* icon = "(?)")
{
	ImGui::TextDisabled(icon);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

bool ModuleGUI::Start()
{
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	//Sphere_A = (Sphere(float3(0, 0, 0), 10));
	//Sphere_B = (Sphere(float3(0, 0, 5), 20));
	//Capsule_A = (Capsule(float3(200, 0, 0), float3(200, 0, 3), 1));
	//Capsule_B = (Capsule(float3(0, -3, 0), float3(0, 3, 0), 1));
	//line = (Line(float3(0, 0, 0), float3(3, 1, 2)));
	//plane = (Plane(float3(3, 2, 1), float3(-3, 0, -2)));
	return true;
}

update_status ModuleGUI::Update(float dt)
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
	// bools statics ----------------------
	static bool window_create_objects = false;
	static bool window_show_objects = false;
	static bool window_Random_generator = false;
	static bool window_about_us = false;


	// Main Menu --------------------------------
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl + N"))
			{

			}
			if (ImGui::MenuItem("Open Scene", "Ctrl + O"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save Scene", "Ctrl + S"))
			{

			}
			if (ImGui::MenuItem("Save Scene as..."))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl + Z"))
			{

			}
			if (ImGui::MenuItem("Redo", "Ctrl + Y"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl + X"))
			{

			}
			if (ImGui::MenuItem("Copy", "Ctrl + C"))
			{

			}
			if (ImGui::MenuItem("Paste", "Ctrl + V"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Etc..."))
			{

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Create Objects"))
			{
				window_create_objects = !window_create_objects;
			}
			if (ImGui::MenuItem("Objects in Scene"))
			{
				window_show_objects = !window_show_objects;
			}
			if (ImGui::MenuItem("Random Generator"))
			{
				window_Random_generator = !window_Random_generator;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Hardware"))
			{
				winManager.push_back(new Hardware(true));
				//App->hardware->OpenClose();
			}
			if (ImGui::MenuItem("Console", "º"))
			{
				App->console->OpenClose();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Configuration"))
			{
				//jordi
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About us..."))
			{
				window_about_us = !window_about_us;
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Documentation 'GitHub'"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Download latest"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/releases/download/0.1.1/Release.-.3D.Engine.v0.1.1.zip", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/releases/download/0.1.1/Release.-.3D.Engine.v0.1.1.zip", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Readme"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/blob/master/README.md", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}


	//Machine Generator -------------------
	if (window_Random_generator)
	{
		static LCG random_generator;
		ImGui::Begin("Random Numbers Generator");
		ImGui::Spacing();
		ImGui::PushItemWidth(60);
		static int numbers_f = 0;
		static int numbers_i = 0;
		if (ImGui::Button("FLOAT"))
			numbers_f++;
		if (ImGui::Button("INT"))
			numbers_i++; ImGui::SameLine();

		static int n1 = 0;
		static int n2 = 0;
		ImGui::PushItemWidth(100);
		ImGui::InputInt("Min", &n1, 1, 0, 1); ImGui::SameLine();
		ImGui::InputInt("Max", &n2, 1, 0, 1);

		if (numbers_f & 1)
		{
			numbers_f++;
			floats_list.push_back(random_generator.Float());
			//ImGui::Text("%.4f", random_generator.Float());
		}

		if (numbers_i & 1)
		{
			if (n1 <= n2)
			{
				numbers_i++;
				ints_list.push_back(random_generator.Int(n1, n2));
			}
		}

		static int clear = 0;
		if (ImGui::Button("CLEAR"))
			clear++;
		if (clear & 1)
		{
			clear++;
			if (floats_list.size() > 0)
			{
				floats_list.clear();
			}

			if (ints_list.size() > 0)
			{
				ints_list.clear();
			}
		}

		ImGui::PopItemWidth();
		ImGui::Spacing();

		ImGui::Text("FLOATS --------------------");
		for (uint n = 0; n < floats_list.size(); n++)
		{
			ImGui::Text("%i - %.4f", n, floats_list[n]);
		}

		ImGui::Text("INTS --------------------");
		for (uint n = 0; n < ints_list.size(); n++)
		{
			ImGui::Text("%i - %i", n, ints_list[n]);
		}
		ImGui::End();
	}
	

	//----------------------------------------------
	// Window About Us... ---------------------------------
	if (window_about_us)
	{
		ImGui::Begin("About Us...");
		//Name of your Engine
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("3D Engine");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Desciption
		ImGui::Text("3D Engine");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Name of the Author
		ImGui::Text("Authors:");
		ImGui::Spacing();
		ImGui::Text("Elliot Jimenez Bosch");
		ImGui::Text("Jordi Ona Rufi");
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Libraries
		ImGui::Text("Libraries:");
		ImGui::Spacing();
		ImGui::Text("SDL");

		ImGui::End();
	}

	// Windows to Create Objects ----------------------
	if (window_create_objects)
	{
		ShowCreateObjects();
	}

	// Windows See Objects in Scene ----------------------
	if (window_show_objects)
	{
		ShowObjectsinScene();
	}

	// Console --------------------------
	if (App->console->IsOpen())
	{
		ShowExampleAppConsole();
	}

	//Update All Modules ----------------------------------
	UpdateWindows(dt);

	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	return true;
}

void ModuleGUI::UpdateWindows(float dt)
{
	std::vector<WindowManager*>::iterator window = winManager.begin();
	for (int i = 0; i < winManager.size(); i++)
	{
		window[i]->Update(dt);
		window++;
	}
}

void ModuleGUI::ShowExampleAppConsole()
{
	App->console->Draw("CONSOLE");
}

// Windows to Create Objects ----------------------
void ModuleGUI::ShowCreateObjects()
{
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
			list_Sphere.push_back(new Sphere(float3(sphere_pos_x, sphere_pos_y, sphere_pos_z), sphere_radius));
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
			list_Capsule.push_back(new Capsule(float3(Capsule_pos_A_x, Capsule_pos_A_y, Capsule_pos_A_z), float3(Capsule_pos_B_x, Capsule_pos_B_y, Capsule_pos_B_z), capsule_radius));
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
			list_Plane.push_back(new Plane(float3(plane_point_pos_x, plane_point_pos_y, plane_point_pos_z), float3(plane_normal_pos_x, plane_normal_pos_y, plane_normal_pos_z)));
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
			list_Ray.push_back(new Ray(Line(float3(ray_pos_x, ray_pos_y, ray_pos_z), float3(ray_dir_x, ray_dir_y, ray_dir_z))));
			create_ray++;
		}
	}

	ImGui::End();
}

// Windows See Objects in Scene ----------------------
void ModuleGUI::ShowObjectsinScene()
{
	ImGui::Begin("Objects in Scene");
	// Spheres --------
	static int objects_selected = 0;
	static int object_delete = 0;
	static bool delete_object = false;
	LOG("%i", objects_selected);
	if (ImGui::TreeNode("Spheres"))
	{
		//static p2List<bool> checkers;
		static std::list<bool> checkers;
		static int num_spheres = 0;
		if (num_spheres < list_Sphere.size())
		{
			for (int i = num_spheres; i < list_Sphere.size(); i++)
			{
				checkers.push_back(bool(false));
			}
			num_spheres = list_Sphere.size();
			LOG("%i", num_spheres);
		}

		std::list<Sphere*>::iterator item = list_Sphere.begin();
		std::list<bool>::iterator check = checkers.begin();
		for (int i = 0; i < list_Sphere.size(); i++)
		{
			ImGui::Text("Sphere %i", i + 1);
			ImGui::PushID(i);
			bool temp = check._Ptr->_Myval;
			ImGui::SameLine(); ImGui::Checkbox("Select", &check._Ptr->_Myval);
			if (temp && check._Ptr->_Myval == false)
			{
				objects_selected--;
			}
			else if (temp == false && check._Ptr->_Myval && objects_selected < 2)
			{
				if (objects_selected == 0)
				{
					Sphere_A = *item._Ptr->_Myval;
				}
				if (objects_selected == 1)
				{
					Sphere_B = *item._Ptr->_Myval;
				}
				objects_selected++;
			}
			else if (temp == false && check._Ptr->_Myval && objects_selected == 2)
			{
				check._Ptr->_Myval = false;
			}
			ImGui::SameLine(); ImGui::Text("Delete:");
			ImGui::SameLine();
			if (ImGui::SmallButton(" "))
			{
				object_delete = i;
				delete_object = true;
			}
			ImGui::PopID();
			ImGui::BulletText("Position: (%.2f, %.2f, %.2f)  Radius: %.2f", item._Ptr->_Myval->pos.x, item._Ptr->_Myval->pos.y, item._Ptr->_Myval->pos.z, item._Ptr->_Myval->r);
			item++;
			check++;
		}
		if (delete_object)
		{
			delete_object = false;
			std::list<Sphere*>::iterator item = list_Sphere.begin();
			for (int i = 0; i < list_Sphere.size(); i++)
			{
				if (object_delete == i)
				{
					list_Sphere.erase(item);
					std::list<Sphere*>::iterator temp = list_Sphere.begin();
					for (int j = 0; j < list_Sphere.size(); j++)
					{
						if (j == i)
						{
							item = temp;
						}
						temp++;
					}
				}
				else
					item++;
			}
			object_delete = 0;
		}
		ImGui::TreePop();
	}
	// Capsules --------
	if (ImGui::TreeNode("Capsules"))
	{
		static std::list<bool> checkers;
		static int num_capsules = 0;
		if (num_capsules < list_Capsule.size())
		{
			for (int i = num_capsules; i < list_Capsule.size(); i++)
			{
				checkers.push_back(bool(false));
			}
			num_capsules = list_Capsule.size();
		}

		std::list<Capsule*>::iterator item = list_Capsule.begin();
		std::list<bool>::iterator check = checkers.begin();
		for (int i = 0; i < list_Capsule.size(); i++)
		{
			ImGui::Text("Capsule %i", i + 1);
			ImGui::PushID(i);
			bool temp = check._Ptr->_Myval;
			ImGui::SameLine(); ImGui::Checkbox("Select", &check._Ptr->_Myval);
			if (temp && check._Ptr->_Myval == false)
			{
				objects_selected--;
			}
			else if (temp == false && check._Ptr->_Myval && objects_selected < 2)
			{
				objects_selected++;
			}
			else if (temp == false && check._Ptr->_Myval && objects_selected == 2)
			{
				check._Ptr->_Myval = false;
			}
			ImGui::SameLine(); ImGui::Text("Delete:");
			ImGui::SameLine();
			if (ImGui::SmallButton(" "))
			{
				object_delete = i;
				delete_object = true;
			}
			ImGui::PopID();
			ImGui::BulletText("Position A (Down): (%.2f, %.2f, %.2f)", item._Ptr->_Myval->l.a.x, item._Ptr->_Myval->l.a.y, item._Ptr->_Myval->l.a.z);
			ImGui::BulletText("Position B (UP): (%.2f, %.2f, %.2f)", item._Ptr->_Myval->l.b.x, item._Ptr->_Myval->l.b.y, item._Ptr->_Myval->l.b.z);
			ImGui::BulletText("Radius: %.2f", item._Ptr->_Myval->r);
			item++;
			check++;
		}
		if (delete_object)
		{
			delete_object = false;
			std::list<Capsule*>::iterator item = list_Capsule.begin();
			for (int i = 0; i < list_Capsule.size(); i++)
			{
				if (object_delete == i)
				{
					list_Capsule.erase(item);
					std::list<Capsule*>::iterator temp = list_Capsule.begin();
					for (int j = 0; j < list_Capsule.size(); j++)
					{
						if (j == i)
						{
							item = temp;
						}
						temp++;
					}
				}
				else
					item++;
			}
			object_delete = 0;
		}
		ImGui::TreePop();
	}
	// Planes --------
	if (ImGui::TreeNode("Planes"))
	{
		static std::list<bool> checkers;
		static int num_planes = 0;
		if (num_planes < list_Plane.size())
		{
			for (int i = num_planes; i < list_Plane.size(); i++)
			{
				checkers.push_back(bool(false));
			}
			num_planes = list_Plane.size();
		}

		std::list<Plane*>::iterator item = list_Plane.begin();
		std::list<bool>::iterator check = checkers.begin();
		for (int i = 0; i < list_Plane.size(); i++)
		{
			ImGui::Text("Plane %i", i + 1);
			ImGui::PushID(i);
			bool temp = check._Ptr->_Myval;
			ImGui::SameLine(); ImGui::Checkbox("Select", &check._Ptr->_Myval);
			if (temp && check._Ptr->_Myval == false)
			{
				objects_selected--;
			}
			else if (temp == false && check._Ptr->_Myval && objects_selected < 2)
			{
				objects_selected++;
			}
			else if (temp == false && check._Ptr->_Myval && objects_selected == 2)
			{
				check._Ptr->_Myval = false;
			}
			ImGui::SameLine(); ImGui::Text("Delete:");
			ImGui::SameLine();
			if (ImGui::SmallButton(" "))
			{
				object_delete = i;
				delete_object = true;
			}
			ImGui::PopID();
			ImGui::BulletText("Position Normal: (%.2f, %.2f, %.2f)", item._Ptr->_Myval->normal.x, item._Ptr->_Myval->normal.y, item._Ptr->_Myval->normal.z);
			item++;
			check++;
		}
		if (delete_object)
		{
			delete_object = false;
			std::list<Plane*>::iterator item = list_Plane.begin();
			for (int i = 0; i < list_Plane.size(); i++)
			{
				if (object_delete == i)
				{
					list_Plane.erase(item);
					std::list<Plane*>::iterator temp = list_Plane.begin();
					for (int j = 0; j < list_Plane.size(); j++)
					{
						if (j == i)
						{
							item = temp;
						}
						temp++;
					}
				}
				else
					item++;
			}
			object_delete = 0;
		}
		ImGui::TreePop();
	}
	//Rays -----------------
	if (ImGui::TreeNode("Rays"))
	{
		static std::list<bool> checkers;
		static int num_rays = 0;
		if (num_rays < list_Ray.size())
		{
			for (int i = num_rays; i < list_Ray.size(); i++)
			{
				checkers.push_back(bool(false));
			}
			num_rays = list_Ray.size();
		}

		std::list<Ray*>::iterator item = list_Ray.begin();
		std::list<bool>::iterator check = checkers.begin();
		for (int i = 0; i < list_Ray.size(); i++)
		{
			ImGui::Text("Ray %i", i + 1);
			ImGui::PushID(i);
			bool temp = check._Ptr->_Myval;
			ImGui::SameLine(); ImGui::Checkbox("Select", &check._Ptr->_Myval);
			if (temp && check._Ptr->_Myval == false)
			{
				objects_selected--;
			}
			else if (temp == false && check._Ptr->_Myval && objects_selected < 2)
			{
				objects_selected++;
			}
			else if (temp == false && check._Ptr->_Myval && objects_selected == 2)
			{
				check._Ptr->_Myval = false;
			}
			ImGui::SameLine(); ImGui::Text("Delete:");
			ImGui::SameLine();
			if (ImGui::SmallButton(" "))
			{
				object_delete = i;
				delete_object = true;
			}
			ImGui::PopID();
			ImGui::BulletText("Position: (%.2f, %.2f, %.2f)", item._Ptr->_Myval->pos.x, item._Ptr->_Myval->pos.y, item._Ptr->_Myval->pos.z);
			ImGui::BulletText("Direction: (%.2f, %.2f, %.2f)", item._Ptr->_Myval->dir.x, item._Ptr->_Myval->dir.y, item._Ptr->_Myval->dir.z);
			item++;
			check++;
		}
		if (delete_object)
		{
			delete_object = false;
			std::list<Ray*>::iterator item = list_Ray.begin();
			for (int i = 0; i < list_Ray.size(); i++)
			{
				if (object_delete == i)
				{
					list_Ray.erase(item);
					std::list<Ray*>::iterator temp = list_Ray.begin();
					for (int j = 0; j < list_Ray.size(); j++)
					{
						if (j == i)
						{
							item = temp;
						}
						temp++;
					}
				}
				else
					item++;
			}
			object_delete = 0;
		}
		ImGui::TreePop();
	}

	ImGui::Spacing();
	ImGui::Spacing();
	static int check_intersect = 0;
	if (ImGui::Button("Check Intersections"))
		check_intersect++;
	if (check_intersect & 1)
	{
		if (objects_selected == 2)
		{
			if (Sphere_A.Intersects(Sphere_B))
			{
				ImGui::Text("YEEES!");
			}
			else
			{
				ImGui::Text("NOOO!");
			}
		}
		else
		{
			ImGui::Text("You need select 2 objects!");
		}
	}

	ImGui::End();
}

void ModuleGUI::ShowHardware()
{

}
