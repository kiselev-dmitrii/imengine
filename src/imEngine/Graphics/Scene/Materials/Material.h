#ifndef MATERIAL_H
#define MATERIAL_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/FileContainers/Xml.h>

namespace imEngine {

/** @brief Тип материала
 */
enum class MaterialType {
        DEFERRED,
        TRANSPARENT,
        UNLIGHTENED
};


/** @brief Базовый класс для материалов
 *
 * Каждый материал имеет соответствующий шейдер.
 */
class Material {
public:
        /// Конструктор
        Material(const String& filename, MaterialType type);

        /// Активирует/деактивирует материал
        virtual void    bind() = 0;
        virtual void    unbind() = 0;

        /// Возвращает программу
        Program*        program() const;
        /// Возвращает тип материала
        MaterialType    type() const;

protected:
        Program*        m_program;
        MaterialType    m_type;
};


/** @brief Умный указатель для Material
 */
typedef std::shared_ptr<Material> MaterialPtr;


} //namespace imEngine

#endif // MATERIAL_H
