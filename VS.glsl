#version 330

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec4 vertex_normal;
layout (location = 2) in vec2 vertex_uv;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;
uniform vec3 camera_position;

float ka = 0.5f;
float kd = 0.8;
float ks= 5;
float sh= 30;

vec3 light_direction = vec3(1,0,1);
vec4 object_color = vec4(1,1,1,1);
vec4 light_color = vec4(1,0.8,0.8,1);

vec3 v;
vec3 n;
vec3 l;
vec3 h;
float d;

//vec3 camera_position = vec3(0,0,5);


out vec2 frag_uv;
out vec4 frag_color;

void main()
{
	gl_Position = projectionMat * viewMat * modelMat * vertex_position;
	
	l = normalize(light_direction);
	n = normalize(normalize(modelMat * vertex_normal).xyz);
	v = normalize(camera_position - (modelMat * vertex_position).xyz);
	h = normalize(v+l);
	d = length(camera_position - (modelMat * vertex_position).xyz);

	vec4 lightIntensity = ka * light_color + (kd * light_color * max(0, dot(l,n)) + ks * vec4(1,1,1,1) * max(0, pow(dot(h,n),sh)))/ (0.3 * pow(d,2));

	frag_uv = vertex_uv;
	frag_color = object_color * lightIntensity;
}