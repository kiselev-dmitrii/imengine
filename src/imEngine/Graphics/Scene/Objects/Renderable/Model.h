#ifndef MODEL_H
#define MODEL_H

#include <imEngine/Graphics/Geometry.h>
#include <imEngine/Graphics/Scene/Materials/Material.h>
#include <imEngine/FileContainers/Json.h>

namespace imEngine {
class Model;
class ModelObject;


/** @brief Объединение геометрии и метода отрисовки
 */
class Detail {
public:
        /// Конструктор
        Detail(const String& name, GeometryPtr geometry, MaterialPtr material, Model* owner);

        /// Возвращает имя детали
        const String&           name() const                                    { return m_name; }
        /// Устанавливает/возвращает материал
        void                    setMaterial(const MaterialPtr& material)        { m_material = material; }
        const MaterialPtr&      material() const                                { return m_material; }
        /// Возвращает геометрию
        GeometryPtr             geometry() const                                { return m_geometry; }
        /// Устанавливает/возвращает владующую модель
        void                    setOwner(Model* owner)                          { m_owner = owner; }
        Model*                  owner()                                         { return m_owner; }

        /// Рендерит деталь
        void                    render(const Mat4& modelMatrix, const Mat4& viewMatrix,
                                       const Mat4& projectionMatrix) const;
private:
        String          m_name;
        Model*          m_owner;

        GeometryPtr     m_geometry;
        MaterialPtr     m_material;
};
typedef std::vector<Detail> MapStringDetail;


/** @brief Класс модели
 *
 * Модель является ресурсом. При копировнии модели - копируется список деталей.
 * Однако, геометрия и материал не копируются, т.к они являются shared_ptr
 */
class Model {
public:
        /// Конструирует модель с именем name (без деталей)
        Model(const String& name);
        /// Конструктор копирования
        Model(const Model& model);

        /// Загружает модель из json файла, содержащего геометрию и материалы
        void                    load(const String& filename);

        /// Возвращает список деталей модели
        MapStringDetail&        details()                                       { return m_details; }
        /// Возвращает деталь с именем name
        Detail*                 detail(const String& name);

        /// Возвращает имя модели
        const String&           name() const                                    { return m_name; }
        /// Возвращает aabb модели
        const AABB&             aabb() const                                    { return m_aabb; }

        /// Устанавливает/возвращает владельца модели
        void                    setOwner(Object* owner)                         { m_owner = owner; }
        Object*                 owner() const                                   { return m_owner; }

public:
        static GeometryPtr      decodeGeometry(const String& encodedVertices, const String& encodedIndices);
        static MaterialPtr      createMaterial(const JsonValue& node);

private:
        void                    loadFromJson(const String& filename);
        void                    recalculateAABB();

private:
        String          m_name;
        AABB            m_aabb;
        MapStringDetail m_details;
        Object*         m_owner;
};


} //namespace imEngine

#endif // MODEL_H
