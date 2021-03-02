#include "main.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <vector>
#include <stdint.h>

#define uint32 uint32_t

//std::vector<unsigned int*> indices[];
std::vector<aiMesh*> meshes;

std::vector<vec3> vertices;
std::vector<vec3> normals;

std::vector<uint32> indices;
//std::vector<unsigned int> indices;
std::vector<vec2> textures;

void ImportFile(const std::string& pFile) {
    // Create an instance of the Importer class
    Assimp::Importer importer;
    
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices);

    if (!scene)
    {
        std::cout << "file not found \n";
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];

        for (int j = 0; j < mesh->mNumVertices; j++)
        {
            //Vertices
            vec3 position = vec3(0.f, 0.f, 0.f);

            position = vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);

            vertices.push_back(position);

            //Normals
            vec3 normal = vec3(0.f, 0.f, 0.f);

            normal = vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

            normals.push_back(normal);

            //Textures
            vec2 texture = vec2(0.f, 0.f);

            if (mesh->mTextureCoords[0])
            {

                texture = vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
            }
            else
            {
                texture = vec2(0.f, 0.f);
            }

            textures.push_back(texture);
        }

        for (int u = 0; u < mesh->mNumFaces; u++)
        {
            aiFace face = mesh->mFaces[u];

            for (unsigned int v = 0; v < face.mNumIndices; v++)
            {
                indices.push_back(face.mIndices[v]);
            }

        }

        meshes.push_back(mesh); // need processed mesh here
    }
}

int main()
{
    ImportFile("blackjack.fbx");

    /*for (int i = 0; i < vertices.size(); i++)
    {
        std::cout << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << std::endl;
        std::cout << normals[i].x << ", " << normals[i].y << ", " << normals[i].z << std::endl;
    }*/

    for (int i = 0; i < indices.size(); i++)
    {
        std::cout << indices[i] << " ";
    }

    return 0;
}