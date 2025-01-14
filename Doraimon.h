//#ifndef DORAIMON_H
//#define DORAIMON_H
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <stb_image.h>
//
//#include "Shader.h"
//#include "Sphere.h"
//
//#include <iostream>
//#include <vector>
//#include <filesystem>
//#include <string>
//#define _USE_MATH_DEFINES
//#include <math.h>
//#include "TextureLoader.h"
//
//unsigned int loadTexture(char const* path);
//
//
//
//Shader SimpleShader("simpleVS.vs", "simpleFS.fs");
//Shader texShader("simpleVS.vs", "texFS.fs");
//
//
//Sphere Head(150.0f, 40 * 5, 20 * 5);
//Sphere Body(140.0f, 40 * 5, 20 * 5);
//Sphere Neck(67.0f, 40 * 5, 20 * 5);
//Sphere Bell(30.0f, 40 * 5, 20 * 5);
//Sphere RightArm(80.0f, 40 * 5, 20 * 5);
//Sphere RightArmBoll(40.0f, 40 * 5, 20 * 5);
//Sphere LeftArm(80.0f, 40 * 5, 20 * 5);
//Sphere LeftArmBoll(40.0f, 40 * 5, 20 * 5);
//Sphere Leg(80.0f, 40 * 5, 20 * 5);
//Sphere LegBoll(40.0f, 40 * 5, 20 * 5);
//Sphere Tile(30.0f, 40 * 5, 20 * 5);
//Sphere Nose(20.0f, 40 * 5, 20 * 5);
//
//
//class Doraimon {
//
//
//private:
//    ///////////load images///////////////
//
//    unsigned int d_head = loadTexture("resources/1x/head.png");
//    unsigned int t_red = loadTexture("resources/n.png");
//    unsigned int t_body = loadTexture("resources/1x/body.png");
//    unsigned int t_bell = loadTexture("resources/1x/bell.png");
//    unsigned int t_blue = loadTexture("resources/1x/blue.png");
//    unsigned int t_white = loadTexture("resources/1x/white.png");
//    unsigned int t_nose = loadTexture("resources/1x/nose.png");
//
//    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
//    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
//
//    // Initialize head position 
//    glm::vec3 headPosition = glm::vec3(0.0f, 0.0f, 0.0f);
//
//    // Initialize body position
//    glm::vec3 bodyPosition = glm::vec3(0.0f, -240.0f, 0.0f);
//
//    // Initialize neck position 
//    glm::vec3 neckPosition = glm::vec3(00.0f, -600.0f, 0.0f);
//
//    // Initialize bell position 
//    glm::vec3 bellPosition = glm::vec3(0.0f, -145.0f, -110.0f);
//
//    // Initialize rightArm position 
//    glm::vec3 rightArmPosition = glm::vec3(150.0f, -150.0f, -25.0f);
//
//    // Initialize rightArmBoll position 
//    glm::vec3 rightArmBollPosition = glm::vec3(240.0f, -150.0f, -25.0f);
//
//    // Initialize LeftArm position 
//    glm::vec3 leftArmPosition = glm::vec3(-150.0f, -150.0f, -25.0f);
//
//    // Initialize LeftArmBoll position 
//    glm::vec3 leftArmBollPosition = glm::vec3(-240.0f, -150.0f, -25.0f);
//
//    // Initialize rightLeg position 
//    glm::vec3 rightLegPosition = glm::vec3(60.0f, -370.0f, -25.0f);
//
//    // Initialize rightLegBoll position 
//    glm::vec3 rightLegBollPosition = glm::vec3(60.0f, -420.0f, -25.0f);
//
//    // Initialize rightLeg position 
//    glm::vec3 leftLegPosition = glm::vec3(-60.0f, -370.0f, -25.0f);
//
//    // Initialize rightLegBoll position 
//    glm::vec3 leftLegBollPosition = glm::vec3(-60.0f, -420.0f, -25.0f);
//
//    //Initialize tile position
//    glm::vec3 tilePosition = glm::vec3(00.0f, -310.0f, 150.0f);
//
//    //Initialize nose position
//    glm::vec3 nosePosition = glm::vec3(00.0f, 60.0f, -150.0f);
//
//
//
//public:
//
//
//    Doraimon() {
//        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
//        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
//
//        unsigned int d_head = loadTexture("resources/1x/head.png");
//        unsigned int t_red = loadTexture("resources/n.png");
//        unsigned int t_body = loadTexture("resources/1x/body.png");
//        unsigned int t_bell = loadTexture("resources/1x/bell.png");
//        unsigned int t_blue = loadTexture("resources/1x/blue.png");
//        unsigned int t_white = loadTexture("resources/1x/white.png");
//        unsigned int t_nose = loadTexture("resources/1x/nose.png");
//
//
//        // Initialize head position 
//        glm::vec3 headPosition = glm::vec3(0.0f, 0.0f, 0.0f);
//
//        // Initialize body position
//        glm::vec3 bodyPosition = glm::vec3(0.0f, -240.0f, 0.0f);
//
//        // Initialize neck position 
//        glm::vec3 neckPosition = glm::vec3(00.0f, -600.0f, 0.0f);
//
//        // Initialize bell position 
//        glm::vec3 bellPosition = glm::vec3(0.0f, -145.0f, -110.0f);
//
//        // Initialize rightArm position 
//        glm::vec3 rightArmPosition = glm::vec3(150.0f, -150.0f, -25.0f);
//
//        // Initialize rightArmBoll position 
//        glm::vec3 rightArmBollPosition = glm::vec3(240.0f, -150.0f, -25.0f);
//
//        // Initialize LeftArm position 
//        glm::vec3 leftArmPosition = glm::vec3(-150.0f, -150.0f, -25.0f);
//
//        // Initialize LeftArmBoll position 
//        glm::vec3 leftArmBollPosition = glm::vec3(-240.0f, -150.0f, -25.0f);
//
//        // Initialize rightLeg position 
//        glm::vec3 rightLegPosition = glm::vec3(60.0f, -370.0f, -25.0f);
//
//        // Initialize rightLegBoll position 
//        glm::vec3 rightLegBollPosition = glm::vec3(60.0f, -420.0f, -25.0f);
//
//        // Initialize rightLeg position 
//        glm::vec3 leftLegPosition = glm::vec3(-60.0f, -370.0f, -25.0f);
//
//        // Initialize rightLegBoll position 
//        glm::vec3 leftLegBollPosition = glm::vec3(-60.0f, -420.0f, -25.0f);
//
//        //Initialize tile position
//        glm::vec3 tilePosition = glm::vec3(00.0f, -310.0f, 150.0f);
//
//        //Initialize nose position
//        glm::vec3 nosePosition = glm::vec3(00.0f, 60.0f, -150.0f);
//
//        DrawHead();
//        DrawBody();
//        DrawNeck();
//        DrawBell();
//        DrawRightArm();
//        DrawRightArmBoll();
//        DrawLeftArm();
//        DrawLeftArmBoll();
//        DrawRightLeg();
//        DrawRightLegBoll();
//        DrawLeftLeg();
//        DrawLeftLegBoll();
//        DrawTile();
//        DrawNose();
//
//
//    }
//    // Member variables for the position and rotation of Doraimon
//   
//
//    //void Draw();
//
//    ///////////////////////// Doraimon drawing //////////////////////////
//    void DrawHead() {
//        glm::mat4 head = glm::mat4(1.0f);
//        head = glm::rotate(head, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        head = glm::rotate(head, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        head = glm::rotate(head, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        head = glm::translate(head, headPosition);
//
//        // Set the shader's model matrix uniform
//        SimpleShader.setMat4("model", head);
//
//        // Bind the head texture
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, d_head);
//
//        // Draw the head using the appropriate Sphere or mesh class
//        SimpleShader.Use();
//        SimpleShader.setMat4("model", head);
//        Head.Draw();
//    }
//
//    void DrawBody() {
//
//        // Doraimon body
//        glm::mat4 body = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_body);
//        body = glm::translate(body, bodyPosition);
//        body = glm::rotate(body, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        body = glm::rotate(body, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        body = glm::rotate(body, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        body = glm::scale(body, glm::vec3(1.0f, 1.0f, 1.0f));
//        SimpleShader.Use();
//        SimpleShader.setMat4("model", body);
//        Body.Draw();
//    }
//
//    void DrawNeck() {
//
//        // Doraimon neck 
//        glm::mat4 neck = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_red);
//        neck = glm::rotate(neck, glm::radians(00.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        neck = glm::rotate(neck, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        neck = glm::rotate(neck, glm::radians(00.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        neck = glm::scale(neck, glm::vec3(1.7f, 0.2f, 1.7f));
//        neck = glm::translate(neck, neckPosition);
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", neck);
//        Neck.Draw();
//    }
//
//    void DrawBell() {
//
//        // Doraimon bell 
//        glm::mat4 bell = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_bell);
//        bell = glm::translate(bell, bellPosition);
//        bell = glm::rotate(bell, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        bell = glm::rotate(bell, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        bell = glm::rotate(bell, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        //bell = glm::scale(bell, glm::vec3(1.7f, 0.2f, 1.7f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", bell);
//        Bell.Draw();
//    }
//
//    void DrawRightArm() {
//
//        // Doraimon rightArm
//        glm::mat4 rightArm = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_blue);
//        rightArm = glm::translate(rightArm, rightArmPosition);
//        //	rightArm = glm::rotate(rightArm, armRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//        rightArm = glm::rotate(rightArm, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        rightArm = glm::rotate(rightArm, glm::radians(00.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        rightArm = glm::scale(rightArm, glm::vec3(1.3f, 0.4f, 0.5f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", rightArm);
//        RightArm.Draw();
//    }
//
//    void DrawRightArmBoll() {
//
//        // Doraimon rightArmBoll
//        glm::mat4 rightArmBoll = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_white);
//        rightArmBoll = glm::translate(rightArmBoll, rightArmBollPosition);
//        //rightArmBoll = glm::rotate(rightArmBoll, armRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//        rightArmBoll = glm::rotate(rightArmBoll, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        rightArmBoll = glm::rotate(rightArmBoll, glm::radians(00.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        //rightArmBoll = glm::scale(rightArmBoll, glm::vec3(1.3f, 0.4f, 0.5f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", rightArmBoll);
//        RightArmBoll.Draw();
//    }
//
//    void DrawLeftArm() {
//
//        // Doraimon leftArm
//        glm::mat4 leftArm = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_blue);
//        leftArm = glm::translate(leftArm, leftArmPosition);
//        //leftArm = glm::rotate(leftArm, -armRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//        leftArm = glm::rotate(leftArm, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        leftArm = glm::rotate(leftArm, glm::radians(00.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        leftArm = glm::scale(leftArm, glm::vec3(1.3f, 0.4f, 0.5f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", leftArm);
//        LeftArm.Draw();
//    }
//
//    void DrawLeftArmBoll() {
//
//        // Doraimon leftArmBoll
//        glm::mat4 leftArmBoll = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_white);
//        leftArmBoll = glm::translate(leftArmBoll, leftArmBollPosition);
//        //	leftArmBoll = glm::rotate(leftArmBoll, -armRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//        leftArmBoll = glm::rotate(leftArmBoll, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        leftArmBoll = glm::rotate(leftArmBoll, glm::radians(00.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        //rightArmBoll = glm::scale(rightArmBoll, glm::vec3(1.3f, 0.4f, 0.5f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", leftArmBoll);
//        LeftArmBoll.Draw();
//    }
//
//    void DrawRightLeg() {
//
//        // Doraimon rightLeg
//        glm::mat4 rightLeg = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_blue);
//        rightLeg = glm::translate(rightLeg, rightLegPosition);
//        //rightLeg = glm::rotate(rightLeg, legRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//        rightLeg = glm::rotate(rightLeg, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        rightLeg = glm::rotate(rightLeg, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        rightLeg = glm::scale(rightLeg, glm::vec3(0.9f, 0.4f, 0.5f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", rightLeg);
//        Leg.Draw();
//    }
//
//    void DrawRightLegBoll() {
//
//        // Doraimon rightLegBoll
//        glm::mat4 rightLegBoll = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_white);
//        rightLegBoll = glm::translate(rightLegBoll, rightLegBollPosition);
//        //rightLegBoll = glm::rotate(rightLegBoll, legRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//        rightLegBoll = glm::rotate(rightLegBoll, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        rightLegBoll = glm::rotate(rightLegBoll, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        rightLegBoll = glm::scale(rightLegBoll, glm::vec3(0.8f, 1.0f, 1.2f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", rightLegBoll);
//        LegBoll.Draw();
//    }
//
//    void DrawLeftLeg() {
//
//        // Doraimon leftLeg
//        glm::mat4 leftLeg = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_blue);
//        leftLeg = glm::translate(leftLeg, leftLegPosition);
//        //leftLeg = glm::rotate(leftLeg, -legRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//        leftLeg = glm::rotate(leftLeg, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        leftLeg = glm::rotate(leftLeg, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        leftLeg = glm::scale(leftLeg, glm::vec3(0.9f, 0.4f, 0.5f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", leftLeg);
//        Leg.Draw();
//    }
//
//    void DrawLeftLegBoll() {
//
//        // Doraimon leftLegBoll
//        glm::mat4 leftLegBoll = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_white);
//        leftLegBoll = glm::translate(leftLegBoll, leftLegBollPosition);
//        //	leftLegBoll = glm::rotate(leftLegBoll, -legRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//        leftLegBoll = glm::rotate(leftLegBoll, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        leftLegBoll = glm::rotate(leftLegBoll, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        leftLegBoll = glm::scale(leftLegBoll, glm::vec3(0.8f, 1.0f, 1.2f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", leftLegBoll);
//        LegBoll.Draw();
//    }
//
//    void DrawTile() {
//
//        // Doraimon tile 
//        glm::mat4 tile = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_red);
//        tile = glm::translate(tile, tilePosition);
//        tile = glm::rotate(tile, glm::radians(00.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        tile = glm::rotate(tile, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        tile = glm::rotate(tile, glm::radians(00.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", tile);
//        Tile.Draw();
//    }
//
//    void DrawNose() {
//
//        // Doraimon nose 
//        glm::mat4 nose = glm::mat4(1.0f);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, t_nose);
//        nose = glm::translate(nose, nosePosition);
//        nose = glm::rotate(nose, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        nose = glm::rotate(nose, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        nose = glm::rotate(nose, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//        SimpleShader.Use();
//
//        SimpleShader.setMat4("model", nose);
//        Nose.Draw();
//    }
//    //////////////////////  finishing drawing  ////////////////////////////////
//
//
//};
//
//#endif
