#include "Model.h"
#include "../../ResourceManager.h"
#include "../../Materials/GenericMaterial.h"
#include "../../Materials/BasicDeferredMaterial.h"
#include "../../Materials/DeferredMaterial.h"
#include "../../Materials/EmissiveMaterial.h"
#include <imEngine/Utils/StringUtils.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/System/Filesystem.h>
#include <cstring>
#include <fstream>

namespace imEngine {

//############################## Detail #####################################//

Detail::Detail(const String &name, GeometryPtr geometry, MaterialPtr material, Model *owner) :
        m_name(name),
        m_owner(owner),
        m_geometry(geometry),
        m_material(material)
{ }

void Detail::render(const Mat4 &modelMatrix, const Mat4 &viewMatrix, const Mat4 &projectionMatrix) const {
        Mat4 modelViewMatrix = viewMatrix * modelMatrix;
        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        Mat3 normalMatrix = glm::transpose(Mat3(glm::inverse(modelViewMatrix)));

        m_material->bind();
        m_material->program()->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
        m_material->program()->setUniform("uModelViewMatrix", modelViewMatrix);
        m_material->program()->setUniform("uNormalMatrix", normalMatrix);
        m_geometry->render();
}

//################################ Model #####################################//

Model::Model(const String& name) :
        m_name(name),
        m_owner(nullptr)
{ }

Model::Model(const Model &model) {
        m_name = model.m_name;
        m_aabb = model.m_aabb;
        m_details = model.m_details;
        m_owner = model.m_owner;

        // При копировании нужно сменить владельцев у деталей
        for (Detail& detail: m_details) detail.setOwner(this);
}

void Model::load(const String &filename) {
        m_details.clear();
        loadFromJson(filename);
        recalculateAABB();
}

void Model::loadFromJson(const String &filename) {
        Json::Value root;
        Json::Reader reader;

        std::ifstream ifile(filename, std::ifstream::binary);
        bool isOk = reader.parse(ifile, root, false);
        if (!isOk) {
                IM_ERROR("Cannot open file " << filename << ": " << reader.getFormatedErrorMessages());
                return;
        }

        loadFromJson(root);
}

void Model::loadFromJson(const JsonValue &root) {
        JsonValue details = root["details"];
        JsonValue materials = root["materials"];

        for (const Json::Value& detail: details) {
                String name = detail.get("name", "").asString();
                String materialName = detail.get("material", "").asString();
                String vertices = detail.get("vertices", "").asString();
                String indices = detail.get("indices", "").asString();

                if (vertices.empty() || indices.empty()) {
                        IM_ERROR("Detail doesn't contain mesh data");
                        return;
                }
                if (materialName.empty()) {
                        IM_ERROR("Detail has not material");
                        return;
                }

                GeometryPtr geometry = loadGeometry(vertices, indices);
                MaterialPtr material = loadMaterial(materials[materialName]);

                m_details.push_back(Detail(name, geometry, material, this));
        }
}

GeometryPtr Model::loadGeometry(const String &encodedVertices, const String &encodedIndices) {
        String decodedVertices = StringUtils::fromBase64(encodedVertices);
        String decodedIndices = StringUtils::fromBase64(encodedIndices);

        uint numVertices = decodedVertices.size() / sizeof(Vertex);
        uint numIndices = decodedIndices.size() / sizeof(uint);

        VertexList vertices;
        vertices.resize(numVertices);
        std::memcpy(&(vertices[0]), decodedVertices.c_str(), decodedVertices.size());

        IndexList indices;
        indices.resize(numIndices);
        std::memcpy(&(indices[0]), decodedIndices.c_str(), decodedIndices.size());

        return GeometryPtr(new Geometry(vertices, indices));
}

MaterialPtr Model::loadMaterial(const JsonValue &material) {
        String type = material.get("type", "").asString();
        if (type.empty()) {
                IM_ERROR("Material has not type");
                return MaterialPtr();
        }

        if (type == "GENERIC") {
                GenericMaterial* result = new GenericMaterial();
                result->loadFromJson(material);
                return MaterialPtr((Material*)result);
        } else if (type == "EMISSIVE") {
                EmissiveMaterial* result = new EmissiveMaterial();
                result->loadFromJson(material);
                return MaterialPtr((Material*)result);
        } else {
                IM_ERROR(type << " is unknown type");
                return MaterialPtr();
        }
}

void Model::recalculateAABB() {
        float minFloat = std::numeric_limits<float>::min();
        float maxFloat = std::numeric_limits<float>::max();
        m_aabb.max = Vec3(minFloat);
        m_aabb.min = Vec3(maxFloat);

        for (const Detail& detail: m_details) {
                AABB geometryAABB = detail.geometry()->aabb();
                m_aabb.max = glm::max(m_aabb.max, geometryAABB.max);
                m_aabb.min = glm::min(m_aabb.min, geometryAABB.min);
        }
}


} //namespace imEngine
