#include "WallHole.h"
#include "TextureLoader.h" // Include the existing TextureLoader header
#include <iostream>


const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}
)";

void Wall3D::createHoleGeometry() {
    // Define vertices for the hole (inner rectangle)
    float holeVertices[] = {
        m_doorBottomLeft.x, m_doorBottomLeft.y, 0.0f, 0.0f, 0.0f,  // Bottom-left
        m_doorBottomRight.x, m_doorBottomRight.y, 0.0f, 1.0f, 0.0f, // Bottom-right
        m_doorTopLeft.x, m_doorTopLeft.y, 0.0f, 0.0f, 1.0f,         // Top-left
        m_doorTopRight.x, m_doorTopRight.y, 0.0f, 1.0f, 1.0f       // Top-right
    };

    unsigned int holeIndices[] = {
        0, 1, 2, // Bottom-left triangle
        2, 1, 3  // Top-right triangle
    };

    m_innerWallIndices = std::vector<unsigned int>(std::begin(holeIndices), std::end(holeIndices));

    // Set up buffers for the hole
    glGenVertexArrays(1, &m_innerVao);
    glGenBuffers(1, &m_innerVbo);
    glGenBuffers(1, &m_innerEbo);

    glBindVertexArray(m_innerVao);

    glBindBuffer(GL_ARRAY_BUFFER, m_innerVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(holeVertices), holeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_innerEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_innerWallIndices.size() * sizeof(unsigned int), m_innerWallIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


Wall3D::Wall3D(float width, float height, glm::vec3 position, const std::string& texturePath,
    glm::vec3 doorBottomLeft, glm::vec3 doorBottomRight, glm::vec3 doorTopLeft, glm::vec3 doorTopRight)
    : m_width(width), m_height(height), m_position(position),
    m_doorBottomLeft(doorBottomLeft), m_doorBottomRight(doorBottomRight),
    m_doorTopLeft(doorTopLeft), m_doorTopRight(doorTopRight) {


    float halfWidth = width / 2;
    float halfHeight = height / 2;


    // Define vertices for the wall with a hole
    float vertices[] = {
        // Left side of wall
        -width, -height, 0.0f, 0.0f, 0.0f,
        -width / 2, -height , 0.0f, 0.0f, 1.0f,
        -width / 2, -height / 3, 0.0f, 0.0f, 1.0f,
        -width, -height / 3, 0.0f, 0.0f, 0.0f,
        // Right side of wall
        width/2, height, 0.0f, 1.0f, 1.0f,
        width , height , 0.0f, 1.0f, 1.0f,
        width, height/3, 0.0f, 1.0f, 0.0f,
        width/2, height / 3, 0.0f, 1.0f, 0.0f,
        //top
        -width, -height/3, 0.0f, 1.0f, 1.0f,
        width, height/3, 0.0f, 1.0f, 0.0f,
        width, height , 0.0f, 1.0f, 0.0f,
        -width, height , 0.0f, 1.0f, 0.0f,

    };


    // Define indices for the wall, excluding the hole
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
        //
        7, 8, 6,
        6 ,9 ,7,
        //
        1,4,5,
        5,6,1
    };



    m_wallIndices = std::vector<unsigned int>(std::begin(indices), std::end(indices));

    // Set up buffers
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_wallIndices.size() * sizeof(unsigned int), m_wallIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load texture
    m_texture = loadTexture(texturePath.c_str());

    compileShaders();
}





void Wall3D::render(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(m_shaderProgram);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Enable stencil testing
    glEnable(GL_STENCIL_TEST);

    // Step 1: Draw the main wall, writing 1 to the stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Always pass stencil test
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Replace stencil value with 1
    glStencilMask(0xFF); // Enable writing to stencil buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_wallIndices.size(), GL_UNSIGNED_INT, 0);

    // Step 2: Draw the hole (inner wall), masking it in the stencil buffer
    glStencilFunc(GL_ALWAYS, 0, 0xFF); // Always pass stencil test, but write 0
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Replace stencil value with 0
    glStencilMask(0xFF); // Enable writing to stencil buffer

    glBindVertexArray(m_innerVao);
    glDrawElements(GL_TRIANGLES, m_innerWallIndices.size(), GL_UNSIGNED_INT, 0);

    // Step 3: Use stencil buffer to render only areas where stencil != 0
    glStencilFunc(GL_NOTEQUAL, 0, 0xFF); // Pass stencil test if stencil value != 0
    glStencilMask(0x00); // Disable writing to stencil buffer
    glDisable(GL_DEPTH_TEST); // Disable depth testing for simplicity

    // Redraw the main wall texture, excluding the hole
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_wallIndices.size(), GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST); // Re-enable depth testing
    glDisable(GL_STENCIL_TEST); // Disable stencil testing
}





Wall3D::~Wall3D() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Wall3D::compileShaders() {
    GLuint vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    // Compile vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Compile fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Create shader program and link
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    checkCompileErrors(m_shaderProgram, "PROGRAM");

    // Clean up shaders as they are no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Wall3D::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    char infoLog[512];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}
