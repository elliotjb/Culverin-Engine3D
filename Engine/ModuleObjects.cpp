#include "ModuleObjects.h"
#include "Primitive.h"
#include "Globals.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "Application.h"
#include "_Cube.h"
#include "_Sphere.h"

ModuleObjects::ModuleObjects(bool start_enabled) : Module(start_enabled)
{

}

ModuleObjects::~ModuleObjects()
{

}

bool ModuleObjects::Start()
{
	return true;
}

update_status ModuleObjects::Update(float dt)
{
	perf_timer.Start();

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}
	
	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}



bool ModuleObjects::CleanUp()
{
	//TODO -> fill THIS
	return true;
}

void ModuleObjects::ShowPerformance(int ms_index)
{
	if (ImGui::CollapsingHeader("OBJECTS"))
	{
		ImGui::Text("Pre-Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", pre_log[ms_index - 1]);
		ImGui::PlotHistogram("", pre_log, IM_ARRAYSIZE(pre_log), 0, NULL, 0.0f, 5.0f, ImVec2(0, 80));
		ImGui::Text("Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", up_log[ms_index - 1]);
		ImGui::PlotHistogram("", up_log, IM_ARRAYSIZE(up_log), 0, NULL, 0.0f, 50.0f, ImVec2(0, 80));
		ImGui::Text("Post-Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", post_log[ms_index - 1]);
		ImGui::PlotHistogram("", post_log, IM_ARRAYSIZE(post_log), 0, NULL, 0.0f, 50.0f, ImVec2(0, 80));
	}
}


bool ModuleObjects::Index_vert(float3* vertex_triangulate, uint num_vertex, BaseObject* obj)
{
	std::vector<float3> all_index;
	std::vector<uint> index_array;
	std::vector<float3> vertex_array;
	uint vertex_count = 0;
	uint size = 0;
	bool temp = false;

	for (int i = 0; i < num_vertex; i++)
	{
		temp = false;
		size = all_index.size();
		for (int j = 0; j < size; j++)
		{
			if (all_index[j] == vertex_triangulate[i])
			{
				index_array.push_back(j);
				temp = true;
			}
		}
		if (temp == false)
		{
			all_index.push_back(vertex_triangulate[i]);
			index_array.push_back(all_index.size() - 1);
			vertex_array.push_back(vertex_triangulate[i]);
		}
	}

	//SET VERTICES --------------------------------
	obj->mesh.num_vertices = vertex_array.size();
	obj->mesh.vertices3 = new float3[obj->mesh.num_vertices];
	for (int i = 0; i < vertex_array.size(); i++)
	{
		obj->mesh.vertices3[i] = vertex_array[i];
	}

	//SET INDICES ------------------------------------------
	obj->mesh.num_indices = index_array.size();
	obj->mesh.indices = new uint[obj->mesh.num_indices];
	for (int i = 0; i < index_array.size(); i++)
	{
		obj->mesh.indices[i] = index_array[i];
	}

	return true;
}

void ModuleObjects::CreateCube(float3 pos, float3 size, bool k, Color color)
{
	_Cube* cube = new _Cube(P_CUBE, pos, size, k, color);
	Index_vert(cube->vertex_array.data(), 36, cube);
	cube->id = count++;
	cube->Init_float3();
	objects.push_back(cube);
	//cubes.push_back(cube);
}

void ModuleObjects::CreateSphere(float3 pos, float radius, uint def, bool k, Color color)
{
	_Sphere* sphere = new _Sphere(P_SPHERE, pos, radius, k, color, def);
	Index_vert(sphere->vertex_array.data(), sphere->definition, sphere);
	sphere->id = count++;
	sphere->Init_float3();
	objects.push_back(sphere);
	//spheres.push_back(sphere);

}


void ModuleObjects::ShowAxis(bool axis)
{
	/*for (int i = 0; i < Body_spheres.size(); i++)
	{
		spheres[i].sphere.axis = axis;
	}
	for (int i = 0; i < Body_cubes.size(); i++)
	{
		cubes[i].cube.axis = axis;
	}*/
}

void ModuleObjects::ShowWire(bool wire)
{
	wireframe_mode = wire;
	/*for (int i = 0; i < Body_spheres.size(); i++)
	{
		spheres[i].sphere.wire = wire;
	}
	for (int i = 0; i < Body_cubes.size(); i++)
	{
		cubes[i].cube.wire = wire;
	}*/

}
