
#ifdef __Vertex_Shader__

layout (location = 0) in vec3 position;


out vec3 TexCoord;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main()
{
    vec4 fullMatrix = modelMatrix * projectionMatrix * viewMatrix * vec4(position, 1.0);
    gl_Position = fullMatrix.xyww;
    TexCoord = position;
}

#endif

//--------------------

#ifdef __Fragment_Shader__

in vec3 TexCoord;
out vec4 color;

uniform samplerCube skybox;

void main()
{
   
   //color = texture(skybox, TexCoord);
    color = vec4(1,0,0,1);
}

#endif








