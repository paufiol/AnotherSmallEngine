
#ifdef __Vertex_Shader__

layout(location = 0) uniform samplerCube skybox;

out vec3 TexCoord;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    TexCoord = texture(skybox, TexCoord);
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
}

#endif

//--------------------

#ifdef __Fragment_Shader__

in vec3 TexCoord;
out vec4 color;

uniform samplerCube skybox;

void main()
{
   
   color =TexCoord;

}

#endif








