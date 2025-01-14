#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos; // Interpolated fragment position

uniform sampler2D texture1; // Texture sampler
uniform vec3 cameraPos;     // Camera position
uniform vec3 lightPos;      // Light position

void main()
{
    // Sample the texture at the given coordinates
    vec4 texColor = texture(texture1, TexCoord);

    // Compute the view direction
    vec3 viewDir = normalize(cameraPos - FragPos);

    // Compute the light direction
    vec3 lightDir = normalize(lightPos - FragPos);

    // Calculate diffuse intensity based on the light direction
    float intensity = max(dot(lightDir, vec3(0.0, 1.0, 0.0)), 0.2); // Min ambient value

    // Blend the texture with lighting intensity
    FragColor = texColor * vec4(vec3(intensity), 1.0); // Modulate texture color
}
