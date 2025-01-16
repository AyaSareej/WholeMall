//// Model.h
//#ifndef MODEL_H
//#define MODEL_H
//
//#include <vector>
//#include <string>
//#include <assimp/scene.h>
//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include "Mesh.h"
//
//class Model {
//public:
//    Model(const std::string& path);
//    void draw(unsigned int shaderProgram);
//
//private:
//    std::vector<Mesh> meshes;
//    std::string directory; // Stores the directory of the loaded model
//
//    void loadModel(const std::string& path);
//    void processNode(aiNode* node, const aiScene* scene);
//    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
//
//    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
//    unsigned int textureFromFile(const char* path, const std::string& directory);
//};
//
//#endif