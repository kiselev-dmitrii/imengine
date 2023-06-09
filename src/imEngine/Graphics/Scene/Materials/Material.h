#ifndef MATERIAL_H
#define MATERIAL_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <json/json.h>
#include <imEngine/FileContainers/Json.h>
#include <imEngine/FileContainers/Xml.h>
#include <imEngine/Graphics/Scene/Objects/Camera/Camera.h>

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

        /// Загружуает настройки материала из json-ноды
        virtual void    loadFromJson(const JsonValue& node)                     { }

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


/** @brief Материал для Entity объектов
 */
class EntityMaterial : public Material {
public:
        /// Конструктор
        EntityMaterial(const String& filename, MaterialType type) : Material(filename, type) { }
};


/** @brief Материал для Volume объектов
 */
class VolumeMaterial : public Material {
public:
        /// Конструктор
        VolumeMaterial(const String& filename);

        /// Загружает настройки материала из json-ноды
        void            loadFromJson(const JsonValue& node);

        /// Обязательные параметры
        /// Установка/возвращение данных в виде трехмерной текстуры
        void            setData(const String& name, const IVec3& size, InternalFormat::Enum internal);
        void            setData(Texture3D* data);
        Texture3D*      data() const;
        /// Установка активной камеры
        void            setActiveCamera(Camera* camera)                         { m_camera = camera; }
        /// Установка рисуемого объекта
        void            setObject(Object* object)                               { m_object = object; }

        /// Необязательные параметры
        /// Шаг трассировки в текселях
        void            setStep(int step)                                       { m_step = glm::clamp(step, 1, 100); }
        int             step() const                                            { return m_step; }
        /// Устанавливает текстуру, переводящую плотность от [0;1] в цвет
        void            setDensityTexture(const String& path);
        void            setDensityTexture(const Texture2DPtr& texture)          { m_densityTexture = texture; }
        Texture2DPtr    densityTexture() const                                  { return m_densityTexture; }


        /// Активирует/деактивирует материал
        void            bind();
        void            unbind();

protected:
        Texture3D*      m_texture;
        Camera*         m_camera;
        Object*         m_object;

        int             m_step;
        Texture2DPtr    m_densityTexture;
};


/** @brief Умный указатель для Material, EntityMaterial и VolumeMaterial
 */
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<EntityMaterial> EntityMaterialPtr;
typedef std::shared_ptr<VolumeMaterial> VolumeMaterialPtr;


} //namespace imEngine

#endif // MATERIAL_H
