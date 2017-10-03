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
	perf_timer.Start();

	//CreateSphere_p
	// --------------------------------------------

	/*std::list<PhysBody3D*>::iterator item_sphere = Body_spheres.begin();
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
	}*/


	/*for (int i = 0; i < spheres_mathgeo.size(); i++)
	{
		RenderSphere(&spheres_mathgeo[i].sphere, spheres_mathgeo[i].color, wireframe_mode);
	}*/
	for (int i = 0; i < sphere_ID.size(); i++)
	{
		Render(sphere_ID[i]);
	}
	
	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

bool ModuleObjects::CleanUp()
{
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


void ModuleObjects::Render(Object_id id)
{
	switch (id.type)
	{
	case SPHERE:
		RenderSphere(id);
		break;
	case CUBE:
		RenderCube(id);
		break;
	default:
		break;
	}
}

void ModuleObjects::CreateSphere(Sphere* sphere, Color color, uint definition/*, bool isKynematic*/)
{
	if (sphere != NULL)
	{
		Sphere* sphe = sphere;
		uint num_vertex = 24;
		//24, 96, 384, 1536,
		switch (definition)
		{
		case 1:
			*(int *)&num_vertex = 24;
			//def = 24;
			break;
		case 2:
			*(int *)&num_vertex = 96;
			//def = 96;
			break;
		case 3:
			*(int *)&num_vertex = 384;
			//def = 384;
			break;
		case 4:
			*(int *)&num_vertex = 1536;
			//def = 1536;
			break;
		case 5:
			*(int *)&num_vertex = 6144;
			//def = 1536;
			break;
		default:
			break;
		}
		//float3 array_v1[384];
		std::vector<float3>array_v1;
		array_v1.reserve(num_vertex);
		//array_v1[num_vertex];
		//float3 array_v2[num_vertex];
		sphe->Triangulate(array_v1.data(), NULL, NULL, num_vertex, false);
		std::vector<GLuint> index;
		std::vector<math::float3> my_vertex;
		Index_vert(array_v1.data(), num_vertex, &index, &my_vertex);

		GLuint ver;
		glGenBuffers(1, &ver);
		glBindBuffer(GL_ARRAY_BUFFER, ver);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * my_vertex.size() * 3, my_vertex.data(), GL_STATIC_DRAW);

		GLuint ver2;
		glGenBuffers(1, &ver2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ver2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * index.size(), index.data(), GL_STATIC_DRAW); // NORMAL NEED RIGHT?
		Object_id temp;
		temp.id = ver;
		temp.index = ver2;
		temp.type = SPHERE;
		temp.color = color;
		temp.num_vertices = num_vertex;
		sphere_ID.push_back(temp);

		/*if (isKynematic)
		{
			Body_spheres_mathgeo.push_back(App->physics->AddBody(*sphere, 0.0f));
		}
		else
		{
			Body_spheres_mathgeo.push_back(App->physics->AddBody(*sphere));
		}*/
	}
}

bool ModuleObjects::Index_vert(float3* vertex_triangulate, uint num_vertex, std::vector<uint>* index, std::vector<float3>* vertex)
{
	std::vector<float3> my_index;
	for (int i = 0; i < num_vertex; i++)
	{
		bool temp = false;
		uint size = my_index.size();
		for (int j = 0; j < size; j++)
		{
			if (my_index[j] == vertex_triangulate[i])
			{
				index->push_back(j);
				temp = true;
			}
		}
		if (temp == false)
		{
			my_index.push_back(vertex_triangulate[i]);
			index->push_back(my_index.size() - 1);
			vertex->push_back(vertex_triangulate[i]);
		}
	}
	return true;
}

