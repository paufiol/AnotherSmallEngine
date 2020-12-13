# AnotherSmallEngine
AnotherSmallEngine is pretty self-explanatory, it is a basic 3D Engine developed by 2 students of the [CITM](<https://www.citm.upc.edu/>).
This project is been created as part of CITM's Game Engine course under supervision of professor Marc Garrigó.
* Repository: https://github.com/paufiol/AnotherSmallEngine 
****
## Developers
### Pau Fiol:
[Github account](<https://github.com/paufiol>).
### Aitor Luque:
[Github account](<https://github.com/Aitorlb7>).
****
## Engine Features
* Window configuration options
* OpenGl configuration options
* Render configuration options
* Hierarchy & Inspector
* GUI Docking
* 3D Model Loading
* Texture Loading (without Serialization)
* GameObject and Components
* Camera(s) with frustum culling, able to preview the InGame Camera (In Game Cameras can be modified from Inspector, the editor camer is in Configuration>Camera)
* Unity Like camera movement
* Objects have Bounding Boxes for Optimisation (Bounding Boxes can be Toggled in Configuration>Draw)
* Object transformation via Gizmos
* Custom Style creator for DearImGui
* Serialization:
   * Meshes
   * Textures
   * Models
   * Scene
* Scene Saving (File > Save Scene)
* Resource Management
   * Creation of Meta files
   * All resources handled by their UIDs
   * Importing, serializing and Loading through Resources
   * Premature assets window (only display)
   * Custom File Formats generated within Library

Please Note: The first time executing AnotherSmallEngine everything will Load without errors. Althought from that time on the engine will load from the information saved in our Custom File Format and the texture loading in that case is not ready yet.

Warning: The models won't load if Library folder is erased and the meta files not, We load with the data stored inside the meta which points to the Custom Files stored in Library. --> To fix that erase all ".meta" files inside Assets folder.
 

****
## How to Use
* **Mouse Wheel:** Zooms camera forward and backwards.
* **Alt + Right click:** Rotate camera over mouse position or else selected game object.
* **Alt + Center click(wheel):** Pan camera.
* **Left click:** Select object / Use Gizmo.
* **W:** Set Guizmo to Translate mode
* **E:** Set Guizmo to Rotate mode
* **R:** Set Guizmo to Scale mode
* Guizmo modes can also be set via the Tool submenu in the main bar

****
## Libraries used
* STL (Standard Template Library)
* Dear ImGui
    * ImGuizmo
* OpenGL 
* Glew 
* MatGeoLib
* Assimp
* Parson
* DevIL
****
## Innovations
**Style Window:** *(top right)* The bottom part of it is just summoned by ImGui, but the top section is custom. It allows for picking of a hue via slider plus three main colors in which you can change the Value/Saturation. This is a significantly quicker, and more visually cohesive way to change the theme for ImGui. 

Another feature is the possibility of changing the debug colours for anything drawn in direct mode. This one is not made with aesthetics in mind, accesibility is the main goal (One of our 3rd year colleagues is colorblind, and I would like him to not have issues when working together.) Allowing the user to change colors can help them better understand the info displayed. 

The ultimate goal would be to serialise it and store it, which hasn't been done because that would be a large time sink, and other core features were not achieved.
If the idea is well received I would like to invest some further time into it.
