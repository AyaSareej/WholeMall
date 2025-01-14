#ifndef DRAWER_H
#define DRAWER_H

#include "Cuboid.h"
#include "Door.h"
#include "Sphere.h"
#include <glm/glm.hpp>

class Drawer {
private:
    Cuboid mainCuboid;
    Cuboid drawerA;
    Cuboid drawerB;
  
    //legs
    Cuboid legLB;
    Cuboid legRB;
    Cuboid legLF;
    Cuboid legRF;

    //
    Sphere spA;
    Sphere spB;

    // State variables
    bool isOpen; // To track if the drawer is open or closed
    float drawerOffset; // How far the drawer has been translated
    float maxDrawerOffset = 100.0f; // Maximum distance to open the drawer
    float drawerSpeed = 100.0f;
    glm::vec3 initialDrawerAPosition;
    glm::vec3 initialhandPosition;



public:
    Drawer(float drawerWidth, float drawerHeight, float depth, glm::vec3 drawerPosition, unsigned int drawerTexture, unsigned int legTexture)
        :
        mainCuboid(drawerWidth, drawerHeight, depth), 
        drawerA(drawerWidth-5.0f, drawerHeight/2 -2.0f, depth -5.0f),
        drawerB(drawerWidth -5.0f, drawerHeight/2 -2.0f, depth -5.0f),
        spA(10.0f, 30 * 5, 10 * 5),
        spB(10.0f, 30 * 5, 10 * 5),

        // legs
        legLB(10.0f, 12.0f, 10.0f),
        legRB(10.0f, 12.0f, 10.0f),
        legLF(10.0f, 12.0f, 10.0f),
        legRF(10.0f, 12.0f, 10.0f),

        //
        isOpen(false), // Initialize drawer as closed
        drawerOffset(0.0f) // Start with no offset
    {
        for (int i = 0; i < 6; i++) {
            if (i == 2) { //2 is the left wall part
                mainCuboid.SetTextureForFace(i, drawerTexture);
            }
            else if (i == 5) {
                drawerA.SetTextureForFace(i, legTexture);
            }
            else {
                mainCuboid.SetTextureForFace(i, drawerTexture);
                legLB.SetTextureForFace(i, legTexture);
                legRB.SetTextureForFace(i, legTexture);
                legLF.SetTextureForFace(i, legTexture);
                legRF.SetTextureForFace(i, legTexture);
                drawerA.SetTextureForFace(i, drawerTexture);
                drawerB.SetTextureForFace(i, drawerTexture);
            }
        }
        // Position the left cuboid
        mainCuboid.SetPosition(drawerPosition.x, drawerPosition.y + drawerHeight / 2 + 15.0f + 7.0f, drawerPosition.z);
        drawerA.SetPosition(drawerPosition.x, drawerPosition.y + drawerHeight - 52.0f , drawerPosition.z);
        drawerB.SetPosition(drawerPosition.x , drawerPosition.y + drawerHeight /4 + 16.0f + 7.0f, drawerPosition.z);
        //
        legLB.SetPosition(drawerPosition.x - drawerWidth / 2 + 5.0f, drawerPosition.y + 16.0f, drawerPosition.z - depth / 2 + 5.0f);
        legRB.SetPosition(drawerPosition.x + drawerWidth / 2 - 5.0f, drawerPosition.y + 16.0f, drawerPosition.z - depth / 2 + 5.0f);
        legLF.SetPosition(drawerPosition.x - drawerWidth / 2 + 5.0f, drawerPosition.y + 16.0f, drawerPosition.z + depth / 2 - 5.0f);
        legRF.SetPosition(drawerPosition.x + drawerWidth / 2 - 5.0f, drawerPosition.y + 16.0f, drawerPosition.z + depth / 2 - 5.0f);

        spA.SetPosition(drawerPosition.x , drawerPosition.y + drawerHeight - 52.0f, drawerPosition.z + depth/2 + 2.0f);
        spB.SetPosition(drawerPosition.x, drawerPosition.y + drawerHeight / 4 + 16.0f + 7.0f, drawerPosition.z + depth/2 + 2.0f);
   /*     std::cout << "Drawer Position: ("
            << spA.GetPosition().x << ", "
            << spA.GetPosition().y << ", "
            << spA.GetPosition().z << ")\n";
        */
        spA.SetTexture(legTexture);
        spB.SetTexture(legTexture);

        // Store the initial position of drawerA
        initialDrawerAPosition = drawerA.GetPosition();
        initialhandPosition = spA.GetPosition();

    }

    void Draw(Shader& shader) {
        mainCuboid.SetFaceEnabled(0, false); //  front face
        mainCuboid.SetFaceEnabled(1, true); //  back face
        mainCuboid.SetFaceEnabled(2, true);  //  left face
        mainCuboid.SetFaceEnabled(3, true);  //  right face
        mainCuboid.SetFaceEnabled(4, true);  //  top face
        mainCuboid.SetFaceEnabled(5, true);  //  bottom face
        mainCuboid.Draw(shader);
        legLB.Draw(shader);
        legRB.Draw(shader);
        legLF.Draw(shader);
        legRF.Draw(shader);
        drawerA.SetFaceEnabled(4, false);
        drawerB.SetFaceEnabled(4, false);
        drawerA.Draw(shader);
        drawerB.Draw(shader);
        spA.Draw(shader);
        spB.Draw(shader);
    }

    void Update(float deltaTime) {
        // Smoothly open or close the drawer
        if (isOpen && drawerOffset < maxDrawerOffset) {
            drawerOffset += drawerSpeed * deltaTime;
            if (drawerOffset > maxDrawerOffset) drawerOffset = maxDrawerOffset;
        }
        else if (!isOpen && drawerOffset > 0.0f) {
            drawerOffset -= drawerSpeed * deltaTime;
            if (drawerOffset < 0.0f) drawerOffset = 0.0f;
        }
     /*   std::cout << "Drawer Position: ("
            << drawerA.GetPosition().x << ", "
            << drawerA.GetPosition().y << ", "
            << drawerA.GetPosition().z << ")\n";*/
        // Update drawerA's position relative to its initial position
        drawerA.SetPosition(initialDrawerAPosition.x, initialDrawerAPosition.y, initialDrawerAPosition.z + drawerOffset);
        spA.SetPosition(initialhandPosition.x , initialhandPosition.y, initialhandPosition.z + drawerOffset);
    }

    void HandleInput(int key) {
        if (key == GLFW_KEY_R) {
            // Start movement (if doors are closed)
            ToggleDrawer();
        }
    }

    void ToggleDrawer() {
        isOpen = !isOpen; // Toggle the drawer state

    }

    //void OnMouseClick(float mouseX, float mouseY, glm::mat4 viewProjectionMatrix, glm::vec2 screenSize) {
    //    ToggleDrawer(); // Handle drawer click
    //}
};

#endif
