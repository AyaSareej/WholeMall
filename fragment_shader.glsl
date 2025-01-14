#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// Uniforms for texture and color
uniform sampler2D faceTexture;
uniform vec4 faceColor;

void main() {
    FragColor = texture(faceTexture, TexCoord) * faceColor;
}
