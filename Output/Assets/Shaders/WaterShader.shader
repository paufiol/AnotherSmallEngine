
#ifdef __Vertex_Shader__

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

#define PI 3.1416
#define g 9.81

out vec4 ourColor;
out vec2 TexCoord;
out vec3 fNormal;
out vec3 fPos;

uniform vec4 inColor;

uniform float time;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float wavelenght_A = 64;
uniform float steepness_A = 0.25; //0 to 1;
uniform vec2 direction_A = vec2(1, 1);

uniform float wavelenght_B = 32;
uniform float steepness_B = 0.25; //0 to 1;
uniform vec2 direction_B = vec2(1, 0.6);

uniform float wavelenght_C = 16;
uniform float steepness_C = 0.25; //0 to 1;
uniform vec2 direction_C = vec2(1, 1.3);

vec3 GerstnerWave(float wavelenght, float steepness, vec2 direction, vec3 pos, inout vec3 tangent, inout vec3 binormal)
{
	
	
	float k = (2 * PI) / wavelenght;
    float a = steepness/ k;
    float c = sqrt(g / k);
    vec2 d = normalize(direction);
    float f  =  (dot(d, pos.xy)  - c * time) * k;
    
    tangent += vec3(
    	 	- (d.x * d.x) * (a * sin(f)), 
    		- (d.x * d.y) * (a * sin(f)),
    		d.x * (a * cos(f))
    );
    
  binormal += vec3(
    	 - (d.x * d.y) *(a * sin(f)),
    	 - (d.y * d.y) *(a * sin(f)),
    	  d.y * (a * cos(f))
    );
    
	return vec3( (cos(f) * a) * d.x,
							(cos(f) * a) * d.y,
							sin(f) * a
	);
}


void main()
{
	vec3 pos;
	pos = position;

    vec3 tangent = vec3( 1, 0, 0);
    vec3 binormal = vec3( 0, 1, 0 );
    
    pos += GerstnerWave(wavelenght_A, steepness_A, direction_A, position, tangent, binormal);
	pos += GerstnerWave(wavelenght_B, steepness_B, direction_B, position, tangent, binormal);
    pos += GerstnerWave(wavelenght_C, steepness_C, direction_C, position, tangent, binormal);
    
    /*
        vec3 old_tangent = normalize(vec3 (
    		1 - steepness * sin(f),
    		0,
    		-  steepness * cos(f)
    ));
    
        vec3 tangent = vec3(
    		- d.x * d.x * (steepness * sin(f)), 
    		- d.x * d.y * (steepness * sin(f)),
    		d.x * (steepness * cos(f))
    );
    
    vec3 binormal = vec3(
    		 - d.x *  d.y *(steepness * sin(f)),
    	 1 - d.y * d.y *(steepness * sin(f)),
    	  d.y * (steepness * cos(f))
    );
    */

    //vec3(-tangent.z, tangent.x, 0);
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0f);
	
	fNormal = normalize(cross(binormal, tangent));
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

uniform float Ka = 0.5;
uniform float Kd = 4.5;
uniform float Ks = 0.0;
uniform float shininess = 100;


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
	vec3 dark_color = vec3(0, 0.2, 0.3);
	vec3 blue = vec3(0,0.5,0.7);
	vec3 foam_color = vec3(0.9,0.95,1);
	float h = 0.9;
	float range = distance(fPos.z/2, -5)/5;
	
    if(fPos.z > texture(ourTexture, TexCoord).r*2 + 1 )
    {
    	float mixvalue = distance(fPos.z/4, 0);
    	lcolor = mix(blue, foam_color, min(mixvalue, 1));
    }
    else
    {
    	float mixvalue = distance(fPos.z/2, -3)/3;
    	//lcolor = mix(dark_color, blue, min(mixvalue, 1));
    	lcolor = blue; 
    	vec4 tex_color =  texture2D(ourTexture, TexCoord);
    	lcolor +=  tex_color.rgb*(0, 0.7, 0.9)*0.4; 
    	
    	lcolor = mix(dark_color, lcolor, min(mixvalue, 1));
	}
	
	/*
		lcolor = h > range 
        ? mix(dark_color, blue, range/h) 
        : mix(blue, foam_color, (range - h)/(1.0 - h));
	
	*/

	vec2 inten = blinnPhongDir(vec3(1, 1, 0), 0.3, Ka, Kd, Ks, shininess);
	
	vec4 tex_color =  texture2D(ourTexture, TexCoord);
	//tex_color =  vec4(1 - tex_color.r, 1 - tex_color.g,1 - tex_color.b, 1) * vec4(1,1,1,1);
	
	//lcolor +=  tex_color.rgb*foam_color*0.4; 
	
	color = vec4(lcolor*inten.x + vec3(1.0) * inten.y, 1.0); //* texture2D(ourTexture, TexCoord);
	
	//color =  texture2D(ourTexture, TexCoord);
	//color = vec4(fNormal.r, fNormal.g, fNormal.b, 1.0);
}

#endif















