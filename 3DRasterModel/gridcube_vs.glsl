#version 330 core
layout (location = 0) in vec3 vp;     // Vertex position
layout (location = 1) in vec3 vColor; // Vertex color (for cube only)

out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int isGrid;
uniform int isCube;
uniform vec3 gridColor;
uniform vec3 col;

void main() {
    gl_Position = projection * view * model * vec4(vp, 1.0);
    
    if (isGrid == 1) 
        fragColor = gridColor;  // Use uniform color for grid
    else if (isCube == 1)
        fragColor = vColor;      // Use per-vertex color for cube
	else
		fragColor = col;
}
