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
	haveConfig = false;
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
		if (geometry->isActive())
		{
			geometry->Draw();
		}
	}
	
	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

update_status ModuleGeometries::UpdateConfig(float dt)
{
	return UPDATE_CONTINUE;
}


bool ModuleGeometries::CleanUp()
{
	//TODO -> fill THIS
	return true;
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
			VertexOld vertex;
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
	/*_Cube* cube = new _Cube(P_CUBE, pos, size, k, color, wireframe_mode);
	Index_vert(cube->vertex_array.data(), 36, &cube->cube_mesh);*/
	//cube->GenFaceNormals(cube->face_centers.data());
	//cube->id = count++;
	//cube->Init();
	//objects.push_back(cube);
	//cubes.push_back(cube);
}

void ModuleGeometries::CreateSphere(float3 pos, float radius, uint def, bool k, Color color)
{
	//_Sphere* sphere = new _Sphere(P_SPHERE, pos, radius, k, color, def, wireframe_mode);
	//Index_vert(sphere->vertex_array.data(), sphere->definition, &sphere->sphere_mesh);
	//sphere->id = count++;
	//sphere->Init();
	//objects.push_back(sphere);
	//spheres.push_back(sphere);

}
