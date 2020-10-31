#ifndef __ResourceMesh_H__
#define __ResourceMesh_H__

#include "glmath.h"
#include <OpenGL.h>
#include "Globals.h"

using namespace std;

class ResourceMesh {
public:


    ResourceMesh();
    ~ResourceMesh();

    void Draw();
    void DrawNormals();
    void DrawTexCoords();




};
#endif //__ResourceMesh_H__