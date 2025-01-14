#ifndef LIFT_H
#define LIFT_H

#include "Cuboid.h"
#include "Door.h"
#include "Sphere.h"
#include <glm/glm.hpp>

class Lift {
private:
    Cuboid outTheLift;
    Cuboid mainCuboid; // inner lift cube
    Cuboid doorL;
    Cuboid doorR;

    //
    Sphere Light;

    // State variables
    bool isOpen; // To track if the lift is open or closed
    float liftDoorOffset; // How far the door lift has been translated
    float maxLiftDoorOffset = 300.0f; // Maximum distance to open the door lift
    float liftDoorSpeed = 100.0f;
    glm::vec3 initialDoorRPosition;
    glm::vec3 initialDoorLPosition;
    //
    float liftOffset; // How far the lift has been translated
    float maxLifOffset = 1500.0f; // Maximum distance to open the lift
    float liftSpeed = 100.0f;
    bool isMoving;           // Track if the lift is currently moving
    bool doorsAreClosed;     // Ensure doors are fully closed before moving
    float currentLiftOffset; // Track the current translation offset of the lift  
    glm::vec3 initialLiftPosition;



public:
    Lift(float liftWidth, float liftHeight, float depth, glm::vec3 liftPosition, unsigned int outLiftTexture, unsigned int LiftDoorTexture, unsigned int LiftWallTexture, unsigned int LiftFloorTexture, unsigned int LiftCeilTexture)
        :
        outTheLift(liftWidth + 20.0f, liftHeight , depth + 10.0f), // the height is entered depending on the floors i had
        mainCuboid(liftWidth, 700, depth), //700 as the half of the height of the wall
        doorL(liftWidth/2 + 10.0f, 700 , 10.0f),
        doorR(liftWidth/2 + 10.0f, 700 , 10.0f),
        Light(10.0f, 30 * 5, 10 * 5),
        //
        isOpen(false), // Initialize lift as closed
        liftDoorOffset(0.0f), // Start with no offset

        isMoving(false),
        doorsAreClosed(true),
        currentLiftOffset(0.0f)
    {
        for (int i = 0; i < 6; i++) {
            if (i == 0) { //2 is the front , left wall part
                mainCuboid.SetFaceEnabled(i, false); //  front face
                outTheLift.SetFaceEnabled(i, false); //  front face
            }
            else if (i == 5) {
                mainCuboid.SetTextureForFace(i, LiftFloorTexture);

            } 
            else if (i == 4) {
                mainCuboid.SetTextureForFace(i, LiftCeilTexture);

            }
            else {
                mainCuboid.SetTextureForFace(i, LiftWallTexture);
            }

        }     
        for (int i = 0; i < 6; i++) {
                outTheLift.SetTextureForFace(i, outLiftTexture);
                doorL.SetTextureForFace(i, LiftDoorTexture);
                doorR.SetTextureForFace(i, LiftDoorTexture);

        }
        // Position the left cuboid
        mainCuboid.SetPosition(liftPosition.x, liftPosition.y + 700 / 2 + 2.0f , liftPosition.z);
        outTheLift.SetPosition(liftPosition.x, liftPosition.y + liftHeight / 2  , liftPosition.z);
        doorL.SetPosition(liftPosition.x + liftWidth/4, liftPosition.y + 700 /2, liftPosition.z + (depth / 2));
        doorR.SetPosition(liftPosition.x - liftWidth/4, liftPosition.y + 700 /2, liftPosition.z + (depth / 2));


        Light.SetPosition(liftPosition.x, liftPosition.y + liftHeight - 52.0f, liftPosition.z + depth / 2 + 2.0f);
        /*     std::cout << "Lift Position: ("
                 << Light.GetPosition().x << ", "
                 << Light.GetPosition().y << ", "
                 << Light.GetPosition().z << ")\n";
             */

        // Store the initial position of doorL
        initialDoorLPosition = doorL.GetPosition();
        initialDoorRPosition = doorR.GetPosition();
        initialLiftPosition = mainCuboid.GetPosition();

    }

    void Draw(Shader& shader) {
        mainCuboid.SetFaceEnabled(0, false); //  front face
        mainCuboid.SetFaceEnabled(1, true); //  back face
        mainCuboid.SetFaceEnabled(2, true);  //  left face
        mainCuboid.SetFaceEnabled(3, true);  //  right face
        mainCuboid.SetFaceEnabled(4, true);  //  top face
        mainCuboid.SetFaceEnabled(5, true);  //  bottom face
        mainCuboid.Draw(shader);
        outTheLift.Draw(shader);
        doorL.Draw(shader);
        doorR.Draw(shader);
       // Light.Draw(shader);
    }

