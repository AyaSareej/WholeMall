#ifndef CLOSET_H
#define CLOSET_H

#include "Cuboid.h"
#include "Door.h"
#include <glm/glm.hpp>

class Closet {
private:
    Cuboid mainCuboid;
    Door doorL;
    Door doorR;
    Cuboid legLB;
    Cuboid legRB;
    Cuboid legLF;
    Cuboid legRF;
     
public:
    Closet(float closetWidth, float closetHeight, float depth, glm::vec3 closetPosition, unsigned int closetTexture, unsigned int legTexture)
        :
        mainCuboid(closetWidth, closetHeight, depth), // Left part
        doorL(closetWidth / 2, closetHeight, 10.0f, glm::vec3(closetPosition.x + (closetWidth / 4), closetPosition.y + (closetHeight / 2) + 15.0f + 7.0f, closetPosition.z + (depth / 2)), closetTexture, glm::vec3(0.0f, 0.0f, 0.0f)), // Door
        doorR(closetWidth / 2, closetHeight, 10.0f, glm::vec3(closetPosition.x - (closetWidth / 4), closetPosition.y + closetHeight / 2 + 15.0f +7.0f, closetPosition.z + (depth / 2)), closetTexture, glm::vec3(0.0f, 0.0f, 0.0f)), // Door
        legLB(10.0f, 12.0f, 10.0f),
        legRB(10.0f, 12.0f, 10.0f),
        legLF(10.0f, 12.0f, 10.0f),
        legRF(10.0f, 12.0f, 10.0f)
    {
        for (int i = 0; i < 6; i++) {
            if (i == 2) { //2 is the left wall part
                mainCuboid.SetTextureForFace(i, closetTexture);
            }
            else {
                mainCuboid.SetTextureForFace(i, closetTexture);
                legLB.SetTextureForFace(i, legTexture);
                legRB.SetTextureForFace(i, legTexture);
                legLF.SetTextureForFace(i, legTexture);
                legRF.SetTextureForFace(i, legTexture);

            }
        }
        // Position the left cuboid
        mainCuboid.SetPosition(closetPosition.x, closetPosition.y + closetHeight /2 + 15.0f + 7.0f, closetPosition.z );
        legLB.SetPosition(closetPosition.x - closetWidth/2 + 5.0f, closetPosition.y + 16.0f , closetPosition.z - depth/2 + 5.0f);
        legRB.SetPosition(closetPosition.x + closetWidth/2 - 5.0f, closetPosition.y + 16.0f , closetPosition.z - depth/2 + 5.0f);
        legLF.SetPosition(closetPosition.x - closetWidth/2 + 5.0f, closetPosition.y + 16.0f , closetPosition.z + depth/2 - 5.0f);
        legRF.SetPosition(closetPosition.x + closetWidth/2 - 5.0f, closetPosition.y + 16.0f , closetPosition.z + depth/2 - 5.0f);
    }

    void Draw(Shader& shader) {
        mainCuboid.SetFaceEnabled(0, false); //  front face
        mainCuboid.SetFaceEnabled(1, true); //  back face
        mainCuboid.SetFaceEnabled(2, true);  //  left face
        mainCuboid.SetFaceEnabled(3, true);  //  right face
        mainCuboid.SetFaceEnabled(4, true);  //  top face
        mainCuboid.SetFaceEnabled(5, true);  //  bottom face
        mainCuboid.Draw(shader);
        doorL.Draw(shader);
        doorR.Draw(shader);
        legLB.Draw(shader);
        legRB.Draw(shader);
        legLF.Draw(shader);
        legRF.Draw(shader);
    }

    void Update(float deltaTime) {
        doorL.Update(deltaTime); // Update the door state
        doorR.Update(deltaTime); // Update the door state
    }

    void OnMouseClick(float mouseX, float mouseY, glm::mat4 viewProjectionMatrix, glm::vec2 screenSize) {
        doorL.OnMouseClick(mouseX, mouseY, viewProjectionMatrix, screenSize); // Handle door click
        doorR.OnMouseClick(mouseX, mouseY, viewProjectionMatrix, screenSize); // Handle door click
    }
};

#endif
