#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 cameraPos;
uniform vec3 lightPos;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord);

    // Discard fragments with very low alpha for optimization
    if (texColor.a < 0.1) {
        discard;
    }

    // Compute lighting
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffuse = max(dot(lightDir, vec3(0.0, 1.0, 0.0)), 0.0);

    // Blend the texture color with lighting
    vec3 lighting = texColor.rgb * diffuse;

    // Final fragment color with transparency
    FragColor = vec4(lighting, texColor.a);
}
