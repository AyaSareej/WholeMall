#ifndef TEDDY_BEAR_H
#define TEDDY_BEAR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Sphere.h"

class TeddyBear {
public:
    glm::vec3 position; // Position of the teddy bear
    glm::vec3 rotation; // Rotation of the teddy bear (degrees)
    glm::vec3 scale;    // Scaling factor for the teddy bear

    GLuint bodyTexture;     // Texture ID for the body
    GLuint headTexture;     // Texture ID for the head
    GLuint armTexture;      // Texture ID for arms
    GLuint legTexture;      // Texture ID for legs
    GLuint earTexture;      // Texture ID for ears

    // Constructor
    TeddyBear(const glm::vec3& pos = glm::vec3(0.0f),
        const glm::vec3& rot = glm::vec3(0.0f),
        const glm::vec3& scl = glm::vec3(1.0f),
        GLuint bodyTex = 0, GLuint headTex = 0, GLuint armTex = 0,
        GLuint legTex = 0, GLuint earTex = 0)
        : position(pos), rotation(rot), scale(scl),
        bodyTexture(bodyTex), headTexture(headTex),
        armTexture(armTex), legTexture(legTex), earTexture(earTex) {}

    // Setters for position, rotation, scale, and textures
    void SetPosition(const glm::vec3& pos) {
        position = pos;
    }


    void SetRotation(const glm::vec3& rot) {
        rotation = rot;
    }

    void SetScale(const glm::vec3& scl) {
        scale = scl;
    }

    void SetBodyTexture(GLuint tex) {
        bodyTexture = tex;
    }

    void SetHeadTexture(GLuint tex) {
        headTexture = tex;
    }

    void SetArmTexture(GLuint tex) {
        armTexture = tex;
    }

    void SetLegTexture(GLuint tex) {
        legTexture = tex;
    }

    void SetEarTexture(GLuint tex) {
        earTexture = tex;
    }

    // Draw the teddy bear
    void Draw(Shader& shader) {

        //glm::mat4 model = glm::mat4(1.0f);
//model = glm::translate(model, position);
//model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
//model = glm::scale(model, scale);

//shader.setMat4("model", model); // Send the base transform to the shader

        // Draw body parts using spheres
        DrawBodyPart(shader, bodyTexture, glm::vec3(0.0f, -5.0f, 0.0f), 10.0f);  // Body
        DrawBodyPart(shader, headTexture, glm::vec3(0.0f, 8.0f, 0.0f), 5.50f);   // Head
        DrawBodyPart(shader, armTexture, glm::vec3(6.6f, 3.3f, 0.0f), 3.30f);    // Right Arm
        DrawBodyPart(shader, armTexture, glm::vec3(-6.6f, 3.3f, 0.0f), 3.30f);   // Left Arm
        DrawBodyPart(shader, legTexture, glm::vec3(6.4f, -13.0f, 0.0f), 3.30f);  // Right Leg
        DrawBodyPart(shader, legTexture, glm::vec3(-6.4f, -13.0f, 0.0f), 3.30f); // Left Leg
        DrawBodyPart(shader, earTexture, glm::vec3(4.4f, 10.2f, 0.0f), 2.20f);   // Right Ear
        DrawBodyPart(shader, earTexture, glm::vec3(-4.4f, 10.2f, 0.0f), 2.20f);  // Left Ear
    }

private:
    // Helper function to draw individual parts of the teddy bear
    void DrawBodyPart(Shader& shader, GLuint texture, const glm::vec3& offset, float radius) {
        Sphere sphere(radius, 36, 18); // Create a sphere for this part
        sphere.SetTexture(texture);     // Apply the specific texture for this part
        sphere.SetPosition(position.x + offset.x,
            position.y + offset.y,
            position.z + offset.z); // Adjust position
        sphere.Draw(shader);            // Draw the sphere
    }
};

#endif // TEDDY_BEAR_H
