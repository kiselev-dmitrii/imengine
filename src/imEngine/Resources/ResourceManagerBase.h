#ifndef RESOURCEMANAGERBASE_H
#define RESOURCEMANAGERBASE_H

#include <imEngine/Utils/Types.h>
#include <map>

namespace imEngine {

class ResourceBase;
typedef std::map<String, ResourceBase*> ResourceList;

/** @brief Базовый класс, от которого наследуются все менеджеры ресурсов
 */
class ResourceManagerBase {
        friend class ResourceBase;
public:
        /// Конструктор. Создает менеджер ресурсов, привязанного к директории directory
        ResourceManagerBase(const String& directory);
        /// Деструктор. При уничтожении освобождает все ресурсы.
        ~ResourceManagerBase();

        /// Возвращает путь к директории с ресурсами
        const String&   directory();
        /// Возвращает список доступных ресурсов (и загруженных и тех, которые на диске)
        StringList      list();

        /// Перезагружает все ресурсы
        void            reloadAll();
        /// Удаляет все ресурсы
        void            removeAll();

protected:
        /// Добавляет ресурс
        void            addResource(ResourceBase* resource);
        /// Ищет ресурс по имени
        ResourceBase*   findResource(const String& name);
        /// Удаляет ресурс по имени
        void            removeResource(const String& name);

private:
        ResourceList    m_resources;
        String          m_directory;

};


/** @brief Базовый класс ресурса
 */
class ResourceBase {
public:
        /// Конструктор
        explicit                ResourceBase(const String& directory = "", const String& name = "", ResourceManagerBase* manager = nullptr);
        /// Деструктор
        virtual                 ~ResourceBase();

        /// Возвращает имя ресурса
        const String&           name();
        /// Устанавилвает имя ресурса
        void                    setName(const String& name);
        /// Возвращает директорию
        const String&           directory();
        /// Устанавливает новый путь к ресурсу
        void                    setDirectory(const String& directory);
        /// Возвращает полное имя ресурса включая путь
        String                  filename();

        /// Возвращает менеджера управляющего этим ресурсом
        ResourceManagerBase*    manager();

        /// Производит загрузку ресурса по имени
        virtual void            load() = 0;
        /// Производит перезагрузку ресурса
        virtual void            reload() = 0;

protected:
        String                  m_directory;
        String                  m_name;
        ResourceManagerBase*    m_manager;
};


} //namespace imEngine

#endif // RESOURCEMANAGERBASE_H
