#include <imEngine/Utils/Debug.h>
#include <imEngine/Math/Vertex.h>
#include <imEngine/Utils/Types.h>
#include <imEngine/Utils/StringUtils.h>

#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>
#include <assimp/aiMaterial.h>
#include <assimp/aiMesh.h>

#include <map>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace imEngine;

//############################################################################//

/*
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

#include <iostream>

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

//############################################################################//

typedef std::vector<Vertex> VertexList;
typedef std::vector<uint> IndexList;

/** @brief Класс, содержащий данный о геометрии части меша
 */
class DetailContainer {
public:
        /// Конструктор
        DetailContainer();

        /// Парсит меш с помощью Assimp
        void            load(const aiMesh* mesh);
        /// Устанавливает материал
        void            setMaterial(const String& materialName);

        /// Сериализует в Json
        StringList      serialize() const;

private:
        VertexList      loadVertices(const aiMesh* mesh) const;
        IndexList       loadIndices(const aiMesh* mesh) const;
        String          vertices() const;
        String          indices() const;

private:
        String          m_name;
        VertexList      m_vertices;
        IndexList       m_indices;
        String          m_materialName;
};

//###################### DetailContainer #####################################//

DetailContainer::DetailContainer()
{ }

void DetailContainer::load(const aiMesh* mesh) {
        IM_ASSERT(mesh->HasPositions());
        IM_ASSERT(mesh->HasNormals());
        IM_ASSERT(mesh->HasTangentsAndBitangents());
        IM_ASSERT(mesh->HasTextureCoords(0));

        m_name = mesh->mName.data;
        m_vertices = loadVertices(mesh);
        m_indices = loadIndices(mesh);
}

void DetailContainer::setMaterial(const String &materialName) {
        m_materialName = materialName;
}

StringList DetailContainer::serialize() const {
        StringList result;

        result.push_back("{");
        result.push_back("\t\"name\"\t\t:\t\"" + m_name + "\",");
        result.push_back("\t\"material\"\t:\t\"" + m_materialName + "\",");
        result.push_back("\t\"vertices\"\t:\t\"" + vertices() + "\",");
        result.push_back("\t\"indices\"\t:\t\"" + indices() + "\",");
        result.push_back("}");

        return result;
}

