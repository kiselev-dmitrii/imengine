#ifndef TEXTURES_H
#define TEXTURES_H

#include "ResourceManagerBase.h"
#include <imEngine/Graphics/GAPI/Texture/Texture1D.h>
#include <imEngine/Graphics/GAPI/Texture/Texture2D.h>
#include <imEngine/Graphics/GAPI/Texture/Texture3D.h>

namespace imEngine {

/** @brief Ресурс одномерной текстуры
 */
class Texture1D : public ResourceBase {
public:
        /// Конструктор
        Texture1D(const String& directory = "", const String& name = "", ResourceManagerBase* manager = nullptr);
        /// Деструктор
        ~Texture1D();

        /// Загружает текстуру по имени ресурса
        virtual void            load();
        /// Перезагружает текстуру
        virtual void            reload();

        /// Возвращает указатель непосредственно на текстуру
        GAPI::Texture1D*        raw()                   { return &m_texture; }

private:
        GAPI::Texture1D         m_texture;

};

/** @brief Ресрус двумерной текстуры
 */
class Texture2D : public ResourceBase {
public:
        /// Конструктор
        Texture2D(const String& directory = "", const String& name = "", ResourceManagerBase* manager = nullptr);
        /// Деструктор
        ~Texture2D();

        /// Загружает текстуру по имени ресурса
        virtual void            load();
        /// Перезагружает текстуру
        virtual void            reload();

        /// Возвращает указатель непосредственно на текстуру
        GAPI::Texture2D*        raw()                   { return &m_texture; }

private:
        GAPI::Texture2D         m_texture;
};

} //namespace imEngine

#endif // TEXTURES_H
