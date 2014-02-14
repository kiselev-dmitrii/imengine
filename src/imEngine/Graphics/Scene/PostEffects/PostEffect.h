#ifndef POSTEFFECT_H
#define POSTEFFECT_H

#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {

/** @brief Абстрактный класс для пост-эффектов
 */
class PostEffect {
public:
        virtual void    bind() = 0;
        virtual void    unbind() = 0;
};


/** @brief Вспомогательный шаблонный класс
 *
 * От данного класса, должны наследоваться все реализации пост эффектов содержащие
 * собтвенный шейдер для рендеринга пост-эффекта.
 * Данный класс позволяет иметь один шейдер для всех экземпляров пост эффекта.
 * Класс имеет счетчик экземпляров. Шейдер создается, если счетчик >0 и удаляется если =0
 */
template <typename Derived>
class PostEffectImplementation : public PostEffect {
public:
        PostEffectImplementation(const String& pathToProgram) {
                if (s_numInstances == 0) {
                        s_program = new Program();
                        s_program->loadSourceFromFile(pathToProgram);
                        s_program->build();
                }
                ++s_numInstances;
        }

        ~PostEffectImplementation() {
                --s_numInstances;
                if (s_numInstances == 0) delete s_program;
        }

protected:
        static int              s_numInstances;
        static Program*         s_program;
};
template<typename Derived> int PostEffectImplementation<Derived>::s_numInstances = 0;
template<typename Derived> Program* PostEffectImplementation<Derived>::s_program;


} //namespace imEngine

#endif // POSTEFFECT_H
