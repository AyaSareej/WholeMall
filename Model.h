#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

#include <string>
#include <vector>
#include <iostream>


class Model {
public:
    glm::vec3 position = glm::vec3(0.0f); // Default position

    Model(const std::string& path) {
        loadModel(path);
    }

    // Set position for the model
    void SetPosition(float x, float y, float z) {
        position = glm::vec3(x, y, z);
    }

    // Set scale for all meshes
    void SetScale(float x, float y, float z) {
        for (Mesh& mesh : meshes) {
            mesh.SetScale(x, y, z);
        }
    }

    void Draw(Shader& shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position); // Apply position
        shader.setMat4("model", model);          // Send model matrix to shader

        for (Mesh& mesh : meshes) {
            shader.setVec3("scale", mesh.scale); // Send scale to shader
            mesh.Draw(shader);
        }
    }

private:
    std::vector<Mesh> meshes;

    void loadModel(const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        return Mesh(vertices, indices, {});
    }
};

#endif
