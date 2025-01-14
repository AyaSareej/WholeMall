#ifndef WALL3D_H
#define WALL3D_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

class Wall3D {
public:
    Wall3D(float width, float height, glm::vec3 position, const std::string& texturePath,
           glm::vec3 doorBottomLeft, glm::vec3 doorBottomRight,
           glm::vec3 doorTopLeft, glm::vec3 doorTopRight);
    void render(const glm::mat4& view, const glm::mat4& projection);
    ~Wall3D();

private:
    GLuint m_vao, m_vbo, m_ebo;
    GLuint m_innerVao, m_innerVbo, m_innerEbo; // New VAO/VBO for the hole
    GLuint m_texture;
    GLuint m_shaderProgram;

    std::vector<unsigned int> m_wallIndices;
    std::vector<unsigned int> m_innerWallIndices; // Indices for the hole

    glm::vec3 m_position;
    float m_width, m_height;

    glm::vec3 m_doorBottomLeft, m_doorBottomRight, m_doorTopLeft, m_doorTopRight;

    void compileShaders();
    void createHoleGeometry(); // New function for the hole geometry
    void checkCompileErrors(GLuint shader, const std::string& type);
};

#endif // WALL3D_H
