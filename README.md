# 3D Engine
This is a project consistent of creating a Game Engine,
that allows you to use tools to create video games.
At this moment, this is a Geometry Loader, where you can import your model (.fbx, etc.)
and see info about it. Also you can change/apply textures to the different meshes
that composes the entire model (only .png or .jpg formats).

# FEATURES
- Configuration options (App, Window, Audio, Render).
- Game Objects Structure (with components).
- Basic GameObject Creation (Cube, Sphere).
- Main Camera (Frustum geometry)

# CAMERA CONTROLS(is Like Unity)
- Only Mouse Right: Move Camera with static position
- Mouse Left with Alt-Left: Move Camera arround object (model)
- Mouse wheel: Move Camera forward and backward
- Mouse Middle: Move Camera like 2D.
- F: Center the Camera on an object

# MODEL/TEXTURES IMPORTING
To import a model (.fbx/.obj) to the Engine, just drag it inside the window.
If loaded correctly, you will see an Inspector Window with Basic information of the model.
To load textures to a model (that accept textures), first of all, they have to be .png/.jpg. Then, 
in the Inspector Window, you will see the textures applied to the model in a small square.
The specular channel of the models is only to show the textures that will be shown in specular channel.

TO APPLY/CHANGE A TEXTURE OF A MESH, DRAG AND DROP IT INSIDE THE IMAGE SQUARE OF THE TEXTURE, IN THE INSPECTOR WINDOW.
Visual Explanation Here -> ![](https://i.gyazo.com/d8da58aaf53a70c7a150e8bf1adee4f6.gif)

# UPDATES
- 0.5.4:
	* Add Window Project:
		> In left window, you can see a hierarchy folders.
		> In right, All files from a folder Test.
	* Add Modules Importers and Loads
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

