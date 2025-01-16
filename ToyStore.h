#ifndef TOYSTORE_H
#define TOYSTORE_H

#include "Shader.h"
#include "Sphere.h"

#include <vector>
#include <utility>
#include "Cuboid.h"
#include "TeddyBear.h"
#include "CarToy.h"
#include "Table.h"

class ToyStore {
public:
    ToyStore(float width, float height, float depth, unsigned int roofTexture, unsigned int wallTexture, unsigned int floorTexture);

    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void SetFaceEnabled(int faceIndex, bool enabled);

    void AddShelf(const Cuboid& shelf, const glm::vec3& relativePosition);
    void AddTeddyBear(const TeddyBear& teddy, const glm::vec3& relativePos);
    void AddSpheres(const Sphere& sphere, const glm::vec3& relativePos);
    void AddCarToy(const CarToy& car, const glm::vec3& relativePos);
    void AddTable(const Table& table, const glm::vec3& relativePos);

    void Render(Shader& shader);

private:
    Cuboid shelf;
    Cuboid storeBody;
    

    glm::vec3 position;
    glm::vec3 rotation;

    std::vector<std::pair<TeddyBear, glm::vec3>> teddyBears;
    std::vector<std::pair<Sphere, glm::vec3>> spheres;
    std::vector<std::pair<CarToy, glm::vec3>> carToys;
    std::vector<std::pair<Cuboid, glm::vec3>> shelves; // Shelves now store Cuboid + relative position
    std::vector<std::pair<Table, glm::vec3>> tables;

};
#endif // TOYSTORE_H
