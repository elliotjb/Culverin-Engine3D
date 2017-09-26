#include "ModuleObjects.h"
#include "Primitive.h"
#include "Globals.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "Application.h"

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
	static bool wire = false;
	static bool axis = false;
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		wire = !wire;
		ShowWire(wire);
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		axis = !axis;
		ShowAxis(axis);
	}

	std::list<PhysBody3D*>::iterator item_sphere = Body_spheres.begin();
	for (int i = 0; i < Body_spheres.size(); i++)
	{
		item_sphere._Ptr->_Myval->GetTransform(&(spheres[i].sphere.transform));
		spheres[i].sphere.Render();
		item_sphere++;
	}

	std::list<PhysBody3D*>::iterator item_cube = Body_cubes.begin();
	for (int i = 0; i < Body_cubes.size(); i++)
	{
		item_cube._Ptr->_Myval->GetTransform(&(cubes[i].cube.transform));
		cubes[i].cube.Render();
		item_cube++;
	}


	return UPDATE_CONTINUE;
}

bool ModuleObjects::CleanUp()
{
	return true;
}

void ModuleObjects::CreateSphere(Sphere_p* sphere, ImVec4 color, bool isKynematic)
{
	if (sphere != NULL)
	{
		Sphere_object temp;
		temp.sphere = *sphere;
		temp.isKynematic = isKynematic;
		temp.sphere.color.r = color.x;
		temp.sphere.color.g = color.y;
		temp.sphere.color.b = color.z;
		temp.sphere.color.a = color.w;
		spheres.push_back(temp);
		if (isKynematic)
		{
			Body_spheres.push_back(App->physics->AddBody(*sphere, 0.0f));
		}
		else
		{
			Body_spheres.push_back(App->physics->AddBody(*sphere));
		}
	}
}

void ModuleObjects::CreateCube(Cube_p* cube, ImVec4 color, bool isKynematic)
{
	if (cube != NULL)
	{
		Cube_object temp;
		temp.cube = *cube;
		temp.isKynematic = isKynematic;
		temp.cube.color.r = color.x;
		temp.cube.color.g = color.y;
		temp.cube.color.b = color.z;
		temp.cube.color.a = color.w;
		cubes.push_back(temp);
		if (isKynematic)
		{
			Body_cubes.push_back(App->physics->AddBox(*cube, 0.0f));
		}
		else
		{
			Body_cubes.push_back(App->physics->AddBox(*cube));
		}
	}
}

void ModuleObjects::ShowAxis(bool axis)
{
	for (int i = 0; i < Body_spheres.size(); i++)
	{
		spheres[i].sphere.axis = axis;
	}
	for (int i = 0; i < Body_cubes.size(); i++)
	{
		cubes[i].cube.axis = axis;
	}
}

void ModuleObjects::ShowWire(bool wire)
{
	for (int i = 0; i < Body_spheres.size(); i++)
	{
		spheres[i].sphere.wire = wire;
	}
	for (int i = 0; i < Body_cubes.size(); i++)
	{
		cubes[i].cube.wire = wire;
	}
}

