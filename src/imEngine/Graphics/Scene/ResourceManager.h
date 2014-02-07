#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>

#include <imEngine/Utils/Types.h>
#include <imEngine/Graphics/GAPI/Texture/Texture1D.h>
#include <imEngine/Graphics/GAPI/Texture/Texture2D.h>
#include <imEngine/Graphics/GAPI/Texture/Texture3D.h>
#include <imEngine/Graphics/GAPI/Texture/CubeTexture.h>
#include <imEngine/Graphics/Scene/Geometry.h>

namespace imEngine {


/** @brief Базовый шаблонный класс для менеджеров ресурсов
 */
class ResourceManagerAbstract {
public:
        /// Конструктор
        explicit ResourceManagerAbstract(const String& directory);
        /// Виртуальный деструктор
        virtual ~ResourceManagerAbstract()                                      {}

        /// Возвращает путь к директории с ресурсами
        const String&   directory() const;
        /// Возвращает список ресурсов в директории
        StringList      list() const;

        /// Перезагружает все ресурсы
        virtual void    reloadAll() = 0;
        /// Удаляет все ресурсы
        virtual void    removeAll() = 0;

private:
        String  m_directory;
};


/** @brief Общая шаблонная реализация менеджерова ресурсов
 *
 *  Позволяет избежать дублирования кода
 */
template <typename Resource>
class ResourceManagerImplementation : public ResourceManagerAbstract {
public:
        /// Конструктор
        ResourceManagerImplementation(const String& directory) : ResourceManagerAbstract(directory) {}

        /// Деструктор. Удаляет все ресурсы из памяти
        ~ResourceManagerImplementation() {
                removeAll();
        }

        /// Удаляет все ресурсы
        void removeAll() {
                for (auto& elem: m_resources) delete elem.second;
                m_resources.clear();
        }

protected:
        /// Ищет ресурс по имени
        Resource* findResource(const String& name) {
                auto it = m_resources.find(name);
                if (it == m_resources.end()) return nullptr;
                else return it->second;
        }

        /// Добавляет ресурс
        void addResource(const String& name, Resource* resource) {
                m_resources.insert(std::pair<String, Resource*>(name, resource));
        }

public:
        typedef std::map<String, Resource*>     MapStringResource;
        MapStringResource       m_resources;
};


/** @brief Менеджер текстур. Позволяет прозрачно работать с изображениями в ФС.
 */
class TextureManager : public ResourceManagerImplementation<Texture> {
public:
        /// Конструктор.
        TextureManager(const String& directory);

        /// Загружает или ищет текстуру в памяти
        Texture1D*      texture1D(const String& name);
        Texture2D*      texture2D(const String& name);
        Texture3D*      texture3D(const String& name);
        CubeTexture*    cubeTexture(const String& name);

        /// Перезагружает все текстуры
        virtual void    reloadAll();
};


/** @brief Менеджер геометрии
 */
class GeometryManager : public ResourceManagerImplementation<Geometry> {
public:
        ///
};

} //namespace imEngine

#endif // RESOURCEMANAGER_H
