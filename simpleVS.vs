#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord; // Texture coordinates attribute

out vec2 TexCoord;
out vec3 FragPos; // Pass fragment position to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    TexCoord = texCoord; // Pass texture coordinates
    FragPos = vec3(model * vec4(position, 1.0)); // Calculate world position
   gl_Position = projection * view * model * vec4(position, 1.0);
  // gl_Position = projection * view * vec4(FragPos, 1.0);
}
