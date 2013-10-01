#ifndef PHYSICAPPLICATION_H
#define PHYSICAPPLICATION_H

#include "GraphicApplication.h"

namespace imFramework {

/** @brief Приложение, в котором будут использоваться возможности физического движка.
 *
 *  Предполагается, что классы приложений ниже в иерархии будут использовать
 *  встроенный в imEngine движок Bullet Physics.
 */
class PhysicApplication : public GraphicApplication {
protected:
        void            initialize();
        void            update();
        void            render();
        void            destroy();

private:
        /*
        imCore::PhysicWorld     m_physicWorld
        */
};

} //namespace imFramework

#endif // PHYSICAPPLICATION_H
