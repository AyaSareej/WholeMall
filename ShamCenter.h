#ifndef SHAMCENTER_H
#define SHAMCENTER_H_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include "Shader.h"
#include "Cuboid.h"
#include "Trapeozid.h"
#include "Arch.h"
#include "Sphere.h"
#include "Cylinder.h"
#include <glm/glm.hpp>

class ShamCenter {
private:
    Cuboid mainCuboid;
    Cuboid undergroundCuboid;
   
    Trapezoid frontMainPartL;
    Trapezoid frontMainPartR;

    Trapezoid frontPartBottomL;
    Trapezoid frontPartBottomR;
    
    Trapezoid leftCenterPart;
    Trapezoid leftCenterPartBottom;
    
    DoorFrame doorFrameOuter;
    DoorFrame doorFrame;
    DoorFrame doorFrame2;
    
    Cylinder cy;

    Cylinder cyO;

    Cylinder c1;
    Cylinder c2;
    Cylinder c3;
    Cylinder c4;
    Cylinder c5;
    Cylinder c6;

    // roof 2
    Cuboid secondFloorPartR;
    Cuboid secondFloorPartL;
    Cuboid secondFloorPartF;
    Cuboid secondFloorPartB;


    // State variables
    bool isOpen; // To track if the drawer is open or closed
    float drawerOffset; // How far the drawer has been translated
    float maxDrawerOffset = 100.0f; // Maximum distance to open the drawer
    float drawerSpeed = 100.0f;
    glm::vec3 initialDrawerAPosition;
    glm::vec3 initialhandPosition;



public:
    ShamCenter(float Width, float Height, float depth, glm::vec3 centerPosition, unsigned int centerTexture, unsigned int legTexture, unsigned int outerDoorTexture, unsigned int floorTexture, unsigned int glassTexture)
        :
        mainCuboid(Width, Height, depth),
        undergroundCuboid(Width, Height/2, depth),
        
        frontMainPartL(10, 100, Height / 4, depth / 2 * 2 / 7),
        frontMainPartR(10, 100, Height / 4, depth / 2 * 2 / 7),
        frontPartBottomL(10, 220, Height / 2, depth / 2 * 2 / 7),
        frontPartBottomR(10, 220, Height / 2, depth / 2 * 2 / 7),

        leftCenterPart(10, 220, Height / 3, Width / 2 - 300),
        leftCenterPartBottom(20, 220, Height / 5, Width / 2 - 300 - 300),

        doorFrameOuter(glm::vec3(centerPosition.x - Width / 2 + 155, centerPosition.y + Height / 2, centerPosition.z + 300), glm::vec3(depth / 3, Height, 30), glm::vec3(depth / 5, Height * 2 / 3, 30), 0),
        doorFrame(glm::vec3(centerPosition.x - Width / 2 + 155, centerPosition.y + Height * 2 / 3 / 2, centerPosition.z + 300), glm::vec3(depth / 5, Height * 2 / 3, 30), glm::vec3(depth / 10, Height / 3, 30), 0),
        doorFrame2(glm::vec3(centerPosition.x - Width / 4 + 100, centerPosition.y + Height / 3 / 2, centerPosition.z - depth / 2 + 200), glm::vec3(Width / 5, Height / 3 + 15, 30), glm::vec3(Width / 10, Height / 5, 30), 0),

        cy(310, Height, 15),
        cyO(50, Height / 3, 36),

        c1(80, Height * 2, 36),
        c2(80, Height * 2, 36),
        c3(80, Height * 2, 36),
        c4(80, Height * 2, 36),
        c5(80, Height * 2, 36),
        c6(80, Height * 2, 36),

        secondFloorPartR(Width - 100,depth/4, 50),
        secondFloorPartL(Width - 200,depth/4 -150, 50),
        secondFloorPartF(Width/4,50, depth/2),
        secondFloorPartB(Width/3 - 100 ,50, depth/2),


