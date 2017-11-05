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


	//OBJECTS CREATION ---------------------
	void Init_IndexVertex(float3* vertex_triangulate, uint num_index, CompMesh* mesh);
	GameObject* CreateGameObject();
	void DeleteAllGameObject(std::vector<GameObject*> gameobjects);
	GameObject* CreateCube();
	GameObject* CreateSphere();
	GameObject* CreateMainCamera();

	void SaveScene();
	void SaveChildGameObject(JSON_Object * config_node, const GameObject & gameObject, uint& count);
	void LoadScene();
	void LoadChildsGameObject(GameObject & parent, GameObject & child, uint uuidParent);


public:
	//GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	//_Cube* test = nullptr;

	//GLuint tex;
	//GLuint Lenna;

	//Drawing Scene inside a Dock
	FrameBuffer* frBuff = nullptr;

	//Container Vector of all Game Objects
	std::vector<GameObject*> gameobjects;

private:
	Quadtree quadtree;
	bool quadtree_draw = false;
	int size_plane;
};