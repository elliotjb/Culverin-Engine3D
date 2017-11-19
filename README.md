# 3D Engine
This is a project consistent of creating a Game Engine,
that allows you to use tools to create video games.

Last Feature: Move camera without restrictions:
![](https://i.gyazo.com/3b8c3ac28bbc3b1ed5355f736fe57458.gif)

# FEATURES
- Configuration options (Application, Memory Consumption, Window, Camera, Input, Audio, Scene, GUI, Renderer).
- Game Objects Structure (with components & following a hierarchy of parent-childrens).
- Basic GameObject Creation (Cube, Empty object).
- Main Camera (Frustum geometry with modificable parameters).
- Game Clock (Play, Pause & Play Frame Modes).
- In Game Mode, you will se the scene through the main camera you have selected from your gameobjects hierarchy.
- Model importation (Imports meshes and textures, and creates Game Objects with correct components).
- You can select game objects that have a bounding box directly from Scene Screen (or you can select them through 
  hierarchy window).
- Skybox as background of the scene.
- Editable quadtree that will contain static objects. 
- Materials and Meshes components uses Resources to load data only once in memory.

# CAMERA CONTROLS (Unity-like)
- Right Mouse Button: look around with static position
- Left Mouse Button + L-Alt: orbit around a selected object.
- Mouse Wheel: Zoom in / Zoom out.
- Middle Mouse Button: drag and move the camera vertically and horitzontally.
- F: center the Camera on the selected object.

# GIZMOS
Only enabled when Editor Mode is Active.
When you select a game object, its gizmo will be activated, to edit its transform.
Controls:
 - 1: Translate.
 - 2: Rotate.
 - 3: Scale.

# QUADTREE
In Windows->Configuration->Scene you will see QUADTREE options:
- You can enable/disable its Debug Draw.
- You can change the size of the root node with the slider.
- To implement static objects inside it, first create game objects and set them to 'static'.
  Then, make sure all of them are inside the quadtree boundaries and click "UPDATE QUADTREE" button (only when Editor Mode is active).
- The quadtree will correctly be divided and will contain static objects.

#GAME OBJECTS / COMPONENT OPTIONS
You can right-click a game object or a component to see which options are enabled that will modify the selected element.

# STATIC OBJECTS
When seting an object 'static', you won't be able to modify its transform when Play Mode is activated.


# MODEL/TEXTURES IMPORTING
To import a model (.fbx/.obj) to the Engine, just drag it inside the window.
It will create a resource to access its information and operate with it internally.


# UPDATES
- 0.7:
    * Added Gizmos that work well with children objects.
    * Implemented Skybox (2 different skyboxes for now).
    * You can update quadtree to collect static games inside it to speed up frustum culling.
    * Added popups to warn users about Object Deletion, Setting Static an object with childs 
      and another PopUp to warn that it's impossible to set more than one game camera active.
    * GameObjects/Components Optionswhen right clicking them
- 0.6:
    * Added Game Clock (with PLAY/PAUSE/PLAY FREAME buttons).
    * Structured camera with a frustum.
    * Added Game camera to visualize the scene in Game Mode.
    * Added Resources Manager (in progress).
    * Implemented Quadtree.
    * Mouse Picking feature added.
    * Camera culling fixed.
    * Added Guizmos to operate with local transforms of Game Objects.
    * Importing models transformations fixed.
    * Implemented Resources.
    * Add class JSONSerialization, Save & Load: Scenes and Prefabs.
    * Fixed Bugs with Import and Load.
- 0.5.5:
	* Fixed Bugs with transmors.
	* Fixed Bugs with Window Project.
- 0.5.4:
	* Add Window Project:
		> In left window, you can see a hierarchy folders.
		> In right, All files from a folder Test.
	* Add Modules -> Importer and Loader
		> Import and Load Meshes.
		> Import Textures.
	* Add ModuleFs.
	* GameObject & Components Structure:
		> Components: transform, mesh, material and camera.
	*Hierarchy and Inspector Windows work on Game Objects structure.
- 0.5:
	* Implemented diference movement Camera (Now is Like Unity).
	* Add more modules in Configuration Window
	* We can set different textures on each different mesh of the model (if imported correctly).
	* Color of the model can be edited.
	* Debug info 
	* Bounding box, Enable/Disable the model.
- 0.4.1:
	* Add buttom to Revert Style Docking. (v0.4.1.1)
	* Added Basic GameObject - Component Structure (Like Unity Engine)
	* Reestructured Model Importer (Now loads the texture it contains in his data automatically).
	* Implemented Save and Load Doking windows.
	* Render Scene in a window "ImGui".

- 0.4:
	* Add ImGui::Dock
	* Add window Hierarchy and Inspector
	* Add icons (play, pause stop).
	* Drag & Drop funcionality (to import FBX models).
	* Texture 2D implemented.
- 0.3:
	* Add icon .exe (v0.3.2.1)
	* Implemented Assimp (v0.3.2)  
	* Fixed structure Objects (v0.3.2)
	* Add window exit (Save, Don't Save and Cancel) - "(v0.3.1)"
	* Implemented "Render" options inside "Configuration" window
		> Depth test, lightning, color material, wireframe,
		  axis, smooth polygons and fog mode.
	* "Hardware" Window implementation:
		> OpenGL info.
	* Creation of spheres and boxes (in "Creation" Window).
- 0.2:
	* Implemented "Configuration" Window:
		> Frame rate options.
		> Window options (resizable, fullscreen, etc).
		> Volume adjoustment.
	* Added "About us..." Window:
		> Name, description, libraries used.
	* "Hardware" Window:
		> CPU's, GPU descriptions & info.

- 0.1.1:
	* Implemented Delete Objects in Scene

# Libraries Used: 
(Not created by us!)
- MathGeoLib -> http://clb.demon.fi/MathGeoLib/nightly/ 
- ImGui -> https://github.com/ocornut/imgui 
- Bullet -> http://bulletphysics.org 
- Sdl -> http://www.libsdl.org/
- parson (json parser) -> https://github.com/kgabis/parson
- ASSIMP -> http://assimp.sourceforge.net/
- DevIL -> http://openil.sourceforge.net/

# ScreenShots & Gifs
- Scene Example:
![](https://i.gyazo.com/168fc2126497595b762aa51338b36628.jpg)

- Create a GameObject Empty and Add a component Mesh ("Cube"):
![](https://i.gyazo.com/9842ac7895a6548156948e469976cbfa.gif)

- How to Delete a GameObjects:
![](https://i.gyazo.com/094a7c24b98ff87a6cb16992953fc99a.gif)

Authors: 
- Elliot Jimenez: https://github.com/elliotjb
- Jordi Oña: https://github.com/Jordior97

Repository Link: https://github.com/elliotjb/3D-Engine

