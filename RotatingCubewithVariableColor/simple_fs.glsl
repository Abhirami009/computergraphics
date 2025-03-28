#version 330

out vec4 frag_colour;
in vec3 frag_pos;

void main () 
{
  frag_colour = vec4(frag_pos,1.0);
}
