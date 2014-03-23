#ifndef POSTEFFECT_H
#define POSTEFFECT_H

namespace imEngine {

/** @brief Базовый класс для постэффектов
 */
class PostEffect {
public:
        PostEffect() : m_enabled(true)                                          { }

        void            apply()                                                 { if (m_enabled) process(); }
        void            setEnabled(bool enabled)                                { m_enabled = enabled; }

protected:
        virtual void    process() = 0;

private:
        bool            m_enabled;
};

} //namespace imEngine

#endif // POSTEFFECT_H
