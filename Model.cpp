//
//// Model.cpp
//#include "Model.h"
//#include <stb_image.h>
//#include <iostream>
//
//Model::Model(const std::string& path) {
//    loadModel(path);
//}
//
//void Model::draw(unsigned int shaderProgram) {
//    for (Mesh& mesh : meshes) {
//        mesh.draw(shaderProgram);
//    }
//}
//
//void Model::loadModel(const std::string& path) {
//    Assimp::Importer importer;
//    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
//
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
//        return;
//    }
//
//    directory = path.substr(0, path.find_last_of('/'));
//    processNode(scene->mRootNode, scene);
//}
//
//void Model::processNode(aiNode* node, const aiScene* scene) {
//    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        meshes.push_back(processMesh(mesh, scene));
//    }
//
//    for (unsigned int i = 0; i < node->mNumChildren; i++) {
//        processNode(node->mChildren[i], scene);
//    }
//}
//
//Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//    std::vector<Texture> textures;
//
//    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
//        Vertex vertex;
//
//        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
//        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
//
//        if (mesh->mTextureCoords[0]) {
//            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
//        }
//        else {
//            vertex.texCoords = glm::vec2(0.0f, 0.0f);
//        }
//
//        vertices.push_back(vertex);
//    }
//
//    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
//        aiFace face = mesh->mFaces[i];
//        for (unsigned int j = 0; j < face.mNumIndices; j++) {
//            indices.push_back(face.mIndices[j]);
//        }
//    }
//
//    if (mesh->mMaterialIndex >= 0) {
//        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//
//        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//
//        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//    }
//
//    return Mesh(vertices, indices, textures);
//}
//
//std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
//    std::vector<Texture> textures;
//    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//
//        Texture texture;
//        texture.id = textureFromFile(str.C_Str(), directory);
//        texture.type = typeName;
//        texture.path = str.C_Str();
//        textures.push_back(texture);
//    }
//    return textures;
//}
//
//
//unsigned int Model::textureFromFile(const char* path, const std::string& directory) {
//    std::string filename = directory + "/" + path;
//
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//    if (data) {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else {
//        std::cerr << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}