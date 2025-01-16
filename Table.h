#ifndef TABLE_H
#define TABLE_H

#include "Cuboid.h"
#include "Cylinder.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Table {
private:
    Cuboid body;
    // Cylinder wheels[4];
    Cuboid whFL;
    Cuboid whFR;
    Cuboid whBL;
    Cuboid whBR;

    glm::vec3 position;
    glm::vec3 rotation;
    float bodyWidth, bodyHeight, bodyDepth;
    float legsSize, legsHeight;

public:
    // Constructor
    Table(float width, float height, float depth, float legS, float legH)
        : body(width, height, depth), whFL(legS, legH, legS), whFR(legS, legH, legS), whBL(legS, legH, legS), whBR(legS, legH, legS),
        bodyWidth(width), bodyHeight(height), bodyDepth(depth),
        legsSize(legS), legsHeight(legH) {

        // Initialize wheels
       /* for (int i = 0; i < 4; ++i) {
            wheels[i] = Cylinder(wheelRadius, wheelWidth, 36);
        }*/

        // Set initial position and rotation
        position = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        updateComponentsPosition();
    }

    // Set the position of the car
    void SetPosition(float x, float y, float z) {
        position = glm::vec3(x, y, z);
        updateComponentsPosition();
    }

    // Set the rotation of the car
    void SetRotation(float x, float y, float z) {
        rotation = glm::vec3(x, y, z);
        body.SetRotation(x, y, z);

        // Set wheel rotations independently if needed
      /*  for (int i = 0; i < 4; ++i) {
            wheels[i].SetRotation(x, y, z);
        }*/
    }

    // Set the body texture
    void SetBodyTexture(GLuint textureID) {
        for (int  i = 0; i < 5; i++) {
            body.SetTextureForFace(i, textureID);

        }
    }

    // Set the wheel texture
    void SetLegsTexture(GLuint textureID) {
        for (int i = 0; i < 5; i++) {
            whFL.SetTextureForFace(i,textureID);
            whFR.SetTextureForFace(i,textureID);
            whBL.SetTextureForFace(i,textureID);
            whBR.SetTextureForFace(i,textureID);
        }     
    }

    // Draw the car
    void Draw(Shader& shader) {
        body.Draw(shader);
        whFL.Draw(shader);
        whFR.Draw(shader);
        whBL.Draw(shader);
        whBR.Draw(shader);
    }

private:
    void updateComponentsPosition() {
        float halfWidth = bodyWidth / 2.0f;
        float Height = bodyHeight + 2.0f;
        float halfDepth = bodyDepth / 2.0f;

        // Set body position
        body.SetPosition(position.x, position.y, position.z);
        //wh.SetPosition(0, 40, 0);
        // Set wheel positions
        //wheels[0].SetPosition(position.x - halfWidth + wheelRadius, position.y - halfHeight, position.z - halfDepth + wheelWidth / 4.0f); // Front left
        whFR.SetPosition(position.x + halfWidth - legsSize, position.y - Height, position.z - halfDepth + legsSize); // Front right
        whBL.SetPosition(position.x - halfWidth + legsSize, position.y - Height, position.z + halfDepth - legsSize); // Back left
        whBR.SetPosition(position.x + halfWidth - legsSize, position.y - Height, position.z + halfDepth - legsSize); // Back right   
        whFL.SetPosition(position.x - halfWidth + legsSize, position.y - Height, position.z - halfDepth + legsSize); // Front left

    }
};

#endif // CAR_TOY_H
