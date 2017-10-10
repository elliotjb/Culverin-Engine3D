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

	unsigned int			frame_id = 0;
	unsigned int			rbo;
	unsigned int			texture;
	unsigned int			depth_id = 0;

	int		width;
	int		height;
};