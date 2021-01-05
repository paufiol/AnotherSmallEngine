
#ifdef __Vertex_Shader__

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec4 ourColor;
out vec2 TexCoord;

uniform vec4 inColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
    ourColor = inColor;
    TexCoord = texCoord;
}

#endif

//--------------------

#ifdef __Fragment_Shader__

in vec4 ourColor;
in vec2 TexCoord;
out vec4 color;
uniform bool hasTexture;
uniform sampler2D ourTexture;
void main()
{
   vec4 texColor = (hasTexture) ? texture(ourTexture, TexCoord) : vec4(1,1,1,1);
   
   color = texColor  * ourColor;

}

#endif


