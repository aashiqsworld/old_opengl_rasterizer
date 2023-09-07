//
// Created by Aashiq Shaikh on 9/6/23.
//

#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include "include/glm/glm.hpp"
#include "shader.h"
#include "mesh.h"



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

    void loadModel(std::string path);
//    void processNode(aiNode

};

#endif
