#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 inColor;  // Vertex color

out vec3 FragPos;
out vec3 Normal;
out vec3 VertexColor;  // Pass to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    VertexColor = inColor;  // Pass color

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
