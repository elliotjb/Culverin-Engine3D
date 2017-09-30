#include "ModuleObjectsUI.h"
#include "ModuleObjects.h"
#include "Application.h"

ModuleObjectsUI::ModuleObjectsUI() : WindowManager()
{
	active.push_back(Active());// Create window
	active.push_back(Active());//Show window
}

ModuleObjectsUI::~ModuleObjectsUI()
{
	active.clear();
}

bool ModuleObjectsUI::Start()
{
	return true;
}

update_status ModuleObjectsUI::Update(float dt)
{
	if (active[CREATE].active)
		ShowCreateObjects();

	if (active[SHOW].active)
		ShowObjectsinScene();

	return UPDATE_CONTINUE;
}

bool ModuleObjectsUI::CleanUp()
{
	return true;
}


// Windows to Create Objects ----------------------
void ModuleObjectsUI::ShowCreateObjects()
{
	if (!ImGui::Begin("Create Objects", &App->gui->winManager[CREATEOBJETCS]->active[CREATE].active, ImGuiWindowFlags_AlwaysAutoResize)) //TODO ELLIOT CLOSE Windows example
	{
		ImGui::End();
		return;
	}
	ImGui::Text("                    ");
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Sphere"))
	{
		ImGui::Text("Position Sphere:");
		static float sphere_pos_x = 0.0f;
		ImGui::PushItemWidth(150);
		ImGui::InputFloat("x", &sphere_pos_x, 0.1f, 0, 3);
		ImGui::SameLine(220);
		static bool kynematic = false;
		ImGui::Checkbox("Kynematic", &kynematic);
		static float sphere_pos_y = 0.0f;
		ImGui::InputFloat("y", &sphere_pos_y, 0.1f, 0, 3);
		static float sphere_pos_z = 0.0f;
		ImGui::InputFloat("z", &sphere_pos_z, 0.1f, 0, 3);
		ImGui::Spacing();
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(100);
		ImGui::Text("Radius Sphere");
		static float sphere_radius = 1.0f;
		ImGui::InputFloat("", &sphere_radius, 0.05f, 0, 3);
		static int create_sphere = 0;
		ImGui::SameLine(260); if (ImGui::Button("Create Sphere"))
			create_sphere++;
		ImGui::PopItemWidth();

		static ImVec4 color = ImColor(255, 255, 255, 255);
		static bool hdr = false;
		static bool alpha_preview = true;
		static bool alpha_half_preview = false;
		static bool options_menu = true;
		int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
		ImGui::Text("Set Color of Sphere:");
		ImGui::ColorEdit4("ColorObject##1", (float*)&color, ImGuiColorEditFlags_RGB | misc_flags);
		if (create_sphere & 1)
		{
			create_sphere++;
			Sphere* temp = new Sphere(float3(sphere_pos_x, sphere_pos_y, sphere_pos_z), sphere_radius);
			Color color_n;
			color_n.Set(color.x, color.y, color.z, color.w);
			App->objects->CreateSphere(temp, color_n/*, kynematic*/);
			/* SPHERE WITH PRIMITIVE -----------------------------------------
			create_sphere++;
			App->objects->math_Sphere.push_back(new Sphere(float3(sphere_pos_x, sphere_pos_y, sphere_pos_z), sphere_radius));
			Sphere_p* temp = new Sphere_p(sphere_radius);
			temp->SetPos(sphere_pos_x, sphere_pos_y, sphere_pos_z);
			App->objects->CreateSphere(temp, color, kynematic);*/
		}
	}
	if (ImGui::CollapsingHeader("Cube"))
	{
		ImGui::Text("Position Cube");
		ImGui::PushItemWidth(150);
		ImGui::BulletText("Position:");
		ImGui::SameLine(180); ImGui::BulletText("Size:");
		static float pos_x = 0.0f;
		ImGui::InputFloat("pos_x", &pos_x, 0.1f, 0, 3);
		static float size_x = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("size_x", &size_x, 0.1f, 0, 3);

		static float pos_y = 0.0f;
		ImGui::InputFloat("pos_y", &pos_y, 0.1f, 0, 3);

		static float size_y = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("size_y", &size_y, 0.1f, 0, 3);

		static float pos_z = 0.0f;
		ImGui::InputFloat("pos_z", &pos_z, 0.1f, 0, 3);

		static float size_z = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("size_z", &size_z, 0.1f, 0, 3);

		ImGui::Spacing();
		ImGui::PopItemWidth();

		static bool kynematic = false;
		ImGui::Checkbox("Kynematic", &kynematic);
		static ImVec4 color = ImColor(255, 255, 255, 255);
		static bool hdr = false;
		static bool alpha_preview = true;
		static bool alpha_half_preview = false;
		static bool options_menu = true;
		int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
		ImGui::Text("Set Color of Sphere:");
		ImGui::ColorEdit4("ColorObject##2", (float*)&color, ImGuiColorEditFlags_RGB | misc_flags);
		ImGui::Spacing();
		static int create_capsule = 0;
		ImGui::SameLine(260); if (ImGui::Button("Create Cube"))
			create_capsule++;

		if (create_capsule & 1)
		{
			create_capsule++;
			OBB* temp = new OBB();
			temp->pos = float3(pos_x, pos_y, pos_z);
			temp->r = float3(size_x, size_y, size_z);
			temp->axis[0] = float3(1, 0, 0);
			temp->axis[1] = float3(0, 1, 0);
			temp->axis[2] = float3(0, 0, 1);
			Color color_n;
			color_n.Set(color.x, color.y, color.z, color.w);
			App->objects->CreateCube(temp, color_n/*, kynematic*/);


			/* Cube WITH PRIMITIVE -----------------------------------------
			Cube_p* temp = new Cube_p();
			temp->SetPos(pos_x, pos_y, pos_z);
			temp->size = vec3(size_x, size_y, size_z);
			App->objects->CreateCube(temp, color, kynematic);*/
		}
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
		ImGui::SameLine(260); if (ImGui::Button("Create Capsule"))
			create_capsule++;
		if (create_capsule & 1)
		{
			create_capsule++;
			//App->objects->math_Capsule.push_back(new Capsule(float3(Capsule_pos_A_x, Capsule_pos_A_y, Capsule_pos_A_z), float3(Capsule_pos_B_x, Capsule_pos_B_y, Capsule_pos_B_z), capsule_radius));
		}
	}
	if (ImGui::CollapsingHeader("Cylinder"))
	{
		ImGui::PushItemWidth(150);

		ImGui::Text("Position");
		ImGui::BulletText("Position Point");

		static float Cylinder_pos_x = 0.0f;
		ImGui::InputFloat("pos_x", &Cylinder_pos_x, 0.01f, 0, 3);

		static float Cylinder_pos_y = 0.0f;
		ImGui::InputFloat("pos_y", &Cylinder_pos_y, 0.01f, 0, 3);

		static float Cylinder_pos_z = 0.0f;
		ImGui::InputFloat("pos_z", &Cylinder_pos_z, 0.01f, 0, 3);

		static float Cylinder_lenght = 0.0f;
		ImGui::InputFloat("lenght", &Cylinder_lenght, 0.01f, 0, 3);
		static float Cylinder_radius = 0.0f;
		ImGui::SameLine(); ImGui::InputFloat("radius", &Cylinder_radius, 0.01f, 0, 3);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::PopItemWidth();
		static int create_cylinder = 0;
		ImGui::SameLine(260); if (ImGui::Button("Create Cylinder"))
			create_cylinder++;
		if (create_cylinder & 1)
		{
			create_cylinder++;
			//App->
			//App->objects->math_Plane.push_back(new Plane(float3(plane_point_pos_x, plane_point_pos_y, plane_point_pos_z), float3(plane_normal_pos_x, plane_normal_pos_y, plane_normal_pos_z)));
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
		ImGui::SameLine(260); if (ImGui::Button("Create Plane"))
			create_plane++;
		if (create_plane & 1)
		{
			create_plane++;
			App->objects->math_Plane.push_back(new Plane(float3(plane_point_pos_x, plane_point_pos_y, plane_point_pos_z), float3(plane_normal_pos_x, plane_normal_pos_y, plane_normal_pos_z)));
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
		ImGui::SameLine(260); if (ImGui::Button("Create Ray"))
			create_ray++;
		if (create_ray & 1)
		{
			App->objects->math_Ray.push_back(new Ray(Line(float3(ray_pos_x, ray_pos_y, ray_pos_z), float3(ray_dir_x, ray_dir_y, ray_dir_z))));
			create_ray++;
		}
	}
	if (ImGui::CollapsingHeader("Frustum"))
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
		ImGui::SameLine(260); if (ImGui::Button("Create Frustum"))
			create_plane++;
		if (create_plane & 1)
		{
			create_plane++;
			//App->objects->math_Plane.push_back(new Plane(float3(plane_point_pos_x, plane_point_pos_y, plane_point_pos_z), float3(plane_normal_pos_x, plane_normal_pos_y, plane_normal_pos_z)));
		}
	}

	ImGui::End();
}

// Windows See Objects in Scene ----------------------
void ModuleObjectsUI::ShowObjectsinScene()
{
	if (!ImGui::Begin("Objects in Scene", &App->gui->winManager[CREATEOBJETCS]->active[SHOW].active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
		return;
	}
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
		if (num_spheres < App->objects->math_Sphere.size())
		{
			for (int i = num_spheres; i < App->objects->math_Sphere.size(); i++)
			{
				checkers.push_back(bool(false));
			}
			num_spheres = App->objects->math_Sphere.size();
			LOG("%i", num_spheres);
		}

		std::list<Sphere*>::iterator item = App->objects->math_Sphere.begin();
		std::list<bool>::iterator check = checkers.begin();
		for (int i = 0; i < App->objects->math_Sphere.size(); i++)
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
			std::list<Sphere*>::iterator item = App->objects->math_Sphere.begin();
			for (int i = 0; i < App->objects->math_Sphere.size(); i++)
			{
				if (object_delete == i)
				{
					App->objects->math_Sphere.erase(item);
					std::list<Sphere*>::iterator temp = App->objects->math_Sphere.begin();
					for (int j = 0; j < App->objects->math_Sphere.size(); j++)
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
		if (num_capsules < App->objects->math_Capsule.size())
		{
			for (int i = num_capsules; i < App->objects->math_Capsule.size(); i++)
			{
				checkers.push_back(bool(false));
			}
			num_capsules = App->objects->math_Capsule.size();
		}

		std::list<Capsule*>::iterator item = App->objects->math_Capsule.begin();
		std::list<bool>::iterator check = checkers.begin();
		for (int i = 0; i < App->objects->math_Capsule.size(); i++)
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
			std::list<Capsule*>::iterator item = App->objects->math_Capsule.begin();
			for (int i = 0; i < App->objects->math_Capsule.size(); i++)
			{
				if (object_delete == i)
				{
					App->objects->math_Capsule.erase(item);
					std::list<Capsule*>::iterator temp = App->objects->math_Capsule.begin();
					for (int j = 0; j < App->objects->math_Capsule.size(); j++)
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
		if (num_planes < App->objects->math_Plane.size())
		{
			for (int i = num_planes; i < App->objects->math_Plane.size(); i++)
			{
				checkers.push_back(bool(false));
			}
			num_planes = App->objects->math_Plane.size();
		}

		std::list<Plane*>::iterator item = App->objects->math_Plane.begin();
		std::list<bool>::iterator check = checkers.begin();
		for (int i = 0; i < App->objects->math_Plane.size(); i++)
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
			std::list<Plane*>::iterator item = App->objects->math_Plane.begin();
			for (int i = 0; i < App->objects->math_Plane.size(); i++)
			{
				if (object_delete == i)
				{
					App->objects->math_Plane.erase(item);
					std::list<Plane*>::iterator temp = App->objects->math_Plane.begin();
					for (int j = 0; j < App->objects->math_Plane.size(); j++)
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
		if (num_rays < App->objects->math_Ray.size())
		{
			for (int i = num_rays; i < App->objects->math_Ray.size(); i++)
			{
				checkers.push_back(bool(false));
			}
			num_rays = App->objects->math_Ray.size();
		}

		std::list<Ray*>::iterator item = App->objects->math_Ray.begin();
		std::list<bool>::iterator check = checkers.begin();
		for (int i = 0; i < App->objects->math_Ray.size(); i++)
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
			std::list<Ray*>::iterator item = App->objects->math_Ray.begin();
			for (int i = 0; i < App->objects->math_Ray.size(); i++)
			{
				if (object_delete == i)
				{
					App->objects->math_Ray.erase(item);
					std::list<Ray*>::iterator temp = App->objects->math_Ray.begin();
					for (int j = 0; j < App->objects->math_Ray.size(); j++)
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

void ModuleObjectsUI::SpecialFunction(const std::string name)
{
	/*if (name == "sphere")
	{
		App->objects->math_Sphere.push_back(new Sphere(float3(0, 1.0f, 0), 1.0f));
		Sphere_p* temp = new Sphere_p(1.0f);
		temp->SetPos(0, 1.0f, 0);
		App->objects->CreateSphere(temp, ImVec4(1, 1, 1, 1));
	}
	else if (name == "cube")
	{
		//App->objects->math_Cube.push_back(new AABB());
		Cube_p* temp = new Cube_p();
		temp->SetPos(0, 1.0f, 0);
		temp->size = vec3(2, 2, 2);
		App->objects->CreateCube(temp, ImVec4(1, 1, 1, 1));
	}
	else if (name == "Capsule")
	{

	}*/
}
