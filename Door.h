#ifndef DOOR_H
#define DOOR_H

#include "Cuboid.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


class Door {
private:
    Cuboid doorCuboid;
    glm::vec3 position;          // Position of the door's center
    bool isOpen;                 // Door state: open or closed
    float openSpeed;             // Speed of door opening/closing
    unsigned int textureID;      // Texture ID for the door
    float rotationAngle;         // Current rotation angle
    glm::vec3 initialRotation;
    float targetAngle;           // Target rotation angle (e.g., 90 degrees for open)
    glm::vec3 hingePoint;        // Hinge point for rotation
    float width, height, depth;
    glm::vec3 rotation;

public:
    Door(float width, float height, float depth, glm::vec3 initialPosition, unsigned int texture, glm::vec3 initialRotation)
        : doorCuboid(width, height, depth), position(initialPosition), isOpen(false), openSpeed(2.0f),
        textureID(texture), rotationAngle(0.0f), targetAngle(0.0f), initialRotation(initialRotation) {

        // Adjust hinge point for initial rotation
        hingePoint = initialPosition - glm::vec3(width / 2.0f, 0.0f, 0.0f); // Hinge on X-axis by default

        doorCuboid.SetRotation(initialRotation.x, initialRotation.y, initialRotation.z);
        doorCuboid.SetPosition(initialPosition.x, initialPosition.y, initialPosition.z);
    }

    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);

        // Translate to hinge point
        model = glm::translate(model, hingePoint);

        // Rotate around the Y-axis (assuming Y is up and door opens along X)
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        // Translate back to the door's position, which is now offset by half its width
        model = glm::translate(model, glm::vec3(width / 2.0f, 0.0f, 0.0f)); // Move to the center of the door

        // Finally, translate to the actual position in the world
        model = glm::translate(model, position - hingePoint); // Adjust final position based on hinge point

        // Debugging output
       // std::cout << "Hinge Point: (" << hingePoint.x << ", " << hingePoint.y << ", " << hingePoint.z << ")\n";
        //std::cout << "Model Matrix: " << glm::to_string(model) << std::endl;

        return model;
    }


    void Draw(Shader& shader) {
        // Bind the texture for the door
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Compute transformation matrix for rotation
        glm::mat4 model = GetModelMatrix();
        model = glm::translate(model, position); // Translate to the door's position

        shader.setMat4("model", model);
       // std::cout << "Model Matrix: " << glm::to_string(model) << std::endl;
        shader.Use();

        // Apply the transformation
        doorCuboid.Draw(shader);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void ToggleDoorState() {
        isOpen = !isOpen;
        targetAngle = isOpen ? 90.0f : 0.0f;  // Open: 90 degrees, Closed: 0 degrees
        std::cout << "Door toggled: " << (isOpen ? "Open" : "Closed") << " TargetAngle: " << targetAngle << std::endl;
    }

    void Update(float deltaTime) {
        if (rotationAngle < targetAngle) {
            rotationAngle = glm::min(rotationAngle + openSpeed * deltaTime * 90.0f, targetAngle);
        }
        else if (rotationAngle > targetAngle) {
            rotationAngle = glm::max(rotationAngle - openSpeed * deltaTime * 90.0f, targetAngle);
        }

        // Update the rotation of the door cuboid
        doorCuboid.SetRotation(initialRotation.x, initialRotation.y + rotationAngle, initialRotation.z);
    }

    void SetRotation(float x, float y, float z) {
        rotation = glm::vec3(x, y, z);
    }

    void SetPosition(float x, float y, float z) {
        position = glm::vec3(x, y, z);
    }

    bool IsMouseOver(float mouseX, float mouseY, glm::mat4 viewProjectionMatrix, glm::vec2 screenSize) {
        // Transform door position to clip space
        glm::vec4 clipSpacePos = viewProjectionMatrix * glm::vec4(position, 1.0f);
        if (clipSpacePos.w <= 0.0f) return false; // Not in view

        // Convert clip space to normalized device coordinates (NDC)
        glm::vec3 ndcPos = glm::vec3(clipSpacePos) / clipSpacePos.w;

        // Scale NDC to screen coordinates
        glm::vec2 doorCenter = (glm::vec2(ndcPos.x, ndcPos.y) + 1.0f) * 0.5f * screenSize;

        // Calculate the door's screen size based on its dimensions
        glm::vec2 doorScreenSize = glm::vec2(doorCuboid.GetWidth(), doorCuboid.GetHeight()) * 0.5f * screenSize; // Adjust scaling as needed

        // Check if the mouse is within the bounds of the door on screen
        return (mouseX >= doorCenter.x - doorScreenSize.x && mouseX <= doorCenter.x + doorScreenSize.x &&
            mouseY >= doorCenter.y - doorScreenSize.y && mouseY <= doorCenter.y + doorScreenSize.y);
    }








    void OnMouseClick(float mouseX, float mouseY, glm::mat4 viewProjectionMatrix, glm::vec2 screenSize) {
        if (IsMouseOver(mouseX, mouseY, viewProjectionMatrix, screenSize)) {
            std::cout << "Mouse clicked on door. Toggling door state." << std::endl;
            ToggleDoorState();
        }
    }
};

#endif // DOOR_H
