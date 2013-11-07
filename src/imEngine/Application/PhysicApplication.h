#ifndef PHYSICAPPLICATION_H
#define PHYSICAPPLICATION_H

#include "GraphicApplication.h"

namespace imEngine {

/** @brief Приложение, в котором будут использоваться возможности физического движка.
 *
 *  Предполагается, что классы приложений ниже в иерархии будут использовать
 *  встроенный в imEngine движок Bullet Physics.
 *  @todo
 *      - Реализовать
 */
class PhysicApplication : public GraphicApplication {
protected:
        void            initialize();
        void            update();
        void            render();
        void            destroy();

private:
        /*
        PhysicWorld     m_physicWorld
        */
};

} //namespace imEngine

#endif // PHYSICAPPLICATION_H
