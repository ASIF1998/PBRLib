//
//  mesh.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 28/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <queue>

using namespace Assimp;

namespace PRGE
{
    vector<Mesh> Mesh::loadModel(const string_view path)
    {
        Importer import;
        auto* scene = import.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        vector<Mesh> meshes;
        queue<aiNode*> nodes;
        aiNode* node;
        aiMesh* mesh;
        size_t numMeshes = 0;

        nodes.push(scene->mRootNode);

        while(!nodes.empty()) {
            node = nodes.front();
            nodes.pop();

            for (size_t i{0}; i < node->mNumMeshes; i++) {
                mesh = scene->mMeshes[node->mMeshes[i]];

                meshes.push_back({mesh->mNumVertices, mesh->mNumFaces * 3});

                for (size_t j{0}; j < mesh->mNumVertices; j++) {
                    meshes[numMeshes].vertices[j][0] = mesh->mVertices[j].x;
                    meshes[numMeshes].vertices[j][1] = mesh->mVertices[j].y;
                    meshes[numMeshes].vertices[j][2] = mesh->mVertices[j].z;

                    meshes[numMeshes].normals[j][0] = mesh->mNormals[j].x;
                    meshes[numMeshes].normals[j][1] = mesh->mNormals[j].y;
                    meshes[numMeshes].normals[j][2] = mesh->mNormals[j].z;

                    meshes[numMeshes].tangets[j][0] = mesh->mTangents[j].x;
                    meshes[numMeshes].tangets[j][1] = mesh->mTangents[j].y;
                    meshes[numMeshes].tangets[j][2] = mesh->mTangents[j].z;

                    meshes[numMeshes].uvs[j][0] = mesh->mTextureCoords[0][j].x;
                    meshes[numMeshes].uvs[j][1] = mesh->mTextureCoords[0][j].y;

                    for (size_t c{0}, k{0}; k < mesh->mNumFaces; c++, k += 3) {
                        meshes[numMeshes].indices[k] = mesh->mFaces[c].mIndices[0];
                        meshes[numMeshes].indices[k + 1] = mesh->mFaces[c].mIndices[1];
                        meshes[numMeshes].indices[k + 2] = mesh->mFaces[c].mIndices[2];
                    }
                }

                numMeshes++;
            }

            for (size_t i{0}; i < node->mNumChildren; i++) {
                nodes.push(node->mChildren[i]);
            }
        }

        import.FreeScene();
        meshes.shrink_to_fit();

        return meshes;
    }
}
