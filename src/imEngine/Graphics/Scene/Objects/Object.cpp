#include "Object.h"
#include <glm/gtx/quaternion.hpp>
#include "../Scene.h"
#include "../ResourceManager.h"

#include "Renderable/Entity.h"
#include "Renderable/Volume.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Camera/FirstPersonCamera.h"
#include "../Materials/RaycastingMaterial.h"
#include "../Materials/IsosurfaceMaterial.h"

namespace imEngine {


//############################ ObjectType ####################################//

String ObjectType::toString(ObjectType::Enum type) {
        switch (type) {
                case OBJECT: return "OBJECT";
                case MOVABLE: return "MOVABLE";
                case LIGHT: return "LIGHT";
                case CAMERA: return "CAMERA";

                case FIRST_PERSON_CAMERA: return "FIRST_PERSON_CAMERA";
                case TARGET_CAMERA: return "TARGET_CAMERA";
                case ENTITY: return "ENTITY";
                case VOLUME: return "VOLUME";
                case SPOT_LIGHT: return "SPOT_LIGHT";
                case POINT_LIGHT: return "POINT_LIGHT";

                default:
                        IM_ERROR("Unknown ObjectType");
                        return "";
        }
}

//############################ Object ########################################//

Object::Object(Object *parent) :
        TreeNamedNode(parent),
        m_scene(nullptr),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        m_type = ObjectType::OBJECT;

        if (parent != nullptr) m_scene = parent->scene();
        notifyTransformUpdated();
}

void Object::loadFromJson(const JsonValue &node) {
        JsonValue name = node["name"];
        if (!name.isNull()) setName(name.asString());

        JsonValue children = node["children"];
        for (JsonValue child : children) {
                Object* object = createObject(child);
                object->loadFromJson(child);
        }
}

void Object::saveAsJson(JsonValue& result) {
        JsonValue childrenValues(Json::arrayValue);
        for (Object* child: children()) {
                JsonValue value(Json::objectValue);
                child->saveAsJson(value);
                childrenValues.append(value);
        }

        result["name"] = name();
        result["type"] = ObjectType::toString(type());
        result["children"] = childrenValues;
}

Scene* Object::scene() const {
        return m_scene;
}

const AABB& Object::aabb() const {
        return m_aabb;
}

ObjectType::Enum Object::type() const {
        return m_type;
}

const Vec3& Object::position() const {
        return m_psTransform.position;
}

const Quat& Object::orientation() const {
        return m_psTransform.orientation;
}

const Vec3& Object::scale() const {
        return m_psTransform.scale;
}

const Vec3& Object::worldPosition() const {
        updateWorldTransform();
        return m_wsTransform.position;
}

const Quat& Object::worldOrientation() const {
        updateWorldTransform();
        return m_wsTransform.orientation;
}

const Vec3& Object::worldScale() const {
        updateWorldTransform();
        return m_wsTransform.scale;
}

Vec3 Object::convertWorldToLocal(const Vec3 &wsVec) const {
        return glm::inverse(worldOrientation())*(wsVec - worldPosition()) / worldScale();
}

Quat Object::convertWorldToLocal(const Quat &wsQuat) const {
        return glm::inverse(worldOrientation()) * wsQuat;
}

Vec3 Object::convertWorldToParent(const Vec3 &wsVec) const {
        if (m_parent) return m_parent->convertWorldToLocal(wsVec);
        else return wsVec;
}

Quat Object::convertWorldToParent(const Quat &wsQuat) const {
        if (m_parent) return m_parent->convertWorldToLocal(wsQuat);
        else return wsQuat;
}

Vec3 Object::convertLocalToWorld(const Vec3 &lsVec) const {
        return (worldOrientation() *  (lsVec * worldScale())) + worldPosition();
}

Quat Object::convertLocalToWorld(const Quat &lsQuat) const {
        return worldOrientation() * lsQuat;
}

const Mat4& Object::localToWorldMatrix() const {
        updateLocalToWorldMatrix();
        return m_localToWorldMatrix;
}

const Mat4& Object::worldToLocalMatrix() const {
        updateWorldToLocalMatrix();
        return m_worldToLocalMatrix;
}

void Object::updateWorldTransform() const {
        if (!m_isNeedToUpdateWorldTransform) return;

        Object* p = m_parent;
        if (p) {
                Vec3 wsParentPos = p->worldPosition();
                Quat wsParentOrient = p->worldOrientation();
                Vec3 wsParentScale = p->worldScale();

                m_wsTransform.position = wsParentOrient * (wsParentScale * m_psTransform.position) + wsParentPos;
                m_wsTransform.orientation = wsParentOrient * m_psTransform.orientation;
                m_wsTransform.scale = wsParentScale * m_psTransform.scale;
        } else {
                m_wsTransform = m_psTransform;
        }

        m_isNeedToUpdateWorldTransform = false;
}

void Object::updateLocalToWorldMatrix() const {
        if (!m_isNeedToUpdateLocalToWorldMatrix) return;

        Vec3 wsPosition = worldPosition();
        Quat wsOrientation = worldOrientation();
        Vec3 wsScale = worldScale();

        Mat3 m = glm::toMat3(wsOrientation);

        m_localToWorldMatrix = Mat4(
                wsScale.x*m[0][0], wsScale.y*m[0][1], wsScale.z*m[0][2], 0,
                wsScale.x*m[1][0], wsScale.y*m[1][1], wsScale.z*m[1][2], 0,
                wsScale.x*m[2][0], wsScale.y*m[2][1], wsScale.z*m[2][2], 0,
                wsPosition.x,      wsPosition.y,      wsPosition.z,      1  );

        m_isNeedToUpdateLocalToWorldMatrix = false;
}

void Object::updateWorldToLocalMatrix() const {
        if (!m_isNeedToUpdateWorldToLocalMatrix) return;

        updateLocalToWorldMatrix();
        m_worldToLocalMatrix = glm::inverse(m_localToWorldMatrix);

        m_isNeedToUpdateWorldToLocalMatrix = false;
}

void Object::notifyTransformUpdated() {
        m_isNeedToUpdateWorldTransform = true;
        m_isNeedToUpdateLocalToWorldMatrix = true;
        m_isNeedToUpdateWorldToLocalMatrix = true;

        for (Object* node: children()) node->notifyTransformUpdated();
}

Object* Object::createObject(const JsonValue &node) {
        String type = node["type"].asString();

        if (type == "entity") return createEntity(node);
        if (type == "volume") return createVolume(node);
        if (type == "point_light") return new PointLight(this);
        if (type == "spot_light") return new SpotLight(this);
        if (type == "first_person_camera") return new FirstPersonCamera(this);

        return nullptr;
}

Object* Object::createEntity(const JsonValue &node) {
        String model = node["model"].asString();
        return new Entity(model, this);
}

Object* Object::createVolume(const JsonValue &node) {
        String data = node["data"].asString();
        Vec3 size = JsonUtils::toVec3(node["size"]);
        InternalFormat::Enum internal = InternalFormat::fromString(node["format"].asString());
        String materialType = node["material"]["type"].asString();

        VolumeMaterialPtr material;
        if (materialType == "RAYCASTING") material.reset(new RaycastingMaterial());
        else if (materialType == "ISOSURFACE") material.reset(new IsosurfaceMaterial());
        else {
                IM_ERROR("Material not found");
                return nullptr;
        }
        material->loadFromJson(node["material"]);

        Texture3D* texture = RESOURCES->textures()->texture3D(data, IVec3(size), internal);
        return new Volume(texture, material, this);
}


} //namespace imEngine
