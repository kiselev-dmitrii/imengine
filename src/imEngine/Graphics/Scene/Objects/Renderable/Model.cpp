#include "Model.h"
#include "../../ResourceManager.h"
#include "../../Materials/BasicDeferredMaterial.h"
#include "../../Materials/DeferredMaterial.h"
#include "../../Materials/EmissiveMaterial.h"
#include <imEngine/Utils/StringUtils.h>
#include <imEngine/Utils/Debug.h>

namespace imEngine {


Model::Model(Object* owner) :
        m_owner(owner)
{ }

Model::Model(const ModelDetailList &details, Object *owner) :
        m_details(details),
        m_owner(owner)
{ }

Model::Model(const String &geometry, MaterialPtr material, Object* owner) :
        m_owner(owner)
{
        ModelDetail detail;
        detail.geometry = RESOURCES->geometry()->geometry(geometry);
        detail.material = material;
        detail.owner = this;

        m_details.push_back(detail);
}

ModelDetailList& Model::details() {
        return m_details;
}

Object* Model::owner() const {
        return m_owner;
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
                ModelDetail detail;
                detail.geometry = createGeometry(detailNode.child("geometry"));
                detail.material = createMaterial(detailNode.child("material"));
                detail.owner = this;

                m_details.push_back(detail);
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
