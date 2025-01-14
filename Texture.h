#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <string>
#include "Shader.h"

class Texture
{
public:
    GLuint ID;
    std::string type;
    GLuint unit;
    std::string path;  // Add this line to store the texture file path

    // Update constructor to accept path
    Texture(const std::string& image, const std::string& texType, GLuint slot, const std::string& texPath);

    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();
};

#endif
