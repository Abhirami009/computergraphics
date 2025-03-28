#version 330

in vec3 vp;
in vec3 color;
out vec3 frag_pos;

void main () 
{
  gl_Position = vec4 (vp, 1.0);
  frag_pos = color;
}
