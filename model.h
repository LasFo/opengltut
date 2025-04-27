#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include "mesh.h"
#include "shader.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
public:
  Model(const std::filesystem::path path) {
    loadModel(path);
  }
  void Draw(Shader &shader) {
    for (const Mesh &m : meshes)
      m.Draw(shader);
  }

  std::vector<Texture> textures_loaded;
  std::vector<Mesh> meshes;
  std::string directory;
private:
  // model data

  void loadModel(const std::filesystem::path path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      fprintf(stderr, "assimp::Importer::ReadFile failed: %s\n", import.GetErrorString());
      return;
    }
    directory = path.parent_path();

    processNode(scene->mRootNode, scene);
  }

  void processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int numTextureCoords = 0;
    for (int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++) {
      numTextureCoords += mesh->mTextureCoords[i] != nullptr;
    }
    if (numTextureCoords > 1) {
      fprintf(stderr, "Warning: meshes with >1 texture coordinates are not supported: got %d\nignoring all texture coordinates but the first.", numTextureCoords);
    }

    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;

      // Positions
      glm::vec3 vector;
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;

      // normals
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;

      // texture coordinates
      if (mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
        glm::vec2 vec;
        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.TexCoords = vec;
      } else {
        vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      }
      vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    // process material
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                              aiTextureType_DIFFUSE, "texture_diffuse");
      std::vector<Texture> specularMaps = loadMaterialTextures(material,
                                                               aiTextureType_SPECULAR, "texture_specular");
      textures.reserve(diffuseMaps.size() + specularMaps.size());
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
  }

  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);
      bool already_loaded = false;
      for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0) {
          textures.push_back(textures_loaded[j]);
          already_loaded = true;
          break;
        }
      }
      if (already_loaded)
        continue;
      Texture texture;
      texture.id = TextureFromFile(str.C_Str(), directory);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
    return textures;
  }
};
#endif
