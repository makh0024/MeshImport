#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "main.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <vector>
#include <stdint.h>
#include <fstream>
#include <string>
#include <experimental/filesystem>

#define uint32 uint32_t

using namespace std;

namespace fs = std::experimental::filesystem;

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
        cout << pFile;
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

            //Normals
            vec3 normal = vec3(0.f, 0.f, 0.f);

            normal = vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

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

            VertFormat vertex = VertFormat(position, normal, texture);

            vertices.push_back(vertex);
        }

        std::vector<VertFormat> finalVert;

        for (int u = 0; u < mesh->mNumFaces; u++)
        {
            aiFace face = mesh->mFaces[u];

            for (unsigned int v = 0; v < face.mNumIndices; v++)
            {
                finalVert.push_back(vertices.at(face.mIndices[v]));
            }
        }

        vertices = finalVert;

        meshes.push_back(mesh);
    }
}

void ImportAndWrite(const std::string& filename)
{
    ImportFile(filename/* + ".fbx"*/);

    string fileNameWithoutExtension = filename.substr(0, filename.rfind("."));

    ofstream newMesh(fileNameWithoutExtension + ".fms", ios::out | ios::binary);

    if (!newMesh)
    {
        cout << "Cannot open file!" << endl;
        return;
    }

    newMesh.write(".fms", 4);

    int numVerts = vertices.size();

    newMesh.write((char*)&numVerts, sizeof(int));

    for (int i = 0; i < vertices.size(); i++)
    {
        char* vertX = (char*)(&vertices[i].position.x);
        char* vertY = (char*)(&vertices[i].position.y);
        char* vertZ = (char*)(&vertices[i].position.z);

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

    return;
}

void CurrentFolderReadMain()
{
    fs::path folder = fs::current_path();

    for (auto& file : fs::directory_iterator(folder))
    {
        if (file.path().extension() == ".fbx")
        {
            ImportAndWrite(file.path().string());
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Checking for all .fbx models in current folder!");

        CurrentFolderReadMain();

        return 0;
    }
    cout << argv[1] << endl;

    string filename;

    filename = argv[1];

    fs::path file = filename;

    if (file.extension() == ".fbx")
    {
        ImportAndWrite(file.string());
    }

    return 0;
}