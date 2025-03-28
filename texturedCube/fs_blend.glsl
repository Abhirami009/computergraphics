#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    vec4 texColor1 = texture(texture1, TexCoord);
    vec4 texColor2 = texture(texture2, TexCoord);

    // Mix both textures (adjust 0.5 for blending)
    FragColor = mix(texColor1, texColor2, 0.3);
}
