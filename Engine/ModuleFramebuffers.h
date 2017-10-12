#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	bool Create(int, int);
	void Bind();
	void UnBind();
	void Destroy();
	void Resize(int Width, int Height);
	unsigned int GetTexture() const;

public:

	uint			frame_id = 0;
	uint			rbo;
	uint			texture;
	uint			depth_id = 0;

	int		width;
	int		height;
	float2	size;
};