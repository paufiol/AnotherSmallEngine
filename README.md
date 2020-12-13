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
* Texture Loading
* Drag & Drop of Models and Textures
* GameObject and Components
* Resource Management
    * Assets window
    * Custom File Formats generated within Library on file import
* Camera(s) with frustum culling
* Unity Like camera movement
* Objects have Bounding Boxes for Optimisation
* Object transformation via Gizmos

****
## How to Use
* **Mouse Wheel:** Zooms camera forward and backwards.
* **Alt + Right click:** Rotate camera over mouse position or else selected game object.
* **Alt + Center click(wheel):** Pan camera.
* **Left click:** Select object / Use Gizmo.
* **W:** Set Guizmo to Translate mode
* **E:** Set Guizmo to Rotate mode
* **R:** Set Guizmo to Scale mode

****
## Libraries used
* STL (Standard Template Library)
* ImGui
    * ImGuizmo
* OpenGL 
* Glew 
* MatGeoLib
* Assimp
* Parson
* DevIL
****
## Innovations
* **Style Window:** *(top right)* The bottom part of it is just summoned by ImGui, but the top section is custom. It allows for picking of a hue via slider plus three main colors in which you can change the Value/Saturation. This is a significantly quicker, and more visually cohesive way to change the theme for ImGui.  
