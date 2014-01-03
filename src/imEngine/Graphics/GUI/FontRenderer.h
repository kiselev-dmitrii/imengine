#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <imEngine/System/Window.h>
#include <imEngine/FileContainers/Font.h>
#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {

/** @brief Структура для передачи данных в VBO
 */
struct SymbolGeometry {
        Vec3    position;       // Позиция глифа в коориданатах окна
        Vec2    size;           // Размер глифа на экране в пикселях
        Vec4    texCoords;      // Текстурные координаты глифа в атласе
};

/** @brief Класс, предназначенный для вывода текста, заданным шрифтом
class FontRenderer {
public:
        /// Конструктор
        FontRenderer(const Font& font, Window* win);

        /// Устанавливает новый шрифт
        void    setFont(const Font& font);
        /// Устанавливает окно в котором происходит рендер
        void    setWindow(Window* win);
        /// Устанавилвает цвет отображаемого текста
        void    setColor(const Vec3& color);
        /// Рендерит текст text в окне win в экранных коориднатах pos и глубине depth
        void    renderText(const String& text, Vec2& pos, float depth) const;

private:
        /// Создает GPU программу для отображения текста
        void    createGpuProgram();
        /// Создает вершинный буфер
        void    createVBO(const Font& font);

        /// Генерирует данные для VBO, которые содержат геометрию текста в оконных координатах
        void    generateTextGeometry(const String& text, const Vec2& pos, float depth, const Font& font) const;

private:
        Window*         m_win;
        Vec3            m_color;

        Font            m_font;
        ProgramPtr      m_program;
        VertexBufferPtr m_vbo;
        VertexArray     m_vao;

};
 */

} //namespace imEngine

#endif // FONTRENDERER_H
