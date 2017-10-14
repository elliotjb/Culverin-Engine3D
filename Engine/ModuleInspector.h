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
	void ShowModelInfo() const;
	
	//void OpenClose();
	//bool IsOpen();

	bool model_loaded = false;

private:

	_Model* model_ref = nullptr;
	char* model_name = nullptr;



};