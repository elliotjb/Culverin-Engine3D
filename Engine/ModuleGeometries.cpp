#include "ModuleGeometries.h"
#include "Primitive.h"
#include "Globals.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "Application.h"
#include "_Cube.h"
#include "_Sphere.h"

ModuleGeometries::ModuleGeometries(bool start_enabled) : Module(start_enabled)
{
	name = "Geometries Manager";
	geometry = nullptr;
}

ModuleGeometries::~ModuleGeometries()
{

}

bool ModuleGeometries::Start()
{
	return true;
}

update_status ModuleGeometries::Update(float dt)
{
	perf_timer.Start();

	//for (int i = 0; i < objects.size(); i++)
	//{
	//	objects[i]->Draw();
	//}

	/*ONLY ONE GEOMETRY CAN BE LOADED FOR PRACTICE 1*/
	if (geometry != nullptr) 
	{
		geometry->Draw();
	}
	
	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}


bool ModuleGeometries::CleanUp()
{
	//TODO -> fill THIS
	return true;
}

void ModuleGeometries::ShowPerformance(int ms_index)
{
	if (ImGui::CollapsingHeader("GEOMETRIES"))
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


bool ModuleGeometries::Index_vert(float3* vertex_triangulate, uint num_vertex, Mesh* mesh)
{
	std::vector<float3> all_index;
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
				mesh->indices.push_back(j);
				temp = true;
			}
		}
		if (temp == false)
		{
			Vertex vertex;
			all_index.push_back(vertex_triangulate[i]);
			mesh->indices.push_back(all_index.size() - 1);
			vertex.pos = vertex_triangulate[i];
			mesh->vertices.push_back(vertex);
		}
	}

	return true;
}

void ModuleGeometries::CreateCube(float3 pos, float3 size, bool k, Color color)
{
	//_Cube* cube = new _Cube(P_CUBE, pos, size, k, color, wireframe_mode);
	//Index_vert(cube->vertex_array.data(), 36, &cube->cube_mesh);
	////cube->GenFaceNormals(cube->face_centers.data());
	//cube->id = count++;
	//cube->Init();
	//objects.push_back(cube);
	////cubes.push_back(cube);
}

void ModuleGeometries::CreateSphere(float3 pos, float radius, uint def, bool k, Color color)
{
	//_Sphere* sphere = new _Sphere(P_SPHERE, pos, radius, k, color, def, wireframe_mode);
	//Index_vert(sphere->vertex_array.data(), sphere->definition, &sphere->sphere_mesh);
	//sphere->id = count++;
	//sphere->Init();
	//objects.push_back(sphere);
	////spheres.push_back(sphere);

}


void ModuleGeometries::ShowAxis(bool axis)
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

void ModuleGeometries::ShowWire(bool wire)
{
	wireframe_mode = wire;

	//for (int i = 0; i < objects.size(); i++)
	//{
	//	objects[i]->SetWireframe(wire);
	//}

	///*ONLY ONE GEOMETRY CAN BE LOADED FOR PRACTICE 1*/
	//if (geometry != nullptr)
	//{
	//	geometry->SetWireframe(wireframe_mode);
	//}
}

void ModuleGeometries::ShowNormals(bool norm)
{
	normals = norm;

	//for (int i = 0; i < objects.size(); i++)
	//{
	//	objects[i]->SetNormals(norm);
	//}

	/*ONLY ONE GEOMETRY CAN BE LOADED FOR PRACTICE 1*/
	//if (geometry != nullptr)
	//{
	//	geometry->SetNormals(norm);
	//}
}
