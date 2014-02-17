#include "Polygonal.h"
#include "../ObjectManager.h"

namespace imEngine {

Polygonal::Polygonal(const String &model, Object *parent) :
        Movable(parent),
        m_model(this)
{
        OBJECTS->registerPolygonal(this);
        m_model.loadFromXML(model);
}



} //namespace imEngine
