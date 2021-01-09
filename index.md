# Another Small Engine

Another Small Engine (or ASE for short) is a simple, lightweight Game Engine created for academic purposes. Authors are [Pau Fiol](https://github.com/paufiol) and [Aitor Luque](https://github.com/Aitorlb7), working under the supervision of [Marc Garrigó](https://github.com/markitus18) for the 3D Engines subject coursed in [CITM](https://www.citm.upc.edu/).

Our main focus has been in implementing and showcasing a shader pipeline.

### The Team

- Pau Fiol Lorente - [paufiol](https://github.com/paufiol)
  - Text Editor
  - Shader Programming
  - GUI
  - Time Handling
  
- Aitor Luque Bodet - [Aitorlb7](https://github.com/Aitorlb7)
  - Shader Pipeline
  - Scene Management
  - Resource Management

### Showcase

Add Gifs and video here

### Features

- Shader Pipeline: Geometry drawn via shaders, using Textures and/or materials.
- In-Engine Text Editor and Shader Compilation: Open .shader files (which contain both fragment and vertex shaders) to edit them, and save them to automatically recompile them. If compilation fails, the console will display the compilation error. 
- Shader Uniform edition: Uniform values show up in the inspector and can be edited while running. 
- Mouse Picking & Gizmos: Click to select GameObjects and transform them with Gizmos. 
- Custom File Format: Smaller files containing only the relevant data for our engine. Lighter files, faster execution.
- Resource Management: Assets enter and leave memory as necessary.
- Cameras: Camera culling with a modifiable render distances and FoV. Preview the inGameCamera

## Engine Features
* Window configuration options
* OpenGl configuration options
* Render configuration options
* Hierarchy & Inspector
* GUI Docking
* GameObject and Components
* Camera(s) with frustum culling, able to preview the InGame Camera (In Game Cameras can be modified from Inspector, the editor camer is in Configuration>Camera)
* Unity-like camera movement
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

### Links

- [Source Code](https://github.com/paufiol/AnotherSmallEngine)
- [Releases](https://github.com/paufiol/AnotherSmallEngine/releases)
