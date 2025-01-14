#ifndef WALL_H
#define WALL_H

#include "Cuboid.h"
#include "Door.h"
#include <glm/glm.hpp>

class WallDoor {
private:
    Cuboid leftCuboid;
    Cuboid rightCuboid;
    Cuboid topCuboid;
    Door door;

public:
    WallDoor(float wallWidth, float wallHeight, float depth, glm::vec3 wallPosition, unsigned int doorTexture, unsigned int WallTexture, unsigned int Wall2Texture )
        :
        leftCuboid(depth, 500.0f, (wallWidth - 300) / 2), // Left part
        rightCuboid(depth, 500.0f, (wallWidth - 300) / 2), // Right part
        topCuboid(depth, wallHeight - 500.0f, wallWidth), // Top part (adjust height as needed)
        door(300.0f, 500.0f, depth, glm::vec3(wallPosition.x, wallPosition.y + 250.0f, wallPosition.z), doorTexture, glm::vec3(0.0f, 90.0f, 0.0f)) // Door
    {
        // Set positions for the cuboids
        float sideWallsWidth = (wallWidth - 300) / 2;
        for (int i = 0; i < 6; i++) {
            if (i == 2) { //2 is the left wall part
                leftCuboid.SetTextureForFace(i, Wall2Texture);
                rightCuboid.SetTextureForFace(i, Wall2Texture);
                topCuboid.SetTextureForFace(i, Wall2Texture);
            }
            else {
                leftCuboid.SetTextureForFace(i, WallTexture);
                rightCuboid.SetTextureForFace(i, WallTexture);
                topCuboid.SetTextureForFace(i, WallTexture);
            }
        }
        // Position the left cuboid
        leftCuboid.SetPosition(wallPosition.x  , wallPosition.y + 250.0f, wallPosition.z - (wallWidth / 2.0f) + sideWallsWidth / 2.0f);

        // Position the right cuboid
        rightCuboid.SetPosition(wallPosition.x , wallPosition.y + 250.0f, wallPosition.z + (wallWidth / 2.0f) - sideWallsWidth / 2.0f);

        // Position the top cuboid (centered above the door)
        topCuboid.SetPosition(wallPosition.x, wallPosition.y + wallHeight / 2.0f + 250.0f, wallPosition.z); // Adjust height as necessary
        topCuboid.SetTextureRepeat(2.0f, 2.0f);

        // Set the door position
        door.SetPosition(wallPosition.x, wallPosition.y - wallHeight / 2.0f + 10.0f, wallPosition.z); // Adjust door position
    }

    void Draw(Shader& shader) {
        leftCuboid.Draw(shader);
        rightCuboid.Draw(shader);
        topCuboid.Draw(shader);
        door.Draw(shader);
    }

    void Update(float deltaTime) {
        door.Update(deltaTime); // Update the door state
    }

    void OnMouseClick(float mouseX, float mouseY, glm::mat4 viewProjectionMatrix, glm::vec2 screenSize) {
        door.OnMouseClick(mouseX, mouseY, viewProjectionMatrix, screenSize); // Handle door click
    }
};

#endif // WALL_H
