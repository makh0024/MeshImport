#include "main.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <vector>
#include <stdint.h>
#include <fstream>

#define uint32 uint32_t

using namespace std;

std::vector<aiMesh*> meshes;

std::vector<VertFormat> vertices;
std::vector<uint32> indices;

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
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];

        for (int j = 0; j < mesh->mNumVertices; j++)
        {
            //Vertices
            vec3 position = vec3(0.f, 0.f, 0.f);

            position = vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);

            //vertices.push_back(position);

            //Normals
            vec3 normal = vec3(0.f, 0.f, 0.f);

            normal = vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

            //normals.push_back(normal);

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

            //textures.push_back(texture);

            VertFormat vertex = VertFormat(position, normal, texture);

            vertices.push_back(vertex);
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
    ImportFile("Cube.fbx");

    /*for (int i = 0; i < vertices.size(); i++)
    {
        std::cout << "Vertex " << i << ": " << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << std::endl;
        std::cout << "Normal " << i << ": " << normals[i].x << ", " << normals[i].y << ", " << normals[i].z << std::endl;
    }

    for (int i = 0; i < indices.size(); i++)
    {
        if (i % 3 == 0)
            std::cout << std::endl;
       
        if (i % 3 == 0)
            std::cout << "Triangle " << i / 3 << ": ";

        std::cout << indices[i] << ", ";

    }
    cout << endl;
    for (int i = 0; i < textures.size(); i++)
    {
        std::cout << "Texture " << i << ": ";

        std::cout << textures[i].x << ", " << textures[i].y << endl;

    }*/

    ofstream newMesh("newMesh.FMS", ios::out | ios::binary);
    
    if (!newMesh)
    {
        cout << "Cannot open file!" << endl;
        return 1;
    }

    newMesh.write(".FMS", 4);

    for (int i = 0; i < vertices.size(); i++)
    {
        char* vertX = (char*) (&vertices[i].position.x);
        char* vertY = (char*) (&vertices[i].position.y);
        char* vertZ = (char*) (&vertices[i].position.z);

        newMesh.write(vertX, sizeof(float));
        newMesh.write(vertY, sizeof(float));
        newMesh.write(vertZ, sizeof(float));

        char* normX = (char*)(&vertices[i].normal.x);
        char* normY = (char*)(&vertices[i].normal.y);
        char* normZ = (char*)(&vertices[i].normal.z);

        newMesh.write(normX, sizeof(float));
        newMesh.write(normY, sizeof(float));
        newMesh.write(normZ, sizeof(float));

        char* textX = (char*)(&vertices[i].texture.x);
        char* textY = (char*)(&vertices[i].texture.y);

        newMesh.write(textX, sizeof(float));
        newMesh.write(textY, sizeof(float));
    }

    newMesh.close();

    return 0;
}