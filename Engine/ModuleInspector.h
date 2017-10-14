#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"
#include <string>

class _Model;

class Inspector : public WindowManager
{
public:

	Inspector();
	virtual ~Inspector();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowInspector();
	void SetInfo(_Model* model);
	void SetTexInfo(_Model* model);
	void ShowModelInfo();
	
	//void OpenClose();
	//bool IsOpen();

	bool model_loaded = false;

	int mesh_index = -1;
	int tex_index = -1;
	int tex_id = -1;

private:
	_Model* model_ref = nullptr;
	char* model_name = nullptr;
};