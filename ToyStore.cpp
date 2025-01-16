#include "ToyStore.h"
#include <iostream>
#include "Sphere.h"

ToyStore::ToyStore(float width, float height, float depth, unsigned int roofTexture, unsigned int wallTexture, unsigned int floorTexture)
    : storeBody(width, height, depth), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), shelf(100, 20, 30) {
    // Set different textures for roof, walls, and floor
    for (int i = 0; i < 6; i++) {
        if (i == 4) { // Roof
            storeBody.SetTextureForFace(i, roofTexture);
        } else if (i == 5) { // Floor
            storeBody.SetTextureForFace(i, floorTexture);
        } else { // Walls
            storeBody.SetTextureForFace(i, wallTexture);
        }
    }

    storeBody.SetTextureRepeat(1.0f, 1.0f); // Optional texture settings

    // Place the shelf relative to the side index 2 wall
    float shelfWidth = 800.0f;  // Customize shelf dimensions
    float shelfHeight = 20.0f;
    float shelfDepth = 10.0f;
    shelf = Cuboid(shelfWidth, shelfHeight, shelfDepth);
    std::vector<std::pair<Cuboid, glm::vec3>> cuboidsIN;
    // Calculate position relative to side index 2 wall
    float wallX = -(width / 2.0f);  // Side 2 wall is the left wall
    float wallY = height / 2.0f - 40.0f; // Position below the roof
    float wallZ = 0.0f;

    glm::vec3 relativeShelfPos(wallX + shelfDepth, wallY, wallZ);
    shelf.SetPosition(relativeShelfPos.x, relativeShelfPos.y, relativeShelfPos.z);
    shelf.SetRotation(0.0f, 90.0f, 0.0f); // Rotate to align with wall
    cuboidsIN.push_back({ shelf, relativeShelfPos });

    
}

void ToyStore::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
    storeBody.SetPosition(x, y, z);
}

void ToyStore::SetRotation(float x, float y, float z) {
    rotation = glm::vec3(x, y, z);
    storeBody.SetRotation(x, y, z);
}

void ToyStore::SetFaceEnabled(int faceIndex, bool enabled) {
    storeBody.SetFaceEnabled(faceIndex, enabled);
}

void ToyStore::AddTeddyBear(const TeddyBear& teddy, const glm::vec3& relativePos) {
    teddyBears.push_back({ teddy, relativePos });
}

void ToyStore::AddCarToy(const CarToy& car, const glm::vec3& relativePos) {
    carToys.push_back({ car, relativePos });
}

void ToyStore::AddShelf(const Cuboid& shelf, const glm::vec3& relativePos) {
    shelves.push_back({ shelf, relativePos });
}

void ToyStore::AddTable(const Table& table, const glm::vec3& relativePosition) {
    tables.push_back({ table, relativePosition }); 
    std::cout << " ToyStore::AddTable size: " << tables.size() << std::endl;

}

void ToyStore::AddSpheres(const Sphere& sphere, const glm::vec3& relativePos) {
    spheres.push_back({ sphere, relativePos });

}

void ToyStore::Render(Shader& shader) {
    // Render the main ToyStore body
    storeBody.Draw(shader);
    // Render Spheres
    for (auto& spherePair : spheres) {
        Sphere sphere = spherePair.first;
       // glm::vec3 worldPos = position + spherePair.second;
        sphere.SetPosition(11, 114,4);
        sphere.Draw(shader);
    }
    // Render TeddyBears
    for (auto& teddyPair : teddyBears) {
        TeddyBear teddy = teddyPair.first;
        glm::vec3 worldPos = position + teddyPair.second;
        teddy.SetPosition(worldPos);
        teddy.Draw(shader);
    }    
    
    // Render CarToys
    for (auto& carPair : carToys) {
        CarToy car = carPair.first;
        glm::vec3 worldPos = position + carPair.second;
        car.SetPosition(worldPos.x, worldPos.y, worldPos.z);

        std::cout << "Rendering car toy at world position: "
                  << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << std::endl;

        car.Draw(shader);
    }

    // Render Shelves
    for (auto& shelfPair : shelves) {
        Cuboid shelf = shelfPair.first;
        glm::vec3 worldPos = position + shelfPair.second;
        shelf.SetPosition(worldPos.x, worldPos.y, worldPos.z);

        std::cout << "Rendering shelf at world position: "
                  << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << std::endl;

        shelf.Draw(shader);
    }    
    

    // Render Tables
    for (auto& tablePair : tables) {
        Table table = tablePair.first;
        glm::vec3 worldPos = position + tablePair.second;
        table.SetPosition(worldPos.x, worldPos.y, worldPos.z);

        std::cout << "table stales tablesssss "
                  << worldPos.x << ", " << worldPos.y << ", " << worldPos.z <<   std::endl;

        table.Draw(shader);
    
    }
    shelf.Draw(shader);
}
