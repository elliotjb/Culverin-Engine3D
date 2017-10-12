#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"
#include <string>

class Inspector : public WindowManager
{
public:

	Inspector();
	virtual ~Inspector();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowInspector();
	void SetInfo(uint m_num, uint v_num, uint f_num, uint t_id, float3 pos, float3 rot, float3 scale);
	void ShowModelInfo() const;
	
	//void OpenClose();
	//bool IsOpen();

	bool model_loaded = false;

private:

	//TRANSFORM INFO
	float3 object_pos = { 0,0,0 };
	float3 object_rot = { 0,0,0 };
	float3 object_scale = { 0,0,0 };

	//GEOMETRY INFO
	uint mesh_num = 0;
	uint vertex_num = 0;
	uint faces_num = 0;

	//TEXTURE INFO
	uint tex_id = 0;
};