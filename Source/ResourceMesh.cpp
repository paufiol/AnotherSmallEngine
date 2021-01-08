#include "ResourceMesh.h"
#include "ModuleRenderer3D.h"
#include "Globals.h"
ResourceMesh::ResourceMesh() : Resource(ResourceType::Mesh)
{
    for (uint i = 0; i < NONE; i++)
    {
        ID[i] = 0;
        size[i] = 0;
    }
    aabb.SetNegativeInfinity();
}
ResourceMesh::ResourceMesh(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID) : Resource(ResourceType::Mesh)
{
    this->name = name;
    this->libraryFile = libraryFile;
    this->assetsFile = assetsFile;
    if (UID != 0) this->UID = UID;

    for (uint i = 0; i < NONE; i++)
    {
        ID[i] = 0;
        size[i] = 0;
    }
    aabb.SetNegativeInfinity();
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

void ResourceMesh::SetUpBuffers(ResourceMesh* mesh)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    
    glGenBuffers(1, (GLuint*)&mesh->ID[ResourceMesh::vertex]);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[ResourceMesh::vertex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->size[ResourceMesh::vertex] * 3, mesh->vertices, GL_STATIC_DRAW);

    glGenBuffers(1, (GLuint*)&mesh->ID[ResourceMesh::index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ID[ResourceMesh::index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->size[ResourceMesh::index], mesh->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    if (mesh->size[ResourceMesh::texture] > 0)
    {
        glGenBuffers(1, (GLuint*)&mesh->ID[ResourceMesh::texture]);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[ResourceMesh::texture]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->size[ResourceMesh::texture] * 2, mesh->texCoords, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    if (mesh->size[ResourceMesh::normal] > 0)
    {
        glGenBuffers(1, (GLuint*)&mesh->ID[ResourceMesh::normal]);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[ResourceMesh::normal]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh->size[ResourceMesh::normal] * 3, mesh->normals, GL_STATIC_DRAW);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);
}

void ResourceMesh::CreateAABB()
{
    aabb.SetNegativeInfinity();
    aabb.Enclose((math::vec*)vertices, size[vertex]);
}

