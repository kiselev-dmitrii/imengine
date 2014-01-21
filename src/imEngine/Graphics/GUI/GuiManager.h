#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Utils/Types.h>
#include <imEngine/FileContainers/Image.h>
#include <imEngine/System/Window.h>

namespace imEngine {

class WidgetAbstract;

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
        GuiManager(const String& themePath, Window* window);
        /// Деструктор. Уничтожает всю иерархию подключенных виджетов
        ~GuiManager();

        /// Устанавилвает новую тему. Это приводит к пересозданию текстуры
        void                    setTheme(const String& themePath);
        /// Возвращает директорию с темой
        String                  themePath();

        /// Устанавливает/возвращает окно, в котором происходит рендер
        void                    setWindow(Window* window);
        Window*                 window() const;

        /// Возвращает сгенерированный текстурный атлас
        Texture2DPtr            textureAtlas() const;
        /// Возвращает программу, с помощью которой рендерится весь GUI
        ProgramPtr              program() const;

        /// Возвращает текстурные координаты по имени картинки в директории с темой
        ImageGeometry*          imageGeometry(const String& name);
        /// Возвращает список доступных изображений
        StringList              imageList() const;

        /// Возвращает корневой элемент менеджера
        WidgetAbstract*         root() const;

        /// Метод должен вызываться при рендере
        void                    processRender();
        /// Метод должен вызываться при изменении позиции мыши
        void                    processMouseMove(int oldX, int oldY, int newX, int newY);
        /// Метод должен вызываться при клике мыши
        void                    processMousePress(int x, int y, char button);
        /// Метод должен вызываться при отжатии клавиши мыши
        void                    processMouseRelease(int x, int y, char button);

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
        Window*                 m_window;
        Texture2DPtr            m_texture;              // текстура со всеми виджетами
        MapImageGeometry        m_imagesGeometry;       // имя виджета -> соответствующие текст. координаты

        WidgetAbstract*         m_root;                 // единственный корневой виджет
        ProgramPtr              m_program;              // программа, которой будут рендерится все виджеты
};


} //namespace imEngine

#endif // GUIMANAGER_H
