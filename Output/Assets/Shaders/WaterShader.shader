
#ifdef __Vertex_Shader__

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec4 ourColor;
out vec2 TexCoord;

uniform vec4 inColor;

uniform mat4 model_matrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model_matrix * vec4(position, 1.0f);
    ourColor = inColor;
    TexCoord = texCoord;
}

#endif

//--------------------

#ifdef __Fragment_Shader__

in vec4 ourColor;
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture;
void main()
{
    //color = texture(ourTexture, TexCoord) * ourColor;
    color = vec4(0, 0, 1, 1);
}

#endif