void ModuleObjects::CreateCube(OBB* cube, Color color/*, bool isKynematic*/)
{
	// Generate Cube without index!
	/*OBB* box = cube;
	float3 array_v1[36];
	float3 array_v2[36];
	box->Triangulate(1, 1, 1, array_v1, array_v2, NULL, false);
	GLuint ver;
	glGenBuffers(1, &ver);
	glBindBuffer(GL_ARRAY_BUFFER, ver);
	glBufferData(GL_ARRAY_BUFFER, sizeof(array_v1), &array_v1, GL_STATIC_DRAW);

	Object_id temp;
	temp.id = ver;
	temp.type = CUBE;
	sphere_ID.push_back(temp);
	*/
	//Generate Cube With Index
	OBB* boxed = cube;
	AABB boxer(*boxed);
	AABB* box = &boxer;
	//float3 array_v1[36];
	std::vector<float3>array_v1;
	array_v1.reserve(36);
	//box->GetCornerPoints(array_v1);
	box->Triangulate(1, 1, 1, array_v1.data(), NULL, NULL, false);
	/*std::vector<unsigned int> index;
	index = {
		// left
		0, 1, 2,
		2, 1, 3,
		// front
		3, 1, 7,
		7, 1, 5,
		// right
		7, 5, 6,
		6, 5, 4,
		// back
		6, 4, 0,
		0, 2, 6,
		// top
		2, 3, 7,
		7, 6, 2,
		// down
		1, 0, 5,
		0, 4, 5,
	};*/
	std::vector<int> normal;
	normal = {
		// left
		1, 0, 0,
		// front
		-1, 0, 0,
		// right
		1, 0, 0,
		// back
		-1, 0, 0,
		// top
		0, 1, 0,
		// down
		0, -1, 0,
	};

	GLuint my_indices;
	GLuint my_vertex;
	GLuint my_normal;

	std::vector<uint> index;
	std::vector<math::float3> my_vertex22;
	Index_vert(array_v1.data(), 36, &index, &my_vertex22);

	glGenBuffers(1, (GLuint*)&my_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, my_vertex);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(array_v1) * 3, &array_v1[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * my_vertex22.size() * 3, my_vertex22.data(), GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*)&my_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *index.size(), &index[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * index.size(), index.data(), GL_STATIC_DRAW); // NORMAL NEED RIGHT?


/*	// Buffer for indices
	glGenBuffers(1, (GLuint*)&my_normal);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_normal);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *normal.size(), &normal[0], GL_STATIC_DRAW);*/

	Object_id temp;
	temp.id = my_vertex;
	temp.index = my_indices;
	//temp.normal = my_normal;
	temp.num_vertices = 36;
	temp.type = CUBE;
	temp.color = color;
	sphere_ID.push_back(temp);



}

void ModuleObjects::RenderSphere(Object_id id/*, bool wire*/)
{
	if (wireframe_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);
	/*glBindBuffer(GL_ARRAY_BUFFER, id.index); NORMAL??
	glNormalPointer(GL_FLOAT, 0, NULL);*/
	glBindBuffer(GL_ARRAY_BUFFER, id.id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id.index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glColorPointer(3, GL_FLOAT, 1, &color);
	// … draw other buffers
	glColor4f(id.color.r, id.color.g, id.color.b, id.color.a);
	//glDrawArrays(GL_TRIANGLES, 0, 512 * 3);
	glDrawElements(GL_TRIANGLES, sizeof(GLuint) * id.num_vertices / 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	if (wireframe_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void ModuleObjects::RenderCube(Object_id id/*, ImVec4 color*/)
{
	if (wireframe_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glColorPointer(3, GL_FLOAT, 1, &color);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id.id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id.index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glBindBuffer(GL_NORMAL_ARRAY, id.normal);

	//glColor4f(id.color.r, id.color.g, id.color.b, id.color.a);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glNormalPointer(GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, sizeof(GLuint) * id.num_vertices / 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	if (wireframe_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

/*void ModuleObjects::CreateSphere(Sphere_p* sphere, ImVec4 color, bool isKynematic)
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
}*/

/*void ModuleObjects::CreateCube(Cube_p* cube, ImVec4 color, bool isKynematic)
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
}*/