VertexList DetailContainer::loadVertices(const aiMesh* mesh) const {
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

IndexList DetailContainer::loadIndices(const aiMesh* mesh) const {
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

String DetailContainer::vertices() const {
        uint len = m_vertices.size() * sizeof(Vertex);
        ubyte* data = (ubyte*) &(m_vertices[0]);
        return base64_encode(data, len);
}

String DetailContainer::indices() const {
        uint len = m_indices.size() * sizeof(uint);
        ubyte* data = (ubyte*) &(m_indices[0]);
        return base64_encode(data, len);
}

//############################################################################//

/** @brief Тип материала
 */
enum class MaterialType {
        NONE,
        GENERIC,
        TRANSPARENT,
        EMISSIVE,
        REFRACTION
};

/** @brief Класс, содержащий настройки материала
 */
class MaterialContainer {
public:
        /// Конструктор
        MaterialContainer();

        /// Парсит материал с помощью Assimp
        void            load(const aiMaterial* material);

        /// Возвращает имя материала
        String          name() const;

        /// Сериализует материал в Json
        StringList      serialize() const;

private:
        Vec3            toVec3(const aiColor3D& color) const;
        String          toJsonVec(const Vec3& vec) const;

private:
        String          m_name;

        /// Generic
        Vec3            m_ambientColor;
        Vec3            m_diffuseColor;
        Vec3            m_specularColor;
        float           m_specularPower;
        String          m_diffuseMap;
        String          m_normalMap;
        String          m_specularMap;
        /// Emissive
        float           m_emissive;
        /// Transparent
        float           m_opacity;
        /// Refraction
        float           opticalDensity;

        MaterialType    m_type;
};

//########################### MaterialContainer ###############################//

MaterialContainer::MaterialContainer() :
        m_type(MaterialType::NONE)
{ }

void MaterialContainer::load(const aiMaterial *material) {
        aiString name;
        aiColor3D ambientColor;
        aiColor3D diffuseColor;
        aiColor3D specularColor;

        material->Get(AI_MATKEY_NAME, name);
        m_name = name.data;

        material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
        m_ambientColor = toVec3(ambientColor);

        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        m_diffuseColor = toVec3(diffuseColor);

        material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        m_specularColor = toVec3(specularColor);

        m_type = MaterialType::GENERIC;
}

String MaterialContainer::name() const {
        return m_name;
}

StringList MaterialContainer::serialize() const {
        StringList result;

        result.push_back("{");
        result.push_back("\t\"name\"\t\t\t:\t\"" + m_name + "\",");
        result.push_back("\t\"ambientColor\"\t:\t\"" + toJsonVec(m_ambientColor) + "\",");
        result.push_back("\t\"diffuseColor\"\t:\t\"" + toJsonVec(m_diffuseColor) + "\",");
        result.push_back("\t\"specularColor\"\t:\t\"" + toJsonVec(m_ambientColor) + "\",");
        result.push_back("}");

        return result;
}

Vec3 MaterialContainer::toVec3(const aiColor3D &color) const {
        return Vec3(color.r, color.b, color.b);
}

String MaterialContainer::toJsonVec(const Vec3 &vec) const {
        std::stringstream ss;
        ss << "[" << vec.x <<  ", " << vec.y << ", " << vec.z << "]";
        return ss.str();
}

//############################################################################//

typedef std::vector<MaterialContainer> MaterialList;
typedef std::vector<DetailContainer> DetailList;

class Mesh {
public:
        /// Конструктор
        Mesh(const String& filename);

        /// Загружает модель и парсит ее
        void            load(const String& filename);

        /// Возвращает Json-строку меша
        String          serialize() const;

private:
        void            loadDetails(const aiScene* scene);
        void            loadMaterials(const aiScene* scene);

        String          serializeMaterials(uint n, const MaterialList& materials) const;
        String          serializeDetails(uint n, const DetailList& details) const;
        String          serializeDetail(uint n, const DetailContainer& detail) const;

        String          indent(uint n) const;
private:
        DetailList      m_details;
        MaterialList    m_materials;
};

//################################# Mesh #####################################//

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

        loadMaterials(scene);
        loadDetails(scene);
}

String Mesh::serialize() const {
        String result;
        result += "{\n";

        result += serializeMaterials(1, m_materials);
        result.erase(result.size()-1);
        result += ",\n";

        result += "\n";

        result += serializeDetails(1, m_details);

        result += "}\n";
        return result;
}

void Mesh::loadMaterials(const aiScene *scene) {
        for (uint i = 0; i < scene->mNumMaterials; ++i) {
                MaterialContainer material;
                material.load(scene->mMaterials[i]);
                m_materials.push_back(material);
        }
}


void Mesh::loadDetails(const aiScene *scene) {
        for (uint i = 0; i < scene->mNumMeshes; ++i) {
                DetailContainer detail;
                detail.load(scene->mMeshes[i]);
                detail.setMaterial(m_materials[scene->mMeshes[i]->mMaterialIndex].name());
                m_details.push_back(detail);
        }
}

String Mesh::serializeMaterials(uint n, const MaterialList &materials) const {
        String result;

        result += indent(n) + "\"materials\":\n";
        result += indent(n) + "[\n";

        for (const MaterialContainer& material: materials) {
                StringList m = material.serialize();
                String ms = "\t\t" + StringUtils::join(m, "\n\t\t");
                result += ms + ",\n";
        }
        result.erase(result.size()-2);
        result += "\n";

        result += indent(n) + "]\n";

        return result;
}

String Mesh::serializeDetails(uint n, const DetailList &details) const {
        String result;

        result += indent(n) + "\"details\":\n";
        result += indent(n) + "[\n";

        for (const DetailContainer& detail: details) {
                StringList d = detail.serialize();
                String ds = "\t\t" + StringUtils::join(d, "\n\t\t");
                result += ds + ",\n";
        }
        result.erase(result.size()-2);
        result += "\n";

        result += indent(n) + "]\n";

        return result;
}

String Mesh::indent(uint n) const {
        String result;
        for (uint i = 0; i < n; ++i) result += "\t";
        return result;
}

int main() {
        Mesh mesh("tank.obj");
        std::ofstream file("tank.json", std::ofstream::out);
        file << mesh.serialize() << std::endl;
        file.close();
}

