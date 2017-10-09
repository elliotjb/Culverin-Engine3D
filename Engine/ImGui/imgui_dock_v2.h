#pragma once
#ifndef _IMGUI_DOCK_V2_
#define _IMGUI_DOCK_V2_

#include "imgui.h"
#include "imgui_internal.h"
#include "../parson.h"


using namespace ImGui;
#define NUMWINDOCK 3

struct  DockContext;

enum Slot_
{
	Slot_Left,
	Slot_Right,
	Slot_Top,
	Slot_Bottom,
	Slot_Tab,

	Slot_Float,
	Slot_None
};

extern void ShutdownDock();
extern void SetNextDock(Slot_ slot);
extern void BeginWorkspace();
extern void EndWorkspace();
extern void SetDockActive();
extern bool BeginDock(const char* label, bool* opened, ImGuiWindowFlags extra_flags);
extern void EndDock();
extern DockContext* getDockContext();



struct DockContext
{
	enum EndAction_
	{
		EndAction_None,
		EndAction_Panel,
		EndAction_End,
		EndAction_EndChild
	};


	enum Status_
	{
		Status_Docked,
		Status_Float,
		Status_Dragged
	};


	struct Dock
	{
		Dock();
		~Dock();

		ImVec2 getMinSize() const;
		bool isHorizontal() const;
		void setParent(Dock* dock);
		Dock& getSibling();
		Dock& getFirstTab();
		Dock& getRoot();
		void setActive();
		bool isContainer() const;
		void setChildrenPosSize(const ImVec2& _pos, const ImVec2& _size);
		void setPosSize(const ImVec2& _pos, const ImVec2& _size);

		char* label;
		ImU32 id;
		Dock* next_tab;
		Dock* prev_tab;
		Dock* children[2];
		Dock* parent;
		bool active;
		ImVec2 pos;
		ImVec2 size;
		Status_ status;
		char location[16];
		bool opened;
		bool first;
		int last_frame;
		int invalid_frames;
	};
	ImVector<Dock*> m_docks;
	ImVec2 m_drag_offset;
	Dock* m_current = nullptr;
	int m_last_frame = 0;
	EndAction_ m_end_action;
	bool m_is_begin_open = false;

	Dock *m_next_parent;
	ImVec2 m_workspace_pos;
	ImVec2 m_workspace_size;
	Slot_ m_next_dock_slot = Slot_Tab;

	DockContext();
	~DockContext();

	Dock& getDock(const char* label, bool opened);

	void putInBackground();

	void splits();

	void checkNonexistent();

	Dock* getDockAt(const ImVec2& pos) const;

	static ImRect getDockedRect(const ImRect& rect, Slot_ dock_slot);

	static ImRect getSlotRect(ImRect parent_rect, Slot_ dock_slot);

	static ImRect getSlotRectOnBorder(ImRect parent_rect, Slot_ dock_slot);

	Dock* getRootDock();

	bool dockSlots(Dock& dock, Dock* dest_dock, const ImRect& rect, bool on_border);

	void handleDrag(Dock& dock);

	void fillLocation(Dock& dock);

	void doUndock(Dock& dock);

	void drawTabbarListButton(Dock& dock);

	bool tabbar(Dock& dock, bool close_button);

	static void setDockPosSize(Dock& dest, Dock& dock, Slot_ dock_slot, Dock& container);

	void doDock(Dock& dock, Dock* dest, Slot_ dock_slot);

	void rootDock(const ImVec2& pos, const ImVec2& size);

	void setDockActive();

	static Slot_ getSlotFromLocationCode(char code);

	static char getLocationCode(Dock* dock);

	void tryDockToStoredLocation(Dock& dock);

	bool begin(const char* label, bool* opened, ImGuiWindowFlags extra_flags);

	void end();

	int getDockIndex(Dock* dock);

	//Save and Load Dock
	bool SaveDock(JSON_Object* config_node, ImU32 position);
	bool LoadDock(JSON_Object* config_node, ImU32 position, bool firstcall);

	Dock* GetDock(int idx);

};

/*

Dock& dock = *m_docks[i];
file << "dock" << (Lumix::u64)&dock << " = {\n";
file << "index = " << i << ",\n";
file << "label = \"" << dock.label << "\",\n";
file << "x = " << (int)dock.pos.x << ",\n";
file << "y = " << (int)dock.pos.y << ",\n";
file << "location = \"" << dock.location << "\",\n";
file << "size_x = " << (int)dock.size.x << ",\n";
file << "size_y = " << (int)dock.size.y << ",\n";
file << "status = " << (int)dock.status << ",\n";
file << "active = " << (int)dock.active << ",\n";
file << "opened = " << (int)dock.opened << ",\n";
file << "prev = " << (int)getDockIndex(dock.prev_tab) << ",\n";
file << "next = " << (int)getDockIndex(dock.next_tab) << ",\n";
file << "child0 = " << (int)getDockIndex(dock.children[0]) << ",\n";
file << "child1 = " << (int)getDockIndex(dock.children[1]) << ",\n";
file << "parent = " << (int)getDockIndex(dock.parent) << "\n";
if (i < m_docks.size() - 1)
file << "},\n";
else
file << "}\n";

std::string di = "Dock" + std::to_string(i) + ".";

json->SetString(di + "ID", std::to_string(i).c_str());
json->SetString(di + "Label", dock.parent ? (dock.label[0] == '\0' ? "DOCK" : dock.label) : dock.status == Status_Float ? dock.label : "ROOT");
json->SetNumber(di + "pos_X", (int)dock.pos.x);
json->SetNumber(di + "pos_Y", (int)dock.pos.y);
json->SetNumber(di + "size_X", (int)dock.size.x);
json->SetNumber(di + "size_Y", (int)dock.size.y);
json->SetNumber(di + "status", (int)dock.status);
json->SetBool(di + "active", dock.active ? true : false);
json->SetBool(di + "opened", dock.opened ? true : false);
fillLocation(dock);
json->SetString(di + "location", strlen(dock.location) ? dock.location : "-1");
json->SetNumber(di + "child0", getDockIndex(dock.children[0]));
json->SetNumber(di + "child1", getDockIndex(dock.children[1]));
json->SetNumber(di + "prev_tab", getDockIndex(dock.prev_tab));
json->SetNumber(di + "next_tab", getDockIndex(dock.next_tab));

if (dock.parent == nullptr && &dock != getRootDock())
{
json->SetNumber(di + "parent", getDockIndex(getRootDock()));
}
else
{
json->SetNumber(di + "parent", getDockIndex(dock.parent));
}

*/
#endif