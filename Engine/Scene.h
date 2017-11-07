#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleFramebuffers.h"
#include "CompMesh.h"
#include "Quadtree.h"
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
	update_status UpdateConfig(float dt);
	bool CleanUp();

	void DrawPlane();

	//OBJECTS CREATION / DELETION ---------------------
	void Init_IndexVertex(float3* vertex_triangulate, uint num_index, CompMesh* mesh);
	GameObject* CreateGameObject(GameObject* parent);
	GameObject* CreateCube(GameObject* parent);
	GameObject* CreateSphere(GameObject* parent);
	GameObject* CreateMainCamera(GameObject* parent);
	void DeleteGameObjects(std::vector<GameObject*>& gameobjects);
	// -------------------------------------

public:
	//GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	//_Cube* test = nullptr;

	//GLuint tex;
	//GLuint Lenna;

	//Drawing Scene & Game inside a Dock
	FrameBuffer* sceneBuff = nullptr;
	FrameBuffer* gameBuff = nullptr;

	//Container Vector of all Game Objects
	std::vector<GameObject*> gameobjects;

	Quadtree quadtree;
	bool quadtree_draw = false;

private:

	int size_plane;
};