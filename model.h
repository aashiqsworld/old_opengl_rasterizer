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
#include "stb_image.h"

using namespace std;

class Model
{
public:
    int c = 0;
    Model(char *path)
    {
        loadModel(path);
    }

    void Draw(Shader &shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(shader);
        }
    }
private:
    // model data
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;

    void loadModel(const string& path)
    {
        if(!filesystem::exists(path))
        {
            cout << "File at path does not exist." << endl;
            return;
        }

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        cout << "Attempting to load model at " << path << "." << endl;

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "Error loading model at " << path << "." << endl;
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "." << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        cout << "Model at " << path << "successfully loaded." << endl;

        cout << "Attempting to process model at " << path << "." << endl;
        processNode(scene->mRootNode, scene);
        cout << "Model at " << path << "successfully processed." << endl;
    }
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
//            c++;
//            cout << "Pushing back mesh " << c << endl;
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }
    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if(mesh->mTextureCoords[0]) // does the mesh contain tex coords?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // process indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        //  process material
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                               aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = loadMaterialTextures(material,
                                                                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        return Mesh(vertices, indices, textures);
    }
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName)
    {
        vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            // iterate through existing textures and see if it has already been loaded
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if(!skip)
            {
                // if texture hasn't been loaded, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
            }

        }
        return textures;
    }

    unsigned int TextureFromFile(const char* path, string _directory)
    {
        string filename = _directory + '/' + string(path);

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        if(data)
        {
            GLenum format;
            if(nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
        {
            std::cout << "Failed to load texture at path: " << path << std::endl;
        }

        stbi_image_free(data);

        return textureID;
    }
};

#endif
