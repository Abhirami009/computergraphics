#version 330

in vec3 vp;
in vec3 color;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () 
{
  gl_Position = projection * view * model * vec4(vp, 1.0);

  frag_pos = color;
}
