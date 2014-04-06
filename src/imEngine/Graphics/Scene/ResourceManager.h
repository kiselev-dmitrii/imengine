#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>

#include <imEngine/Utils/Types.h>
#include <imEngine/Graphics/GAPI/Texture/Texture1D.h>
#include <imEngine/Graphics/GAPI/Texture/Texture2D.h>
#include <imEngine/Graphics/GAPI/Texture/Texture3D.h>
#include <imEngine/Graphics/GAPI/Texture/CubeTexture.h>
#include <imEngine/Graphics/GAPI/Shader/Program.h>
#include <imEngine/Graphics/Geometry.h>
#include <imEngine/Graphics/Scene/Objects/Renderable/Model.h>

namespace imEngine {

class TextureManager;
class GeometryManager;
class ProgramManager;
class ModelManager;
#define RESOURCES ResourceManager::instance()


/** @brief Менеджер разделяемых ресурсов
 */
class ResourceManager {
public:
        /// Точка доступа к синглтону
        static ResourceManager* instance();
        /// Деструктор
        ~ResourceManager();

        /// Возвращает указатели на менеджеры
        TextureManager*         textures();
        GeometryManager*        geometry();
        ProgramManager*         programs();
        ModelManager*           models();

private:
        ResourceManager();
        ResourceManager(const ResourceManager&);
        ResourceManager& operator=(const ResourceManager&);

private:
        TextureManager*         m_textureMgr;
        GeometryManager*        m_geometryMgr;
        ProgramManager*         m_programMgr;
        ModelManager*           m_modelMgr;
};


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
        Texture3D*      texture3D(const String& name, const IVec3& size, InternalFormat::Enum internal);
        CubeTexture*    cubeTexture(const String& name);

        /// Перезагружает все текстуры
        virtual void    reloadAll();
};


/** @brief Менеджер геометрии. Предоставляет общий доступ к одной и той же геометрии
 *
 * Данный менеджер реализован отдельно, поскольку несколько программ могут иметь
 * одинаковые имена, но разные defines
 */
class GeometryManager : public ResourceManagerImplementation<Geometry> {
public:
        /// Конструктор
        GeometryManager(const String& directory);

        /// Загружает или ищет геометрию в памяти
        Geometry*       geometry(const String& name);

        /// Перезагружает всю геометрию
        virtual void    reloadAll();
};


/** @brief Менеджер GPU програм.
 */
class ProgramManager : public ResourceManagerAbstract {
public:
        /// Конструктор
        ProgramManager(const String& directory);
        /// Деструктор. Удаляет все шейдеры
        ~ProgramManager();

        /// Загружает или ищет шейдер
        Program*        program(const String& name, const StringList& defines);
        Program*        program(const String& name);

        /// Перезагружает и перекомпилирует все шейдеры
        void            reloadAll();
        /// Удаляет все шейдеры
        void            removeAll();

private:
        /// Ищет программу по имени и дефайнам
        Program*        findResource(const String& name, const StringList& defines);
        /// Добавляет ресурс
        void            addResource(const String& name, Program* resource);

private:
        typedef std::multimap<String, Program*> MultiMapStringResource;
        MultiMapStringResource  m_resources;
};


/** @brief Менеджер моделей
 */
class ModelManager : public ResourceManagerImplementation<Model> {
public:
        /// Конструктор
        ModelManager(const String& directory);

        /// Загружает или ищет геометрию в памяти
        Model*          model(const String& name);
        /// Перезагружает всю геометрию
        virtual void    reloadAll();
};

} //namespace imEngine

#endif // RESOURCEMANAGER_H
