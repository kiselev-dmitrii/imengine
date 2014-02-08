#include "Model.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/StringUtils.h>
#include "Resources.h"
#include "Material/TextureMaterial.h"
#include "Material/WiredMaterial.h"
#include "Material/PhongMaterial.h"

namespace imEngine {

Model::Model(const String &filename) {
        load(filename);
}

void Model::load(const String &filename) {
        XmlDocument doc;
        XmlResult result = doc.load_file(filename.c_str());
        if (!result) {
                IM_ERROR("Cannot open file " << filename);
                return;
        }

        load(doc.first_child());
}

void Model::load(const XmlNode &modelNode) {
        for (const XmlNode& detailNode: modelNode.children()) {
                ModelDetail detail;
                detail.geometry = createGeometry(detailNode.child("geometry"));
                detail.material = createMaterial(detailNode.child("material"));

                m_details.push_back(detail);
        }
}

const ModelDetailList& Model::details() const {
        return m_details;
}

Geometry* Model::createGeometry(const XmlNode &geometryNode) {
        IM_VAR(geometryNode.name());
        String filename = geometryNode.attribute("filename").value();
        if (filename == "") {
                IM_ERROR("Tag geometry must contain attribute filename");
                return nullptr;
        }

        return Resources::instance()->geometry()->geometry(filename);
}

MaterialPtr Model::createMaterial(const XmlNode &materialNode) {
        String type = materialNode.attribute("type").value();
        if (type == "") {
                IM_ERROR("Tag material must contain attribute type");
                return MaterialPtr();
        }

        if (type == "texture") {
                String textureFilename = materialNode.attribute("texture").value();
                if (textureFilename == "") {
                        IM_ERROR("TextureMaterial must contain attribute texture");
                        return MaterialPtr();
                }
                Texture2D* texture = Resources::instance()->textures()->texture2D(textureFilename);
                return MaterialPtr(new TextureMaterial(texture));

        } else if (type == "wired") {
                WiredMaterial* result = new WiredMaterial();

                String borderColor = materialNode.attribute("border_color").value();
                String baseColor = materialNode.attribute("base_color").value();
                String borderWidth = materialNode.attribute("border_width").value();
                String scale = materialNode.attribute("scale").value();
                if (borderColor != "") result->setBorderColor(parseColor(borderColor));
                if (baseColor != "") result->setBaseColor(parseColor(baseColor));
                if (borderWidth != "") result->setBorderWidth(std::stof(borderWidth));
                if (scale != "") result->setScale(std::stof(scale));

                return MaterialPtr(result);

        } else if (type == "phong") {
                PhongMaterial* result = new PhongMaterial();

                String ambient = materialNode.attribute("ambient").value();
                String diffuse = materialNode.attribute("diffuse").value();
                String specular = materialNode.attribute("specular").value();
                String shininess = materialNode.attribute("shininess").value();
                if (ambient != "") result->setAmbientColor(Vec3(parseColor(ambient)));
                if (diffuse != "") result->setDiffuseColor(Vec3(parseColor(diffuse)));
                if (specular != "") result->setSpecularColor(Vec3(parseColor(specular)));
                if (shininess != "") result->setShininess(std::stof(shininess));

                return MaterialPtr((Material*)result);

        } else {
                IM_ERROR("Material has unknown type");
                return MaterialPtr();
        }
}

Vec4 Model::parseColor(const String &color) {
        /// Цвет задан в формате (255,255,255,255)
        Vec4 result;
        StringList list = StringUtils::split(color, ",");
        result.x = std::stof(list[0])/255.0f;
        result.y = std::stof(list[1])/255.0f;
        result.z = std::stof(list[2])/255.0f;
        if (list.size() == 4) result.w = std::stof(list[3])/255.0f;

        return result;
}

} //namespace imEngine
