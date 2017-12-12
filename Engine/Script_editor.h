#ifndef _SCRIPT_EDITOR_H_
#define _SCRIPT_EDITOR_H_

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"
#include "TextEditor.h"

enum ActionEditor
{
	COPY,
	PASTE,
	UNDO,
	REUNDO,
	RETURN
};

class Script_editor
{
public:
	Script_editor();
	~Script_editor();

	void Start(std::string nameScript, bool isnew = true);
	void SetInitInfo(std::string nameScript);
	void SetExampleInfo();
	void Show(bool& active);

	void SetAction(ActionEditor action);

	void SaveScript();

public:
	TextEditor editor;
	std::string textSelected;
	std::string name;
	std::string directory;
};


#endif