        //
        isOpen(false), // Initialize drawer as closed
        drawerOffset(0.0f) // Start with no offset
    {
        for (int i = 0; i < 6; i++) {            
                mainCuboid.SetTextureForFace(i, centerTexture);
                undergroundCuboid.SetTextureForFace(i, centerTexture);
                doorFrameOuter.SetFaceTexture(i, outerDoorTexture);
                doorFrame.SetFaceTexture(i, legTexture);
                doorFrame2.SetFaceTexture(i, outerDoorTexture);
                secondFloorPartR.SetTextureForFace(i, floorTexture);
                secondFloorPartL.SetTextureForFace(i, floorTexture);
                secondFloorPartF.SetTextureForFace(i, floorTexture);
                secondFloorPartB.SetTextureForFace(i, floorTexture);

        }
        for (int i = 0; i < 6; i++) {
            if (i == 1 ) { // 3 from inside , 4 the top, 5 bottom , 2 front
                frontMainPartL.SetTextureForFace(i, legTexture);
                frontMainPartR.SetTextureForFace(i, legTexture);
                frontPartBottomL.SetTextureForFace(i, legTexture);
                frontPartBottomR.SetTextureForFace(i, legTexture);
                leftCenterPart.SetTextureForFace(i, legTexture);
                leftCenterPartBottom.SetTextureForFace(i, legTexture);
            }
            else if (i == 0) {
                    frontMainPartL.SetTextureForFace(i, legTexture);
                    frontMainPartR.SetTextureForFace(i, legTexture);
                    frontPartBottomL.SetTextureForFace(i, legTexture);
                    frontPartBottomR.SetTextureForFace(i, legTexture);
                    leftCenterPart.SetTextureForFace(i, legTexture);
                    leftCenterPartBottom.SetTextureForFace(i, legTexture);
            }
            else {
                frontMainPartL.SetTextureForFace(i, centerTexture);
                frontMainPartR.SetTextureForFace(i, centerTexture);
                frontPartBottomL.SetTextureForFace(i, centerTexture);
                frontPartBottomR.SetTextureForFace(i, centerTexture);
                leftCenterPart.SetTextureForFace(i, centerTexture);
                leftCenterPartBottom.SetTextureForFace(i, centerTexture);
            }
        }
       
        /// Door place
        doorFrameOuter.Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        doorFrame.Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        ///
        cy.SetTexture(glassTexture);
        cy.SetTextureRepeat(5, 3);
        cy.SetTransparency(0.7f);

        cyO.SetTexture(outerDoorTexture);
        c1.SetTexture(outerDoorTexture);
        c2.SetTexture(outerDoorTexture);
        c3.SetTexture(outerDoorTexture);
        c4.SetTexture(outerDoorTexture);
        c5.SetTexture(outerDoorTexture);
        c6.SetTexture(outerDoorTexture);

        frontMainPartL.SetTextureRepeat(5, 7);
        frontMainPartR.SetTextureRepeat(5, 7);
        mainCuboid.SetTextureRepeat(7, 7);
        undergroundCuboid.SetTextureRepeat(7, 7);
       
        leftCenterPart.SetTextureRepeat(9, 14);
        leftCenterPartBottom.SetTextureRepeat(9, 2);
       
        frontPartBottomL.SetTextureRepeat(4, 6);
        frontPartBottomR.SetTextureRepeat(4, 6);
        

        cy.SetPosition(centerPosition.x - Width/3 - 200  , centerPosition.y + Height / 2 , centerPosition.z - depth* 3/7 + 50);
        cyO.SetPosition(centerPosition.x - Width/4 - 100   , centerPosition.y + Height/6  , centerPosition.z - depth / 2 - 100);
       
        c1.SetPosition(centerPosition.x  - Width / 5, centerPosition.y   , centerPosition.z - depth/4 );
        c2.SetPosition(centerPosition.x  - Width / 5, centerPosition.y   , centerPosition.z - 0 );
        c3.SetPosition(centerPosition.x  - Width / 5, centerPosition.y   , centerPosition.z + depth/4 );

        c4.SetPosition(centerPosition.x  + Width / 5, centerPosition.y   , centerPosition.z - depth / 4);
        c5.SetPosition(centerPosition.x  + Width / 5, centerPosition.y   , centerPosition.z  + 0);
        c6.SetPosition(centerPosition.x  + Width / 5, centerPosition.y   , centerPosition.z + depth / 4);

        mainCuboid.SetPosition(centerPosition.x, centerPosition.y + Height / 2 , centerPosition.z);
        undergroundCuboid.SetPosition(centerPosition.x, centerPosition.y - Height/4  , centerPosition.z);
        
        frontMainPartL.SetPosition(centerPosition.x - Width / 2 , centerPosition.y + Height* 2/3, centerPosition.z - depth / 5 - 20.0f);
        frontMainPartR.SetPosition(centerPosition.x - Width / 2 , centerPosition.y + Height * 2/3  , centerPosition.z + depth / 2 - (depth / 7) - 10.0f);
        frontPartBottomL.SetPosition(centerPosition.x - Width / 2 + (250/2), centerPosition.y + Height / 2 , centerPosition.z - depth /5 - 20.0f);
        frontPartBottomR.SetPosition(centerPosition.x - Width / 2 + (250 / 2), centerPosition.y + Height / 2 , centerPosition.z + depth / 2 - (depth/7) - 10.0f);
        
        secondFloorPartR.SetPosition(centerPosition.x + 100, centerPosition.y + Height / 2, depth /3 + 150);
        secondFloorPartR.SetRotation(90, 0, 0);
        secondFloorPartR.SetTextureRepeat(5, 5);
        
        secondFloorPartL.SetPosition(centerPosition.x + 100, centerPosition.y + Height / 2, - depth /3 -50 );
        secondFloorPartL.SetRotation(90, 0, 0);
        secondFloorPartL.SetTextureRepeat(5, 5); 
        
        secondFloorPartF.SetPosition(centerPosition.x - Width/4 - 100 , centerPosition.y + Height / 2, centerPosition.z  );
        secondFloorPartF.SetTextureRepeat(5, 5); 
        
        secondFloorPartB.SetPosition(centerPosition.x + Width/3  + 100  , centerPosition.y + Height / 2, centerPosition.z  );
        secondFloorPartB.SetTextureRepeat(5, 5);

        undergroundCuboid.SetTextureForFace(5, floorTexture);
        leftCenterPart.SetPosition(centerPosition.x + 300  , centerPosition.y + Height * 2 / 3, centerPosition.z - depth/2 );
        leftCenterPartBottom.SetPosition(centerPosition.x + 300 + 300  , centerPosition.y + (Height /3 )/ 2 , centerPosition.z - depth/2 );
        leftCenterPart.SetRotation(0, -90, 0);
        leftCenterPartBottom.SetRotation(0, -90, 0);
        float n = frontPartBottomL.GetDepth();
        float m = mainCuboid.GetDepth();
        //
        std::cout << "frontPartBottomL frontPartBottomL frontPartBottomL " << n << std::endl;


        /*     std::cout << "Drawer Position: ("
                 << spA.GetPosition().x << ", "
                 << spA.GetPosition().y << ", "
                 << spA.GetPosition().z << ")\n";
             */

        // Store the initial position of drawerA
        //initialDrawerAPosition = drawerA.GetPosition();

    }

