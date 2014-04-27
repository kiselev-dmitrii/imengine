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
        result.push_back("\t\"indices\"\t:\t\"" + indices() + "\"");
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
                if (mesh->HasTextureCoords(0)) v.texcoords = Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                else IM_LOG("WARNING: Mesh has no texture coords");
                if (mesh->HasTangentsAndBitangents()) v.tangent = Vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                else IM_LOG("WARNING: Mesh has no tangent vectors");

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
        return StringUtils::toBase64(data, len);
}

String DetailContainer::indices() const {
        uint len = m_indices.size() * sizeof(uint);
        ubyte* data = (ubyte*) &(m_indices[0]);
        return StringUtils::toBase64(data, len);
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
        String          toString(const Vec3& vec) const;
        String          toString(float value) const;

private:
        String          m_name;

        /// Generic
        Vec3            m_ambientColor;
        Vec3            m_diffuseColor;
        Vec3            m_specularColor;
        float           m_specularPower;
        String          m_diffuseMap;
        String          m_normalMap;
        String          m_heightMap;
        String          m_specularMap;
        /// Emissive
        Vec3            m_emissiveColor;
        /// Transparent
        Vec3            m_transparentColor;
        /// Refraction
        float           m_refractionIndex;

        MaterialType    m_type;
};

//########################### MaterialContainer ###############################//

MaterialContainer::MaterialContainer() :
        m_type(MaterialType::NONE)
{ }

void MaterialContainer::load(const aiMaterial *material) {
        aiColor3D outColor;

        /// Generic type
        aiString name;
        material->Get(AI_MATKEY_NAME, name);
        m_name = name.data;

        material->Get(AI_MATKEY_COLOR_AMBIENT, outColor);
        m_ambientColor = toVec3(outColor);

        material->Get(AI_MATKEY_COLOR_DIFFUSE, outColor);
        m_diffuseColor = toVec3(outColor);

        material->Get(AI_MATKEY_COLOR_SPECULAR, outColor);
        m_specularColor = toVec3(outColor);

        material->Get(AI_MATKEY_SHININESS, m_specularPower);

        aiString diffuseMap;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseMap);
        m_diffuseMap = diffuseMap.data;

        aiString normalMap;
        material->GetTexture(aiTextureType_NORMALS, 0, &normalMap);
        m_normalMap = normalMap.data;

        aiString heightMap;
        material->GetTexture(aiTextureType_HEIGHT, 0, &heightMap);
        m_heightMap = heightMap.data;

        aiString specularMap;
        material->GetTexture(aiTextureType_SPECULAR, 0, &specularMap);
        m_specularMap = specularMap.data;

        m_type = MaterialType::GENERIC;

        /// Emissive type
        material->Get(AI_MATKEY_COLOR_EMISSIVE, outColor);
        m_emissiveColor = toVec3(outColor);
        if (glm::length(m_emissiveColor) != 0.0f) m_type = MaterialType::EMISSIVE;

        /// Transparent type
        material->Get(AI_MATKEY_COLOR_TRANSPARENT, outColor);
        m_transparentColor = toVec3(outColor);
        if (glm::length(m_transparentColor) != 0.0f) m_type = MaterialType::TRANSPARENT;

        /// Refraction type
        material->Get(AI_MATKEY_REFRACTI, m_refractionIndex);
        if (m_refractionIndex != 1.0) m_type = MaterialType::REFRACTION;
}

String MaterialContainer::name() const {
        return m_name;
}

StringList MaterialContainer::serialize() const {
        StringList result;

        result.push_back("\"" + m_name + "\": {");

        if (m_type == MaterialType::GENERIC) {
                result.push_back("\t\"ambientColor\"\t:\t" + toString(m_ambientColor) + ",");
                result.push_back("\t\"diffuseColor\"\t:\t" + toString(m_diffuseColor) + ",");
                result.push_back("\t\"specularColor\"\t:\t" + toString(m_specularColor) + ",");
                result.push_back("\t\"specularPower\"\t:\t" + toString(m_specularPower) + ",");

                if (!m_diffuseMap.empty()) result.push_back("\t\"diffuseMap\"\t:\t\"" + m_diffuseMap + "\",");
                if (!m_normalMap.empty()) result.push_back("\t\"normalMap\"\t:\t\"" + m_normalMap + "\",");
                if (!m_heightMap.empty()) result.push_back("\t\"heightMap\"\t\t:\t\"" + m_heightMap + "\",");
                if (!m_specularMap.empty()) result.push_back("\t\"specularMap\"\t:\t\"" + m_specularMap + "\",");
                result.push_back("\t\"type\"\t\t\t:\t\"GENERIC\"");

        } else if (m_type == MaterialType::EMISSIVE) {
                result.push_back("\t\"emissiveColor\"\t:\t" + toString(m_emissiveColor) + ",");
                result.push_back("\t\"type\"\t\t\t:\t\"EMISSIVE\"");

        } else if (m_type == MaterialType::TRANSPARENT) {
                result.push_back("\t\"transparentColor\"\t:\t" + toString(m_transparentColor) + ",");
                result.push_back("\t\"type\"\t\t\t:\t\"TRANSPARENT\"");

        } else if (m_type == MaterialType::REFRACTION) {
                result.push_back("\t\"refractionIndex\"\t:\t" + toString(m_refractionIndex) + ",");
                result.push_back("\t\"type\"\t\t\t:\t\"REFRACTION\"");

        }

        result.push_back("}");

        return result;
}

Vec3 MaterialContainer::toVec3(const aiColor3D &color) const {
        return Vec3(color.r, color.g, color.b);
}

String MaterialContainer::toString(const Vec3 &vec) const {
        std::stringstream ss;
        ss << "[" << vec.x <<  ", " << vec.y << ", " << vec.z << "]";
        return ss.str();
}

String MaterialContainer::toString(float value) const {
        std::stringstream ss;
        ss << value;
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

        result += indent(n) + "\"materials\": {\n";

        for (const MaterialContainer& material: materials) {
                StringList m = material.serialize();
                String ms = "\t\t" + StringUtils::join(m, "\n\t\t");
                result += ms + ",\n";
        }
        result.erase(result.size()-2);
        result += "\n";

        result += indent(n) + "}\n";

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

int main(int argc, char** argv) {
        if (argc != 3) {
                std::cout << "Help: ./MeshConverter <obj file> <json file>" << std::endl;
                return 0;
        }

        Mesh mesh(argv[1]);
        std::ofstream file(argv[2], std::ofstream::out);
        file << mesh.serialize() << std::endl;
        file.close();
}

