#include "Model.h"
#include "../../ResourceManager.h"
#include "../../Materials/BasicDeferredMaterial.h"
#include "../../Materials/DeferredMaterial.h"
#include "../../Materials/EmissiveMaterial.h"
#include <imEngine/Utils/StringUtils.h>
#include <imEngine/Utils/Debug.h>

namespace imEngine {

//############################## Detail #####################################//

Detail::Detail(const String &name, Geometry* geometry, MaterialPtr material, Model *owner) :
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

Model::Model(const String &name, const MapStringDetail &details) :
        m_name(name),
        m_details(details),
        m_owner(nullptr)
{ }

Model::Model(const String &name, const String& geometry, const MaterialPtr &material) :
        m_name(name),
        m_owner(nullptr)
{
        Geometry* g = RESOURCES->geometry()->geometry(geometry);
        m_details.push_back(Detail("unnamed", g, material, this));
        recalculateAABB();
}

Model::Model(const String &name, Geometry* geometry, const MaterialPtr &material) :
        m_name(name),
        m_owner(nullptr)
{
        m_details.push_back(Detail("unnamed", geometry, material, this));
        recalculateAABB();
}

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
        loadFromXML(filename);
        recalculateAABB();
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

void Model::loadFromXML(const String &filename) {
        XmlDocument doc;
        XmlResult result = doc.load_file(filename.c_str());
        if (!result) {
                IM_ERROR("Cannot open file " << filename);
                return;
        }

        loadFromXML(doc.first_child());
}

void Model::loadFromXML(const XmlNode &modelNode) {
        for (const XmlNode& detailNode: modelNode.children()) {
                String name = detailNode.attribute("name").value();
                Geometry* geometry = createGeometry(detailNode.child("geometry"));
                MaterialPtr material = createMaterial(detailNode.child("material"));

                m_details.push_back(Detail(name, geometry, material, this));
        }
}

Geometry* Model::createGeometry(const XmlNode &geometryNode) {
        IM_VAR(geometryNode.name());
        String filename = geometryNode.attribute("filename").value();
        if (filename == "") {
                IM_ERROR("Tag geometry must contain attribute filename");
                return nullptr;
        }

        return RESOURCES->geometry()->geometry(filename);
}

MaterialPtr Model::createMaterial(const XmlNode &materialNode) {
        String type = materialNode.attribute("type").value();
        if (type == "") {
                IM_ERROR("Tag material must contain attribute type");
                return MaterialPtr();
        }

        if (type == "basic_deferred") {
                BasicDeferredMaterial* result = new BasicDeferredMaterial();
                result->loadFromXML(materialNode);
                return MaterialPtr((Material*)result);

        } else if (type == "textured_deferred") {
                TexturedDeferredMaterial* result = new TexturedDeferredMaterial();
                result->loadFromXML(materialNode);
                return MaterialPtr((Material*)result);
        } else if (type == "emissive") {
                EmissiveMaterial* result = new EmissiveMaterial();
                result->loadFromXML(materialNode);
                return MaterialPtr((Material*)result);
        } else {
                IM_ERROR("Material " << type << " is not found");
                return MaterialPtr();
        }
}


} //namespace imEngine