    void Draw(Shader& shader) {
        mainCuboid.SetFaceEnabled(0, true); //  front face
        mainCuboid.SetFaceEnabled(1, false); //  back face
        mainCuboid.SetFaceEnabled(2, false);  //  left face
        mainCuboid.SetFaceEnabled(3, true);  //  right face
        mainCuboid.SetFaceEnabled(4, true);  //  top face
        mainCuboid.SetFaceEnabled(5, false);  //  bottom face
        mainCuboid.Draw(shader);
        undergroundCuboid.Draw(shader);
        undergroundCuboid.SetFaceEnabled(4, false);  //  top face

        frontMainPartL.Draw(shader);
        frontMainPartR.Draw(shader);
        doorFrameOuter.Draw(shader);
        doorFrame.Draw(shader);
        doorFrame2.Draw(shader);
        frontPartBottomL.Draw(shader);
        frontPartBottomR.Draw(shader);
        leftCenterPart.Draw(shader);
        leftCenterPartBottom.Draw(shader);
        cy.Draw(shader);
        cyO.Draw(shader);

        c1.Draw(shader);
        c2.Draw(shader);
        c3.Draw(shader);
        c4.Draw(shader);
        c5.Draw(shader);
        c6.Draw(shader);

        secondFloorPartR.Draw(shader);
        secondFloorPartL.Draw(shader);
        secondFloorPartF.Draw(shader);
        secondFloorPartB.Draw(shader);
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
        //drawerA.SetPosition(initialDrawerAPosition.x, initialDrawerAPosition.y, initialDrawerAPosition.z + drawerOffset);
        //spA.SetPosition(initialhandPosition.x, initialhandPosition.y, initialhandPosition.z + drawerOffset);
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

    void SetPosition(glm::vec3 newPosition) {
        mainCuboid.SetPosition(newPosition.x, newPosition.y, newPosition.z);

        frontMainPartL.SetPosition(newPosition.x - mainCuboid.GetWidth() / 2 - 50.0f, newPosition.y + mainCuboid.GetHeight() / 2, newPosition.z + mainCuboid.GetWidth() / 5);
        frontMainPartR.SetPosition(newPosition.x + mainCuboid.GetWidth() / 2 + 50.0f, newPosition.y + mainCuboid.GetHeight() / 2, newPosition.z + mainCuboid.GetWidth() / 5);
        //frontPartUpL.SetPosition(newPosition.x - mainCuboid.GetWidth() / 2 - 25.0f, newPosition.y + mainCuboid.GetHeight() / 5.0f / 2 + 50.0f, newPosition.z + mainCuboid.GetWidth() / 5);
        frontPartBottomL.SetPosition(newPosition.x - mainCuboid.GetWidth() / 2 + 50.0f, newPosition.y + mainCuboid.GetHeight() / 2, newPosition.z - mainCuboid.GetDepth() * 1 / 7);
        frontPartBottomR.SetPosition(newPosition.x - mainCuboid.GetWidth() / 2 + 50.0f, newPosition.y + mainCuboid.GetHeight() / 2, newPosition.z + mainCuboid.GetDepth() * 2 / 7);
        //frontPartUpR.SetPosition(newPosition.x, newPosition.y + mainCuboid.GetHeight() / 2.0f + 70.0f, newPosition.z - mainCuboid.GetDepth() / 2 - 100);

        //doorFrameOuter.SetPosition(newPosition);
        //doorFrame.SetPosition(newPosition);

        cy.SetPosition(newPosition.x - mainCuboid.GetWidth() / 3, newPosition.y + mainCuboid.GetHeight() / 2 + 50.0f, newPosition.z - mainCuboid.GetDepth() * 3 / 7);
    }

    //void OnMouseClick(float mouseX, float mouseY, glm::mat4 viewProjectionMatrix, glm::vec2 screenSize) {
    //    ToggleDrawer(); // Handle drawer click
    //}
};

#endif
