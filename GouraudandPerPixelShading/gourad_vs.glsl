#version 330

// Input attributes
in vec3 vp;        // Vertex position
in vec3 normal;    // Vertex normal
in vec3 color;     // Base color

// Output to Fragment Shader
out vec3 frag_color;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;  // Light position in world space
uniform vec3 viewPos;   // Camera position
uniform vec3 lightColor; // Light color

void main()
{
    // Transform vertex position to world space
    vec4 worldPos = model * vec4(vp, 1.0);
    
    // Transform normal to world space
    vec3 normalWS = normalize(mat3(transpose(inverse(model))) * normal);
    
    // Compute light direction
    vec3 lightDir = normalize(lightPos - worldPos.xyz);
    
    // Compute view direction
    vec3 viewDir = normalize(viewPos - worldPos.xyz);
    
    // Compute ambient component
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    
    // Compute diffuse component
    float diff = max(dot(normalWS, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Compute specular component (Phong model)
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, normalWS);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Final color calculation (Phong Illumination Model)
    frag_color = (ambient + diffuse + specular) * color;

    // Transform vertex position to clip space
    gl_Position = projection * view * worldPos;
}
