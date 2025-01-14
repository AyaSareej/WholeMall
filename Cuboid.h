#ifndef CUBOID_H
#define CUBOID_H

#include <glad/glad.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>



class Cuboid {
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GLuint VBO, VAO, EBO;
    GLuint textures[6]; // One texture for each face
    bool useTexture;    // Whether to use textures or colors
    std::vector<float> faceColors; // Color for each face (R, G, B, A)
    std::vector<bool> faceEnabled;
    glm::vec3 position;
    glm::vec3 rotation;
    float halfWidth, halfHeight, halfDepth; // Dimensions of the cuboid

public:
    // Destructor to clean up OpenGL resources
    ~Cuboid() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    // Constructor to initialize the cuboid dimensions and set up vertices and indices
    Cuboid(float width, float height, float depth)
        : halfWidth(width / 2.0f), halfHeight(height / 2.0f), halfDepth(depth / 2.0f) , position(0.0f), rotation(0.0f), faceEnabled(6, true) {

        // Initialize face colors with white as the default
        faceColors = std::vector<float>(6 * 4, 1.0f); // 6 faces, each with RGBA

        useTexture = false; // By default, use colors, not textures

        // Define vertices and indices
        vertices = {
            // Front face
            -halfWidth, -halfHeight,  halfDepth,  0.0f, 0.0f,
             halfWidth, -halfHeight,  halfDepth,  1.0f, 0.0f,
             halfWidth,  halfHeight,  halfDepth,  1.0f, 1.0f,
            -halfWidth,  halfHeight,  halfDepth,  0.0f, 1.0f,
            // Back face
            -halfWidth, -halfHeight, -halfDepth,  0.0f, 0.0f,
             halfWidth, -halfHeight, -halfDepth,  1.0f, 0.0f,
             halfWidth,  halfHeight, -halfDepth,  1.0f, 1.0f,
            -halfWidth,  halfHeight, -halfDepth,  0.0f, 1.0f,
            // Left face
            -halfWidth, -halfHeight, -halfDepth,  0.0f, 0.0f,
            -halfWidth, -halfHeight,  halfDepth,  1.0f, 0.0f,
            -halfWidth,  halfHeight,  halfDepth,  1.0f, 1.0f,
            -halfWidth,  halfHeight, -halfDepth,  0.0f, 1.0f,
            // Right face
             halfWidth, -halfHeight, -halfDepth,  0.0f, 0.0f,
             halfWidth, -halfHeight,  halfDepth,  1.0f, 0.0f,
             halfWidth,  halfHeight,  halfDepth,  1.0f, 1.0f,
             halfWidth,  halfHeight, -halfDepth,  0.0f, 1.0f,
             // Top face
               -halfWidth,  halfHeight, -halfDepth,  0.0f, 0.0f,
                halfWidth,  halfHeight, -halfDepth,  1.0f, 0.0f,
                halfWidth,  halfHeight,  halfDepth,  1.0f, 1.0f,
               -halfWidth,  halfHeight,  halfDepth,  0.0f, 1.0f,
               // Bottom face
                  -halfWidth, -halfHeight, -halfDepth,  0.0f, 0.0f,
                   halfWidth, -halfHeight, -halfDepth,  1.0f, 0.0f,
                   halfWidth, -halfHeight,  halfDepth,  1.0f, 1.0f,
                  -halfWidth, -halfHeight,  halfDepth,  0.0f, 1.0f,
        };

        indices = {
            0,  1,  2,  2,  3,  0,   // Front face
            4,  5,  6,  6,  7,  4,   // Back face
            8,  9, 10, 10, 11,  8,   // Left face
            12, 13, 14, 14, 15, 12,  // Right face
            16, 17, 18, 18, 19, 16,  // Top face
            20, 21, 22, 22, 23, 20   // Bottom face
        };

        // Generate and bind OpenGL buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture coords
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void SetPosition(float x, float y, float z) {
        position = glm::vec3(x, y, z);
    }
    void SetRotation(float x, float y, float z) {
        rotation = glm::vec3(x, y, z);
    }

    void SetFaceEnabled(int faceIndex, bool enabled) {
        if (faceIndex >= 0 && faceIndex < 6) {
            faceEnabled[faceIndex] = enabled;
        }
    }
    void SetTextureRepeat(float repeatX, float repeatY) {
        for (size_t i = 0; i < vertices.size(); i += 5) {
            vertices[i + 3] *= repeatX; // Scale U (X) texture coordinate
            vertices[i + 4] *= repeatY; // Scale V (Y) texture coordinate
        }

        // Update VBO with new texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    void SetTextureParameters() {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void SetFaceColor(int faceIndex, float r, float g, float b, float a) {
        if (faceIndex >= 0 && faceIndex < 6) {
            faceColors[faceIndex * 4 + 0] = r;
            faceColors[faceIndex * 4 + 1] = g;
            faceColors[faceIndex * 4 + 2] = b;
            faceColors[faceIndex * 4 + 3] = a;
        }
    }

    void SetTextureForFace(int faceIndex, GLuint textureID) {
        if (faceIndex >= 0 && faceIndex < 6) {
            textures[faceIndex] = textureID;
            useTexture = true; // Switch to texture mode
        }
    }

    void DisableTextureForFace(int faceIndex) {
        if (faceIndex >= 0 && faceIndex < 6) {
            textures[faceIndex] = 0; // Clear the texture
        }
    }


    void Draw(Shader& shader) {
        shader.Use(); // Use the shader program

        // Set the model matrix for the cuboid
        glm::mat4 model = glm::mat4(1.0f); // Initialize to identity matrix
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

        shader.setMat4("model", model);

        glBindVertexArray(VAO); // Bind the VAO


        for (int i = 0; i < 6; ++i) {
            if (!faceEnabled[i]) continue; // Skip this face if disabled

            if (useTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[i]);
                shader.setInt("ourTexture", 0);
            }
            else {
                shader.setVec4("faceColor", glm::vec4(faceColors[i * 4], faceColors[i * 4 + 1],
                    faceColors[i * 4 + 2], faceColors[i * 4 + 3]));
            }

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int)));
        }


        glBindVertexArray(0); // Unbind the VAO
    }
    // Get the full width of the cuboid
    float GetWidth() const {
        return halfWidth * 2.0f;
    }

    // Get the full height of the cuboid
    float GetHeight() const {
        return halfHeight * 2.0f;
    }

    // Optionally, you can add a method to get the depth as well
    float GetDepth() const {
        return halfDepth * 2.0f;
    }

    glm::vec3 GetPosition() const {
        return position;
    }

};

#endif // CUBOID_H
