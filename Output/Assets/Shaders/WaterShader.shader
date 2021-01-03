
#ifdef __Vertex_Shader__

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

#define PI 3.1416

out vec4 ourColor;
out vec2 TexCoord;
out vec3 fNormal;
out vec3 fPos;

uniform vec4 inColor;

uniform float time;
vec3 pos;

uniform mat4 model_matrix;
uniform mat4 view;
uniform mat4 projection;

uniform float wavelenght = 0.2;
uniform float frequency;
uniform float amplitude = 5;

void main()
{
	pos = position;
	pos.z += sin(pos.x * wavelenght -  time) * amplitude;
     
	float k = 2 * PI / wavelenght;
    
    float f  =  (pos.x * wavelenght - time);
    vec3 tangent = normalize(vec3(1, 0,  amplitude * cos(f)));
    vec3 wave_normal = vec3(-tangent.z, tangent.x, 0);
    
    gl_Position = projection * view * model_matrix * vec4(pos, 1.0f);
	
	fNormal = wave_normal;
    ourColor = inColor;
    TexCoord = texCoord;
    fPos = pos;
}

#endif

//--------------------

#ifdef __Fragment_Shader__

in vec4 ourColor;
in vec2 TexCoord;
in vec3 fNormal;
in vec3 fPos;
out vec4 color;

uniform float contrast;

uniform sampler2D ourTexture;

vec2 blinnPhongDir(vec3 lightDir, float lightInt, float Ka, float Kd, float Ks, float shininess)
{
	vec3 s = normalize(lightDir);
	vec3 v = normalize(-fPos);
	vec3 n = normalize(fNormal);
	vec3 h = normalize(v+s);
	float diffuse = Ka + Kd * lightInt * max(0.0, dot(n, s));
	float spec = Ks * pow(max(0.0, dot(n,h)), shininess);
	return vec2(diffuse, spec);
}

void main()
{

	vec3 lcolor;
	lcolor = vec3(0.9,0.95,1);
    if(fPos.z > 2.5)
    {
    	lcolor = vec3(0.9,0.95,1);
    }
    else
    {
    	lcolor = vec3(0,0.2,0.7);
	}
	vec2 inten = blinnPhongDir(vec3(1,0,0), 0.5, 0.2, 0.8, 0.8, 80.0);
	
	color = vec4(lcolor*inten.x + vec3(1.0) * inten.y, 1.0);
	
	//color = vec4(fNormal,1.0);
}

#endif





