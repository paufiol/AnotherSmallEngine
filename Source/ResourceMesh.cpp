#include "ResourceMesh.h"
#include "ModuleRenderer3D.h"

ResourceMesh::ResourceMesh()
{

}
ResourceMesh::~ResourceMesh() {}


void ResourceMesh::DrawNormals()
{
    //glBegin(GL_LINES);

    //for (uint i = 0; i < vertices.size(); ++i)																
    //{
    //    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    //    glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
    //    glVertex3f(vertices[i].position.x + vertices[i].normal.x, vertices[i].position.y + vertices[i].normal.y, vertices[i].position.z + vertices[i].normal.z);
    //}

    //glEnd();
}
void ResourceMesh::DrawTexCoords()
{
  /*  glBegin(GL_POINTS);
    glPointSize(100.0f);
    for (uint i = 0; i < vertices.size(); ++i)
    {
        glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
        glVertex3f(vertices[i].position.x + vertices[i].texCoords.x, vertices[i].position.y + vertices[i].texCoords.y, vertices[i].position.z);
    }

    glPointSize(1.0f);
    glEnd();*/
}

