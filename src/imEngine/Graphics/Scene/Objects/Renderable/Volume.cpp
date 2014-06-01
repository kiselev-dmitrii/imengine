#include "Volume.h"
#include "../../Scene.h"
#include "../../ResourceManager.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

GeometryPtr Volume::s_cube;

Volume::Volume(Texture3D *data, const VolumeMaterialPtr &material, Object *parent) :
        Movable(parent),
        m_material(material)
{
        m_type = ObjectType::VOLUME;

        if (!s_cube) s_cube = Geometry::box(Vec3(-0.5), Vec3(0.5));
        m_aabb = s_cube->aabb();

        m_material->setData(data);
        m_material->setObject(this);

        scene()->registerVolume(this);
}

Volume::~Volume() {
        scene()->unregisterVolume(this);
}

void Volume::setMaterial(const VolumeMaterialPtr &material) {
        material->setObject(this);
        material->setData(m_material->data());
        m_material = material;
}

const VolumeMaterialPtr& Volume::material() {
        return m_material;
}

void Volume::render() {
        m_material->setActiveCamera(scene()->activeCamera());
        m_material->bind();
        s_cube->render();
}

} //namespace imEngine

