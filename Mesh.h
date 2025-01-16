//// Mesh.h
//#ifndef MESH_H
//#define MESH_H
//
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <vector>
//#include <string>
//
//struct Vertex {
//    glm::vec3 position;
//    glm::vec3 normal;
//    glm::vec2 texCoords;
//};
//
//struct Texture {
//    unsigned int id;
//    std::string type;
//    std::string path;
//};
//
//class Mesh {
//public:
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//    std::vector<Texture> textures;
//    unsigned int VAO;
//
//    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
//    void draw(unsigned int shaderProgram);
//
//private:
//    unsigned int VBO, EBO;
//    void setupMesh();
//};
//
//#endif // MESH_H
//
