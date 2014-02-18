#include "Polygonal.h"
#include "../../Scene.h"

namespace imEngine {

Polygonal::Polygonal(const String &model, Object *parent) :
        Movable(parent),
        m_model(this)
{
        scene()->registerPolygonal(this);
        m_model.loadFromXML(model);
}



} //namespace imEngine
