#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ResourceManagerBase.h"
#include "Texture.h"

namespace imEngine {

class TextureManager;
class ProgramManager;

/** @brief Главный ресурс менеджер приложения
 *
 *  Содержит указатели на менеджеры ресурсов, шейдеров, моделей и т.д
 */
class ResourceManager {
public:
        ResourceManager();
        ~ResourceManager();

        TextureManager* textures();
        ProgramManager* programs();

private:
        TextureManager* m_textureMgr;
        ProgramManager* m_programMgr;
};


/** @brief Текстурный менеджер
 */
class TextureManager : public ResourceManagerBase {
public:
        /// Конструктор
        TextureManager(const String& directory);

        /// Возвращает одномерную текстуру
        Texture1D*      texture1D(const String& name);
        /// Возвращает двумерную текстуру
        Texture2D*      texture2D(const String& name);
};


} //namespace imEngine

#endif // RESOURCEMANAGER_H
