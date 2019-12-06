#version 330

layout (location = 0) in vec4 vertex_position;

out vec4 frag_color;
void main()
{
	gl_Position = vec4 (vertex_position.x, sin(vertex_position.x * 4), 0, 1);
	frag_color = vec4 (1, 1, 1, 1);
}