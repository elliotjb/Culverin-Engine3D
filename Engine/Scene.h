#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleFramebuffers.h"
#include "CompMesh.h"
#include <vector>

class GameObject;

class Scene : public Module
{
public:
	Scene(bool start_enabled = false);
	virtual ~Scene();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void DrawPlane(int size);


	//OBJECTS CREATION ---------------------
	void IndexVertex(float3* vertex_trianglulate, uint num_vertex, CompMesh* mesh);

	GameObject* CreateGameObject();
	GameObject* CreateCube(float3 p, float3 r, float3 s);



public:
	//GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	//_Cube* test = nullptr;

	//GLuint tex;
	//GLuint Lenna;

	//Drawing Scene inside a Dock
	FrameBuffer* frBuff = nullptr;

	//Container Vector of all Game Objects
	std::vector<GameObject*> gameobjects;

};