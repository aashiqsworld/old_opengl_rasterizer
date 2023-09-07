//
// Created by Aashiq Shaikh on 9/6/23.
//

#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include "include/glm/glm.hpp"
#include "shader.h"
#include "mesh.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Model
{
public:
    Model(char *path)
    {
//        loadModel(path);
    }

    void Draw(Shader &shader);
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName);

};

#endif
