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

	std::list<PhysBody3D*>::iterator item = Body_spheres.begin();
	for (int i = 0; i < Body_spheres.size(); i++)
	{
		item._Ptr->_Myval->GetTransform(&(spheres[i].sphere.transform));
		spheres[i].sphere.Render();
		item++;
	}


	return UPDATE_CONTINUE;
}

bool ModuleObjects::CleanUp()
{
	return true;
}

void ModuleObjects::CreateSphere(Sphere_p* sphere, bool isKynematic)
{
	if (sphere != NULL)
	{
		Sphere_object temp;
		temp.sphere = *sphere;
		temp.isKynematic = isKynematic;
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

