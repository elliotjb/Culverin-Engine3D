# 3D Engine
This is a project consistent of creating a Game Engine,
that allows you to use tools to create video games.

#VERY IMPORTANT!
Make sure that you store the Release in a path without spaces, because the system won't detect successfully the path, and you won't be able to compile any script.

## Example Street scene:
![](https://i.gyazo.com/2fb46188c5188013b7bd76f6e2278bfc.jpg)

# FEATURES
- Configuration options (Application, Memory Consumption, Window, Camera, Input, Audio, Scene, GUI, Renderer).
- Game Objects Structure (with components & following a hierarchy of parent-childrens).
- Basic GameObject Creation (Cube, Empty object).
- Component options (Select between all loaded meshes(MESH COMPONENT), materials (MATERIAL COMPONENT), reset transforms (TRANSFORM COMPONENT), etc.)
- GameObject/Components deletion.
- Main Camera (Frustum geometry with modificable parameters).
- Game Clock (Play, Pause & Play Frame Modes).
- In Game Mode, you will see the scene through the active camera you have selected from your gameobjects hierarchy.
- Models importation (Imports meshes and textures, and creates Game Objects with correct components, following the correct hierarchy).
- Models reimportation (You can modify an imported mesh/texture externally and changes will be applied automatically in the engine).
- You can select game objects that have a bounding box directly from Scene Screen (or you can select them through hierarchy window).
- Skybox as background of the scene.
- Editable quadtree that will contain static objects. 
- Materials and Meshes components uses Resources to load data only once in memory.

# INNOVATIONS
- Basic GameObject Creation: Cube.
- Entering Play Mode enables the Active Game Camera to see the scene from it.
- Copy/Paste of GameObjects: by right clicking them (or Ctrl+C and Ctrl+V).
- Skybox as background: you can choose between 2 predefined skyboxes in Windows->Configuration->Scene.
- Resource Management:
	1) You can import an entire folder in the engine to copy all the files inside it, and see them through "Project" Window.
	2) Reimporting elements: you can import a model/texture, edit them externally, and changes will be applied automatically.
	3) Meta files: importing a model/texture will create a .meta file with information about the imported elemend and resources it uses.
	4) Folders Management: you can create, rename (only for empty folders), and remove folders (it will delete resources if they are inside,
			       so, components that use these resources will be "empty").
- Project window like Unity: with resizable icons and same navigation as Unity: folders on the left, files of the folder on the right.
- Gizmos: enable transform modifications of gameobjects in the scene.
- Mouse without restrictions: when you are moving the editor camera with right-click, if you reach a screen border, the mouse will
  reappear on the extreme side of the screen.

# IMPORTING/REIMPORTING FILES
To import a model (.fbx/.obj), texture (.png/.jpg/.tga) or a folder to the Engine, just drag it inside the window.
It will create all necessary resources to apply to GameObjects/Components. 
To load a model into the scene, double-click the icon that has appeared when the file was dragged. It will create a prefab.

Helper videos about resource managing:
- Importing/Reimporting Models: https://youtu.be/r2dZ5sxbU-U
- Importing/Reimporting Textures: https://youtu.be/rUXQ10-Nwls
- Editing Mesh & Material components: https://youtu.be/rsceOFcBO0c

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

If you have problems to select a game object by clicking on the scene screen, tree to move the 
camera to get a better vision angle.

# QUADTREE
In Windows->Configuration->Scene you will see QUADTREE options:
- You can enable/disable its Debug Draw.
- You can change the size of the root node with the slider.
- To implement static objects inside it, first create game objects and set them to 'static'.
  Then, make sure all of them are inside the quadtree boundaries and click "UPDATE QUADTREE" button (only when Editor Mode is active).
- The quadtree will correctly be divided and will contain static objects.

# CAMERA CULLING
- Only enabled in Play Mode.
- To cull static objects you have to set them 'static' first, then update the quadtree. If they are inside the quadtree, culling for
  static objects in Play Mode is enabled.

# GAME OBJECTS / COMPONENT OPTIONS
You can right-click a game object or a component to see which options are enabled that will modify the selected element.
Warning: for this release, cubes created in the engine hasn't got texture coords, so you won't be able to see the applied texture.

# STATIC OBJECTS
When setting an object 'static', you won't be able to modify its transform when Play Mode is activated.

#VERY IMPORTANT!
Make sure that you store the Release in a path without spaces, because the system won't detect successfully the path, and you won't be able to compile any script.

# UPDATES
- 0.7:
    * Added Gizmos that work well with children objects.
    * Implemented Skybox (2 different skyboxes for now).
    * You can update quadtree to collect static games inside it to speed up frustum culling.
    * Added popups to warn users about Object Deletion, Setting Static an object with childs 
      and another PopUp to warn that it's impossible to set more than one game camera active.
    * GameObjects/Components Options when right clicking them.
    * Resource manager with Import/Reimport functionality.
    * Camera moving without restrictions (only with right-click).
    * Correct Save/Load of the scene when entering/exiting Play Mode.
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


Authors: 
- Elliot Jimenez: https://github.com/elliotjb
- Jordi Oña: https://github.com/Jordior97

Repository Link: https://github.com/elliotjb/3D-Engine