    void Update(float deltaTime) {
        // Smoothly open or close the doors
        if (isOpen && liftDoorOffset < maxLiftDoorOffset) {
            liftDoorOffset += liftDoorSpeed * deltaTime;
            if (liftDoorOffset > maxLiftDoorOffset) liftDoorOffset = maxLiftDoorOffset;
        }
        else if (!isOpen && liftDoorOffset > 0.0f) {
            liftDoorOffset -= liftDoorSpeed * deltaTime;
            if (liftDoorOffset < 0.0f) liftDoorOffset = 0.0f;
        }

        // Check if doors are closed
        doorsAreClosed = (liftDoorOffset == 0.0f);

        // Handle lift movement if doors are closed
        if (isMoving && doorsAreClosed) {
            currentLiftOffset += liftSpeed * deltaTime; // Move the lift
            if (currentLiftOffset > maxLifOffset) {
                currentLiftOffset = maxLifOffset; // Limit maximum offset
                isMoving = false;               // Stop moving if max reached
            }

            // Update lift position
            mainCuboid.SetPosition(initialLiftPosition.x, initialLiftPosition.y + currentLiftOffset, initialLiftPosition.z);
        }

        // Update door positions relative to the lift's current position
        glm::vec3 currentLiftPosition = mainCuboid.GetPosition();
        doorL.SetPosition(
            currentLiftPosition.x + liftDoorOffset + (mainCuboid.GetPosition().x / 4) - 25.0f,
            currentLiftPosition.y,
            initialDoorLPosition.z
        );
        doorR.SetPosition(
            currentLiftPosition.x - liftDoorOffset - (mainCuboid.GetPosition().x / 4) + 25.0f,
            currentLiftPosition.y,
            initialDoorRPosition.z
        );
    }



    void StartLiftMovement() {
        if (doorsAreClosed) {
            isMoving = true;
        }
    }

    void StopLiftMovement() {
        isMoving = false;
    }

    void HandleInput(int key) {
        // Map keys to specific floor offsets
        int targetFloor = -1;

        if (key == GLFW_KEY_0) {
            targetFloor = 0; // Ground floor
            std::cout << "Ground floor Ground floor Ground floor"<< targetFloor << std::endl;

        }
        else if (key == GLFW_KEY_Z) {
            targetFloor = 1; // First floor
            std::cout << "target Floor target floor target floor"<< targetFloor << std::endl;
            std::cout << "Key Pressed: " << key << std::endl;

        }
        else if (key == GLFW_KEY_2) targetFloor = 2; // Second floor

        if (targetFloor != -1) { // If a valid floor was selected
            float targetOffset = targetFloor * maxLifOffset; // Calculate target offset based on floor
            MoveLiftToOffset(targetOffset); // Move the lift to the calculated offset
        }

        
        else if (key == GLFW_KEY_L) {
            // Start movement (if doors are closed)
            StartLiftMovement();
        }
        else if (key == GLFW_KEY_K) {
            // Stop movement
            StopLiftMovement();
        }
        else if (key == GLFW_KEY_LEFT_BRACKET) {
            // Toggle the doors
            ToggleLift();
        }
    }

    // Function to move the lift to the specified offset
    void MoveLiftToOffset(float targetOffset) {
        if (!doorsAreClosed) {
            std::cout << "Close the doors before moving the lift!" << std::endl;
            return;
        }
        std::cout << "target Offset" << targetOffset << std::endl;


        isMoving = true; // Start moving the lift
        float direction = (targetOffset > currentLiftOffset) ? 1.0f : -1.0f; // Determine direction

        while (currentLiftOffset != targetOffset) {
            // Simulate lift movement
            currentLiftOffset += direction * liftSpeed * 0.01f; // Incremental movement
            if ((direction > 0 && currentLiftOffset >= targetOffset) ||
                (direction < 0 && currentLiftOffset <= targetOffset)) {
                currentLiftOffset = targetOffset; // Snap to target when close enough
            }
            std::cout << "currentLiftOffset" << currentLiftOffset << std::endl;


            // Update lift position
            mainCuboid.SetPosition(
                initialLiftPosition.x,
                initialLiftPosition.y + currentLiftOffset,
                initialLiftPosition.z
            );

            // Exit condition for the simulation loop (only for this example)
            if (currentLiftOffset == targetOffset) break;
        }

        isMoving = false; // Stop moving after reaching the target
    }



    void ToggleLift() {
        isOpen = !isOpen; // Toggle the lift state

    }

    //void OnMouseClick(float mouseX, float mouseY, glm::mat4 viewProjectionMatrix, glm::vec2 screenSize) {
    //    ToggleLift(); // Handle lift click
    //}
};

#endif
