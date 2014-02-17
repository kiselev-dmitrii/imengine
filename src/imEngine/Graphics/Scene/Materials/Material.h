#ifndef MATERIAL_H
#define MATERIAL_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/FileContainers/Xml.h>

namespace imEngine {


/** @brief Базовый класс для материалов
 *
 * Каждый материал имеет соответствующий шейдер.
 */
class Material {
public:
        /// Конструктор
        Material(const String& filename, bool isTransparent);

        /// Активирует/деактивирует материал
        virtual void    bind() = 0;
        virtual void    unbind() = 0;

        /// Возвращает программу
        Program*        program() const;

        /// Определяет, является ли материал прозрачным
        bool            isTransparent() const;

protected:
        Program*        m_program;
        bool            m_isTransparent;
};


/** @brief Умный указатель для Material
 */
typedef std::shared_ptr<Material> MaterialPtr;


} //namespace imEngine

#endif // MATERIAL_H
