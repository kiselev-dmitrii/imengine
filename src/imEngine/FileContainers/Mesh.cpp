#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <imEngine/Utils/Debug.h>

namespace imEngine {


Mesh::Mesh(const String& filename) {
        load(filename);
}

void Mesh::load(const String &filename) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
        if (!scene) {
                IM_ERROR("Cannot load mesh from file " << filename);
                IM_ERROR(importer.GetErrorString());
                return;
        }
        IM_ASSERT(scene->mNumMeshes == 1);              //в файле содержится только один меш

        const aiMesh* mesh = scene->mMeshes[0];
        IM_ASSERT(mesh->HasPositions());
        IM_ASSERT(mesh->HasNormals());
        IM_ASSERT(mesh->HasTangentsAndBitangents());
        IM_ASSERT(mesh->HasTextureCoords(0));

        m_vertices = loadVertices(mesh);
        m_indices = loadIndices(mesh);
}

const VertexList& Mesh::vertices() const {
        return m_vertices;
}

const IndexList& Mesh::indices() const {
        return m_indices;
}

IndexList Mesh::loadIndices(const aiMesh* mesh) {
        IndexList indices;
        indices.reserve(mesh->mNumFaces * 3);   //предполагается, что каждая грань - треугольник

        for (uint i = 0; i < mesh->mNumFaces; ++i) {
                const aiFace& face = mesh->mFaces[i];
                IM_ASSERT(face.mNumIndices == 3);

                indices.push_back(face.mIndices[0]);
                indices.push_back(face.mIndices[1]);
                indices.push_back(face.mIndices[2]);
        }

        return indices;
}

VertexList Mesh::loadVertices(const aiMesh *mesh) {
        VertexList vertices;
        vertices.reserve(mesh->mNumVertices);

        for (uint i = 0; i < mesh->mNumVertices; ++i) {
                Vertex v;
                v.position = Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                v.normal = Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                v.tangent = Vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                v.texcoords = Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

                vertices.push_back(v);
        }

        return vertices;
}


} //namespace imEngine
