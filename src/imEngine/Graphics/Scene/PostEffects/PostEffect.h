#ifndef POSTEFFECT_H
#define POSTEFFECT_H

#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {

/** @brief Базовый класс для постэффектов
 */
class PostEffect {
public:
        PostEffect() : m_enabled(true)                                          { }

        void   setEnabled(bool enabled)                                         { m_enabled = enabled; }
        bool   isEnabled() const                                                { return m_enabled; }

        virtual Texture2D*      apply() = 0;

private:
        bool    m_enabled;
};

} //namespace imEngine

#endif // POSTEFFECT_H
