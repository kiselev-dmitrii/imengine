#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>

#include <imEngine/Utils/Types.h>
#include <imEngine/Graphics/GAPI/Texture/Texture1D.h>
#include <imEngine/Graphics/GAPI/Texture/Texture2D.h>
#include <imEngine/Graphics/GAPI/Texture/Texture3D.h>
#include <imEngine/Graphics/GAPI/Texture/CubeTexture.h>
#include <imEngine/Graphics/GAPI/Shader/Program.h>

namespace imEngine {

class TextureManager;
class ProgramManager;

/** @brief Менеджер ресурсов. Необходим для предоставления общего досупа к ресурсам
 */
class ResourceManager {
public:
        ResourceManager();
        ~ResourceManager();

        const TextureManager&   textures();
        const ProgramManager&   programs();

private:
        TextureManager* m_textureMgr;
        ProgramManager* m_programMgr;
};


/** @brief Базовый шаблонный класс для менеджеров ресурсов
 */
class ResourceManagerAbstract {
public:
        /// Конструктор
        explicit ResourceManagerAbstract(const String& directory);
        /// Виртуальный деструктор
        virtual ~ResourceManagerAbstract()                                  {}

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


/** @brief Менеджер текстур.
 *
 *  Позволяет прозрачно работать с изображениями в ФС.
 */
class TextureManager : public ResourceManagerAbstract {
public:
        /// Конструктор.
        TextureManager(const String& directory);
        /// Деструктор. Удаляет все текстуры из памяти
        ~TextureManager();

        /// Загружает или ищет текстуру в памяти
        Texture1D*      texture1D(const String& name);
        Texture2D*      texture2D(const String& name);
        Texture3D*      texture3D(const String& name);
        CubeTexture*    cubeTexture(const String& name);

        /// Перезагружает все текстуры
        virtual void    reloadAll();
        /// Удаляет все текстуры
        virtual void    removeAll();

private:
        /// Ищет ресурс по имени
        Texture*        findResource(const String& name);
        /// Добавляет ресурс
        void            addResource(const String& name, Texture* texture);

private:
        typedef std::map<String, Texture*> MapStringTexture;
        MapStringTexture        m_resources;
};


/** @brief Менеджер шейдеров.
 *
 *  Позволяет держать в памяти один экземпляр скомпилированных шейдеров.
 */
class ProgramManager : public ResourceManagerAbstract {
public:
        /// Конструктор
        ProgramManager(const String& directory);
        /// Деструктор
        ~ProgramManager();

        /// Загружает или ищет шейдер в памяти с такими характеристиками
        Program*        program(const String& name, const StringList& defines);

        /// Перезагружает все шейдеры
        virtual void    reloadAll();
        /// Удаляет все шейдеры
        virtual void    removeAll();

private:
        typedef std::map<String, Program*> MapStringProgram;
        MapStringProgram        m_resources;
};

} //namespace imEngine

#endif // RESOURCEMANAGER_H
