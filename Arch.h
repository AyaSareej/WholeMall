#ifndef DOORFRAME_H
#define DOORFRAME_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

class DoorFrame {
private:
    struct Face {
        unsigned int textureID;            // Texture ID for this face
        unsigned int indexCount;          // Number of indices for the face
        unsigned int indexOffset;         // Offset in the index buffer
    };

    std::vector<float> vertices;          // Vertex data (positions + texture coords)
    std::vector<unsigned int> indices;    // Indices for all faces
    std::vector<Face> faces;              // Face-specific texture data

    unsigned int VAO, VBO, EBO;
    glm::vec3 position;                   // Center of the frame
    glm::vec3 dimensions;                 // Dimensions of the outer cuboid
    glm::vec3 holeDimensions;             // Dimensions of the inner rectangular hole
    float bottomHeight;                   // Height of the bottom frame section

    glm::mat4 modelMatrix;                // Model transformation matrix

    void GenerateFrameGeometry() {
        float outerWidth = dimensions.x;
        float outerHeight = dimensions.y;
        float outerDepth = dimensions.z;

        float holeWidth = holeDimensions.x;
        float holeHeight = holeDimensions.y;

        float xStart = -outerWidth / 2.0f;
        float yStart = -outerHeight / 2.0f;
        float zStart = -outerDepth / 2.0f;

        float xHoleStart = -holeWidth / 2.0f;
        float yHoleStart = yStart + bottomHeight;

        float zEnd = zStart + outerDepth;

        // Clear previous data
        vertices.clear();
        indices.clear();
        faces.clear();

        // Bottom section
        AddBox(xStart, yStart, zStart, xStart + outerWidth, yStart + bottomHeight, zEnd, 0);

        // Top section
        AddBox(xStart, yHoleStart + holeHeight, zStart, xStart + outerWidth, yStart + outerHeight, zEnd, 1);

        // Left section
        AddBox(xStart, yHoleStart, zStart, xHoleStart, yHoleStart + holeHeight, zEnd, 2);

        // Right section
        AddBox(xHoleStart + holeWidth, yHoleStart, zStart, xStart + outerWidth, yHoleStart + holeHeight, zEnd, 3);

        modelMatrix = glm::translate(glm::mat4(1.0f), position); // Initialize model matrix
    }

    void AddBox(float x1, float y1, float z1, float x2, float y2, float z2, unsigned int textureID) {
        unsigned int indexStart = vertices.size() / 5;

        // Add vertices
        vertices.insert(vertices.end(), {
            x1, y1, z1, 0.0f, 0.0f, // Bottom-left
            x2, y1, z1, 1.0f, 0.0f, // Bottom-right
            x2, y2, z1, 1.0f, 1.0f, // Top-right
            x1, y2, z1, 0.0f, 1.0f, // Top-left
            x1, y1, z2, 0.0f, 0.0f, // Back-bottom-left
            x2, y1, z2, 1.0f, 0.0f, // Back-bottom-right
            x2, y2, z2, 1.0f, 1.0f, // Back-top-right
            x1, y2, z2, 0.0f, 1.0f  // Back-top-left
            });

        // Add indices
        std::vector<unsigned int> faceIndices = {
            indexStart, indexStart + 1, indexStart + 2, indexStart, indexStart + 2, indexStart + 3, // Front
            indexStart + 4, indexStart + 5, indexStart + 6, indexStart + 4, indexStart + 6, indexStart + 7, // Back
            indexStart, indexStart + 4, indexStart + 7, indexStart, indexStart + 7, indexStart + 3, // Left
            indexStart + 1, indexStart + 5, indexStart + 6, indexStart + 1, indexStart + 6, indexStart + 2, // Right
            indexStart + 3, indexStart + 2, indexStart + 6, indexStart + 3, indexStart + 6, indexStart + 7, // Top
            indexStart, indexStart + 1, indexStart + 5, indexStart, indexStart + 5, indexStart + 4  // Bottom
        };

        // Calculate offsets and counts for this face
        Face face = { textureID, static_cast<unsigned int>(faceIndices.size()), static_cast<unsigned int>(indices.size()) };
        faces.push_back(face);

        // Append indices
        indices.insert(indices.end(), faceIndices.begin(), faceIndices.end());
    }

    void UpdateBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

public:
    DoorFrame(glm::vec3 position, glm::vec3 dimensions, glm::vec3 holeDimensions, float bottomHeight)
        : position(position), dimensions(dimensions), holeDimensions(holeDimensions), bottomHeight(bottomHeight) {
        GenerateFrameGeometry();

        // OpenGL setup
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void Rotate(float angle, glm::vec3 axis) {
        // Start from the base position
        modelMatrix = glm::translate(glm::mat4(1.0f), position);

        // Apply rotation
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
    }


    void Draw(Shader& shader) {
        shader.Use();
        shader.setMat4("model", modelMatrix);

        glBindVertexArray(VAO);

        for (const auto& face : faces) {
            glBindTexture(GL_TEXTURE_2D, face.textureID);
            glDrawElements(GL_TRIANGLES, face.indexCount, GL_UNSIGNED_INT, (void*)(face.indexOffset * sizeof(unsigned int)));
        }

        glBindVertexArray(0);
    }

    void SetFaceTexture(unsigned int faceIndex, unsigned int textureID) {
        if (faceIndex < faces.size()) {
            faces[faceIndex].textureID = textureID;
        }
    }

    ~DoorFrame() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

#endif
