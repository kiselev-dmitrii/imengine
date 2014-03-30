#ifndef POSTEFFECT_H
#define POSTEFFECT_H

#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {

/** @brief Базовый класс для постэффектов
 */
class PostEffect {
public:
        PostEffect() : m_isEnabled(true)                                        { }

        void   setEnabled(bool enabled)                                         { m_isEnabled = enabled; }
        bool   isEnabled() const                                                { return m_isEnabled; }

        virtual Texture2D*      apply() = 0;

private:
        bool    m_isEnabled;
};

} //namespace imEngine

#endif // POSTEFFECT_H
