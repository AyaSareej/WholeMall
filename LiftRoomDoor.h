#ifndef LIFTDOOROOM_H
#define LIFTDOORROOM_H

#include "Cuboid.h"
#include "Door.h"
#include "Sphere.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class LiftRoomDoor {
private:
    Cuboid doorL;
    Cuboid doorR;

    // State variables
    bool isOpen; // To track if the door is open or closed
    float doorDoorOffset; // How far the door door has been translated
    float maxLiftDoorOffset = 300.0f; // Maximum distance to open the door door
    float doorDoorSpeed = 100.0f;
    glm::vec3 initialDoorRPosition;
    glm::vec3 initialDoorLPosition;
    //




public:
    LiftRoomDoor(float doorWidth, float doorHeight, float depth, glm::vec3 doorPosition, unsigned int outLiftTexture, unsigned int LiftDoorTexture)
        :
        doorL(doorWidth / 2, 700, depth),
        doorR(doorWidth / 2, 700, depth),
        //
        isOpen(false), // Initialize door as closed
        doorDoorOffset(0.0f) // Start with no offset
    {

        for (int i = 0; i < 6; i++) {
            if (i == 0) {
                doorL.SetTextureForFace(i, LiftDoorTexture);
                doorR.SetTextureForFace(i, LiftDoorTexture);
            }
            else if (i == 1) {
                doorL.SetTextureForFace(i, outLiftTexture);
                doorR.SetTextureForFace(i, outLiftTexture);
            }


        }
        // Position the left cuboid
        doorL.SetPosition(doorPosition.x + doorWidth / 4, doorPosition.y + 700 / 2, doorPosition.z + (depth / 2));
        doorR.SetPosition(doorPosition.x - doorWidth / 4, doorPosition.y + 700 / 2, doorPosition.z + (depth / 2));


        /*     std::cout << "Lift Position: ("
                 << Light.GetPosition().x << ", "
                 << Light.GetPosition().y << ", "
                 << Light.GetPosition().z << ")\n";
             */

             // Store the initial position of doorL
        initialDoorLPosition = doorL.GetPosition();
        initialDoorRPosition = doorR.GetPosition();

    }

    void Draw(Shader& shader) {
        doorL.Draw(shader);
        doorR.Draw(shader);
        // Light.Draw(shader);
    }

    void Update(float deltaTime) {
        // Smoothly open or close the doors
        if (isOpen && doorDoorOffset < maxLiftDoorOffset) {
            doorDoorOffset += doorDoorSpeed * deltaTime;
            if (doorDoorOffset > maxLiftDoorOffset) doorDoorOffset = maxLiftDoorOffset;
        }
        else if (!isOpen && doorDoorOffset > 0.0f) {
            doorDoorOffset -= doorDoorSpeed * deltaTime;
            if (doorDoorOffset < 0.0f) doorDoorOffset = 0.0f;
        }

        doorL.SetPosition(
            initialDoorLPosition.x + doorDoorOffset  - 25.0f,
            initialDoorLPosition.y,
            initialDoorLPosition.z
        );
        doorR.SetPosition(
            initialDoorRPosition.x - doorDoorOffset - + 25.0f,
            initialDoorRPosition.y,
            initialDoorRPosition.z
        );
    }



    void HandleInput(int key) {
        if (key == GLFW_KEY_SLASH) { // Start movement]
            ToggleLift();
        }
        
    }



    void ToggleLift() {
        isOpen = !isOpen; // Toggle the door state

    }

    //void OnMouseClick(float mouseX, float mouseY, glm::mat4 viewProjectionMatrix, glm::vec2 screenSize) {
    //    ToggleLift(); // Handle door click
    //}
};



#endif 
