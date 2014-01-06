#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Utils/Types.h>
#include <imEngine/FileContainers/Image.h>

namespace imEngine {

class Widget;

/** @brief Структура хранящая положение и размер изображения в текстурном атласе
 */
struct ImageGeometry {
        Vec2    size;                   // Размер изображения в пикселях
        Vec4    texCoords;              // Текстурные координаты в OGL системе
};
typedef std::map<String, ImageGeometry> MapImageGeometry;


/** @brief Текстурный менеджер, хранящий текстурный атлас с виджетами
 */
class GuiManager {
public:
        /// Конструктор, создает текстурный атлас из картинок в директории themePath
        GuiManager(const String& themePath);
        /// Деструктор. Уничтожает всю иерархию подключенных виджетов
        ~GuiManager();

        /// Устанавилвает новую тему. Это приводит к пересозданию текстуры
        void                    setTheme(const String& themePath);
        /// Возвращает директорию с темой
        String                  themePath();

        /// Возвращает сгенерированный текстурный атлас
        Texture2DPtr            textureAtlas() const;

        /// Добавляет виджет к менеджеру
        void                    attachWidget(Widget* widget);
        /// Удаляет виджет из менеджера
        void                    detachWidget(Widget* widget);

        /// Возвращает текстурные координаты по имени картинки в директории с темой
        ImageGeometry*          imageGeometry(const String& name);

        /// Рендерит все виджеты
        void                    render();

private:
        typedef std::vector<ImagePtr> ImageList;

private:
        /// Определяет размеры текструного атласа
        IVec2                   calcSizeOfTextureAtlas(const ImageList& images) const;
        /// Генерирует текстуру.
        Texture2DPtr            generateTextureAtlas(const ImageList& images) const;
        /// Заполняет структуру out текстурными координатами и размером текстур
        void                    fillImagesGeometry(const ImageList& images, const StringList& files, Texture2DPtr texture, MapImageGeometry* out) const;
        /// Создает и собирает программу
        ProgramPtr              createProgram() const;

private:
        String                  m_themePath;
        Texture2DPtr            m_texture;              // текстура со всеми виджетами
        MapImageGeometry        m_imagesGeometry;       // имя виджета -> соответствующие текст. координаты

        Widget*                 m_rootWidget;                 // единственный корневой виджет
        ProgramPtr              m_program;              // программа, которой будут рендерится все виджеты
};


} //namespace imEngine

#endif // GUIMANAGER_H
