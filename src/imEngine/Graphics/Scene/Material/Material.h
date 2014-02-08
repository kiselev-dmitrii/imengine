#ifndef MATERIAL_H
#define MATERIAL_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/FileContainers/Xml.h>

namespace imEngine {


/** @brief Базовый класс для материалов
 */
class Material {
public:
        /// Привязывает/отвязывает материал к программе program
        virtual void            bind() = 0;
        virtual void            unbind() = 0;

        /// Возвращает программу
        virtual Program*        program() = 0;
};

/** @brief Умный указатель для Material
 */
typedef std::shared_ptr<Material> MaterialPtr;


/** @brief Вспомогательный шаблонный класс.
 *
 * Содержит общую реализацию всех материалов. Наследуемые классы ки этого класса
 * будут иметь общую GPU программу для всех экземпляров.
 * Таким образом, при копировании материала будет копироваться только информация
 * о самом материале - цвет, яркость и прочие детали.
 */
template <typename Derived>
class MaterialImplementation : public Material {
public:
        Program* program() {
                return s_program.get();
        }

protected:
        void initProgram(const String& path) {
                if (s_wasProgramInited) return;

                s_program = ProgramPtr(new Program());
                s_program->loadSourceFromFile(path);
                s_program->build();

                s_wasProgramInited = true;
        }

        static ProgramPtr       s_program;
        static bool             s_wasProgramInited;
};
template<typename Derived> ProgramPtr MaterialImplementation<Derived>::s_program;
template<typename Derived> bool MaterialImplementation<Derived>::s_wasProgramInited = false;


} //namespace imEngine

#endif // MATERIAL_H
