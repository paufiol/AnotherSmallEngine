# AnotherSmallEngine
AnotherSmallEngine is pretty self-explanatory, it is a basic 3D Engine developed by 2 students of the [CITM](<https://www.citm.upc.edu/>).
This project is been created as part of CITM's Game Engine course under supervision of professor Marc Garrigó.
* [Repository](https://github.com/paufiol/AnotherSmallEngine)
* [Site](https://paufiol.github.io/AnotherSmallEngine/)

****
## Developers
### Pau Fiol - [paufiol](<https://github.com/paufiol>).
### Aitor Luque - [Aitorlb7](<https://github.com/Aitorlb7>).
****
## Engine Features
* Window configuration options
* OpenGl configuration options
* Render configuration options
* Hierarchy & Inspector
* GUI Docking
* GameObject and Components
* Camera(s) with frustum culling, able to preview the InGame Camera (In Game Cameras can be modified from Inspector, the editor camer is in Configuration>Camera)
* Unity Like camera movement
* Objects have Bounding Boxes for Optimisation (Bounding Boxes can be Toggled in Configuration>Draw)
* Object transformation via Gizmos
* Custom Style creator for DearImGui
* Serialization:
   * Meshes
   * Materials/Shaders
   * Models
   * Scene
* Scene Saving (File > Save Scene)
* Resource Management
   * Creation of Meta files
   * All resources handled by their UIDs
   * Importing, serializing and Loading through Resources
   * Assets Window and Explorer
   * Custom File Formats generated within Library
   
### New Engine Features
* Shader Pipeline: Load and render 3D models using shaders, and apply textures and materials to them.
  * Use Skybox.
  * Shader Uniforms can be edited from within the inspector.
  * Model, View, Projection Matrices; Time; Camera Position; are all automatically provided to all shaders. 
  * Assign any shader / texture to any given Game Object.
* Text Editor: Click on Edit Shader on the inspector to open a Text Editor. It has color higlighting, basic navigation and text edition commands (Ctrl+C,Ctrl+X,Ctrl+V,Ctrl+Z...). Saving will also recompile the shader to be able to see real-time the changes. Any compilation errors will be shown in the console.
* Water Shader: Vertex shader uses 3 overlapping [Gerstner Waves](https://en.wikipedia.org/wiki/Trochoidal_wave) to achieve the desired shape, and the Fragment Shader uses Height and a Texture (in the showcase scene uses [Worley Noise](https://en.wikipedia.org/wiki/Worley_noise)) To combine colors as well as a bit of reflection.
* Reflection & Reflaction Shader: These are very straightforward shaders once a Skybox is properly setup.

**Warning:** The models won't load if Library folder is erased and the meta files are not, We load with the data stored inside the meta which points to the Custom Files stored in Library. --> To fix that erase all ".meta" files inside Assets folder.
 

****
## How to Use
* **Mouse Wheel:** Zooms camera forward and backwards.
* **Alt + Right click:** Rotate camera over mouse position or else selected game object.
* **Alt + Center click(wheel):** Pan camera.
* **Left click:** Select object / Use Gizmo.
* **DELETE:** delete selected Game Object.
* **W:** Set Guizmo to Translate mode.
* **E:** Set Guizmo to Rotate mode.
* **R:** Set Guizmo to Scale mode.

****
## Libraries used
* STL (Standard Template Library)
* Dear ImGui
    * [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
    * [ImGuiColorTextEdit](https://github.com/BalazsJako/ImGuiColorTextEdit)
* OpenGL 
* Glew 
* MatGeoLib
* Assimp
* Parson
* DevIL
****
## Innovations
* **Skybox, Reflection and Refraction shaders** 
