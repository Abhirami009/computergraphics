// #version 330

// out vec4 frag_colour;
// in vec3 frag_pos;

// void main () 
// {
//   frag_colour = vec4(frag_pos,1.0);
// }

#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, TexCoord);
}
