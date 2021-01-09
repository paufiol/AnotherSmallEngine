
#ifdef __Vertex_Shader__

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec4 ourColor;
out vec3 ourNormal;
out vec3 ourPosition;

uniform vec4 inColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
	ourNormal = mat3(inverse(transpose(modelMatrix))) * normal;
	//ourNormal = normal;
	
	ourPosition = vec3(modelMatrix * vec4(position, 1.0));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}

#endif

//--------------------

#ifdef __Fragment_Shader__

in vec4 ourColor;
in vec3 ourNormal;
in vec3 ourPosition;

uniform vec3 cameraPosition;
uniform samplerCube skybox;
uniform float refractionIndex = 1.33;

out vec4 color;

uniform bool hasTexture;
uniform sampler2D ourTexture;
void main()
{
	
	float ratio = 1.0f / refractionIndex;
   vec3 I = normalize(ourPosition - cameraPosition);
   vec3 R = refract(I, normalize(ourNormal), ratio);
   
   color = vec4(texture(skybox, R).rgb, 1.0);
	//color = vec4(ourNormal, 1.0); 
}

#endif

















