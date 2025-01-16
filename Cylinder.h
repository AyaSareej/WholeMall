#ifndef CYLINDER_H
#define CYLINDER_H

#include <glad/glad.h>
#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#define _USE_MATH_DEFINES
#include <math.h>



class Cylinder {
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GLuint VBO, VAO, EBO;
    GLuint texture;          // Texture ID for the cylinder
    bool useTexture;         // Whether to use texture or face color
    glm::vec4 faceColor;     // Color of the cylinder
    glm::vec3 position;      // Position of the cylinder
    glm::vec3 rotation;      // Rotation of the cylinder
    float radius, height;    // Radius and height of the cylinder
    int sectors;             // Number of sectors (sides)

public:
    // Destructor to clean up OpenGL resources
    ~Cylinder() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    // Default constructor
    Cylinder() : radius(1.0f), height(1.0f), sectors(36), position(0.0f), rotation(0.0f), useTexture(false), faceColor(1.0f, 1.0f, 1.0f, 1.0f) {
        generateVertices();
        setupBuffers();
    }

    // Constructor to initialize the cylinder dimensions and setup buffers
    Cylinder(float radius, float height, int sectors)
        : radius(radius), height(height), sectors(sectors), position(0.0f), rotation(0.0f), useTexture(false), faceColor(1.0f, 1.0f, 1.0f, 1.0f) {
        generateVertices();
        setupBuffers();
    }

    // Set the position of the cylinder
    void SetPosition(const glm::vec3& pos) {
        position = pos;
    }

    void SetPosition(float x, float y, float z) {
        position = glm::vec3(x, y, z);
    }


    // Set the rotation of the cylinder
    void SetRotation(float x, float y, float z) {
        rotation = glm::vec3(x, y, z);
    }

    // Set the color of the cylinder
    void SetColor(float r, float g, float b, float a = 1.0f) {
        faceColor = glm::vec4(r, g, b, a);
        useTexture = false;
    }

    // Set the texture for the cylinder
    void SetTexture(GLuint textureID) {
        texture = textureID;
        useTexture = true;
    }

    // Draw the cylinder
    void Draw(Shader& shader) {
        shader.Use(); // Use the shader program

        // Set the model matrix for the cylinder
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        shader.setMat4("model", model);

        glBindVertexArray(VAO);

        if (useTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            shader.setInt("ourTexture", 0);
        }
        else {
            shader.setVec4("faceColor", faceColor);
        }

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

private:
    // Generate vertices and indices for the cylinder
    void generateVertices() {
        float halfHeight = height / 2.0f;

        // Generate vertices for top and bottom circles and the sides
        for (int i = 0; i <= sectors; ++i) {
            float angle = i * 2.0f * 3.14159265359f / sectors;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            // Top circle
            vertices.push_back(x);
            vertices.push_back(halfHeight);
            vertices.push_back(z);
            vertices.push_back(i / (float)sectors); // Texture coordinate u
            vertices.push_back(1.0f);             // Texture coordinate v

            // Bottom circle
            vertices.push_back(x);
            vertices.push_back(-halfHeight);
            vertices.push_back(z);
            vertices.push_back(i / (float)sectors); // Texture coordinate u
            vertices.push_back(0.0f);              // Texture coordinate v
        }

        // Center points for top and bottom circles
        vertices.push_back(0.0f); vertices.push_back(halfHeight); vertices.push_back(0.0f);
        vertices.push_back(0.5f); vertices.push_back(1.0f);

        vertices.push_back(0.0f); vertices.push_back(-halfHeight); vertices.push_back(0.0f);
        vertices.push_back(0.5f); vertices.push_back(0.0f);

        // Generate indices for sides
        for (int i = 0; i < sectors; ++i) {
            int top1 = i * 2;
            int bottom1 = top1 + 1;
            int top2 = (i + 1) % sectors * 2;
            int bottom2 = top2 + 1;

            // Side quad
            indices.push_back(top1);
            indices.push_back(bottom1);
            indices.push_back(top2);

            indices.push_back(bottom1);
            indices.push_back(bottom2);
            indices.push_back(top2);
        }

        // Generate indices for top and bottom circles
        int topCenter = sectors * 2;
        int bottomCenter = topCenter + 1;

        for (int i = 0; i < sectors; ++i) {
            int top1 = i * 2;
            int top2 = (i + 1) % sectors * 2;

            int bottom1 = top1 + 1;
            int bottom2 = top2 + 1;

            // Top triangle
            indices.push_back(top1);
            indices.push_back(top2);
            indices.push_back(topCenter);

            // Bottom triangle
            indices.push_back(bottom1);
            indices.push_back(bottomCenter);
            indices.push_back(bottom2);
        }
    }

    // Setup OpenGL buffers
    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture coords
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

#endif // CYLINDER_H
