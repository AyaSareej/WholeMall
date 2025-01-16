#ifndef CAR_TOY_H
#define CAR_TOY_H

#include "Cuboid.h"
#include "Cylinder.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CarToy {
private:
    Cuboid body;
   // Cylinder wheels[4];
    Cylinder whFL;
    Cylinder whFR;
    Cylinder whBL;
    Cylinder whBR;

    glm::vec3 position;
    glm::vec3 rotation;
    float bodyWidth, bodyHeight, bodyDepth;
    float wheelRadius, wheelWidth;

public:
    // Constructor
    CarToy(float width, float height, float depth, float wheelR, float wheelW)
        : body(width, height, depth), whFL(wheelR, wheelW,33), whFR(wheelR, wheelW, 33), whBL(wheelR, wheelW, 33), whBR(wheelR, wheelW, 33),
        bodyWidth(width), bodyHeight(height), bodyDepth(depth),
        wheelRadius(wheelR), wheelWidth(wheelW) {

        // Initialize wheels
       /* for (int i = 0; i < 4; ++i) {
            wheels[i] = Cylinder(wheelRadius, wheelWidth, 36);
        }*/

        // Set initial position and rotation
        position = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        whFL.SetRotation(90, 10, 0);
        whFR.SetRotation(90, 10, 0);
        whBL.SetRotation(90, 10, 0);
        whBR.SetRotation(90, 10, 0);
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
        body.SetTextureForFace(0, textureID); // Apply to the front face (as an example)
        for (int i = 1; i < 6; ++i) {
            body.SetTextureForFace(i, textureID);
        }
    }

    // Set the wheel texture
    void SetWheelTexture(GLuint textureID) {
        whFL.SetTexture(textureID);
        whFR.SetTexture(textureID);
        whBL.SetTexture(textureID);
        whBR.SetTexture(textureID);
       /* for (int i = 0; i < 4; ++i) {
            wheels[i].SetTexture(textureID);
        }*/
    }

    // Draw the car
    void Draw(Shader& shader) {
        body.Draw(shader);
        //wheels[0].SetPosition(glm::vec3(0, 0, 0));
        whFL.Draw(shader);
        whFR.Draw(shader);
        whBL.Draw(shader);
        whBR.Draw(shader);
       /* for (int i = 0; i < 4; ++i) {
            wheels[i].Draw(shader);
        }*/
    }

private:
    void updateComponentsPosition() {
        float halfWidth = bodyWidth / 2.0f;
        float halfHeight = bodyHeight / 2.0f;
        float halfDepth = bodyDepth / 2.0f;

        // Set body position
        body.SetPosition(position.x, position.y, position.z);
        //wh.SetPosition(0, 40, 0);
        // Set wheel positions
        //wheels[0].SetPosition(position.x - halfWidth + wheelRadius, position.y - halfHeight, position.z - halfDepth + wheelWidth / 4.0f); // Front left
        whFR.SetPosition(position.x + halfWidth, position.y - halfHeight, position.z - halfDepth ); // Front right
        whBL.SetPosition(position.x - halfWidth , position.y - halfHeight, position.z + halfDepth ); // Back left
        whBR.SetPosition(position.x + halfWidth , position.y - halfHeight, position.z + halfDepth ); // Back right   
        whFL.SetPosition(position.x - halfWidth, position.y - halfHeight, position.z - halfDepth ); // Front left
     
    }
};

#endif // CAR_TOY_H
