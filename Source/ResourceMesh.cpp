#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}
ResourceMesh::~ResourceMesh() {}

void ResourceMesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    
    glBindVertexArray(0);

}
void ResourceMesh::DrawNormals()
{
    glBegin(GL_LINES);

    for (uint i = 0; i < vertices.size(); ++i)																
    {
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
        glVertex3f(vertices[i].position.x + vertices[i].normal.x, vertices[i].position.y + vertices[i].normal.y, vertices[i].position.z + vertices[i].normal.z);
    }

    glEnd();
}
void ResourceMesh::DrawTexCoords()
{
    glBegin(GL_POINTS);
    glPointSize(100.0f);
    for (uint i = 0; i < vertices.size(); ++i)
    {
        glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
        glVertex3f(vertices[i].position.x + vertices[i].texCoords.x, vertices[i].position.y + vertices[i].texCoords.y, vertices[i].position.z);
    }

    glPointSize(1.0f);
    glEnd();
}
void ResourceMesh::Draw(GLuint id, GLubyte checker_image[64][64][4])
{
    //DrawTexCoords();
    //DrawNormals();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);

   
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDisable(GL_TEXTURE_2D);
}
