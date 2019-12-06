#version 330

in vec2 frag_uv;
in vec4 frag_color;
uniform sampler2D colormap;

void main()
{
	gl_FragColor=texture(colormap, frag_uv) * frag_color;
}