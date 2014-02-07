#ifndef MATERIAL_H
#define MATERIAL_H

#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {

/** @brief Базовый класс для материалов
 */
class Material {
public:
        /// Привязывает/отвязывает материал к программе program
        virtual void            bind() = 0;
        virtual void            unbind() = 0;

        /// Возвращает программу
        virtual Program*        program() = 0;
};

/** @brief Умный указатель для Material
 */
typedef std::shared_ptr<Material> MaterialPtr;


/** @brief Вспомогательный шаблонный класс.
 *
 * Таким образом мы определяем, что каждый дочерний класс будет иметь свою
 * статическую программу s_program
 */
template <typename Derived>
class MaterialImplementation : public Material {
public:
        Program* program() {
                return s_program.get();
        }

protected:
        void initProgram(const String& path) {
                if (s_wasProgramInited) return;

                s_program = ProgramPtr(new Program());
                s_program->loadSourceFromFile(path);
                s_program->build();

                s_wasProgramInited = true;
        }

        static ProgramPtr       s_program;
        static bool             s_wasProgramInited;
};
template<typename Derived> ProgramPtr MaterialImplementation<Derived>::s_program;
template<typename Derived> bool MaterialImplementation<Derived>::s_wasProgramInited = false;


/** @brief Материал с простой текстурой
 */
class TextureMaterial : public MaterialImplementation<TextureMaterial> {
public:
        /// Конструктор.
        TextureMaterial(Texture2DPtr texture);

        /// Устанавливает/возвращает текстуру
        void            setTexture(Texture2DPtr texture);
        Texture2DPtr    texture() const;

        /// Перегруженные методы
        void            bind();
        void            unbind();

private:
        Texture2DPtr            m_textureBase;
};


/** @brief Сетка.
 */
class WiredMaterial : public MaterialImplementation<WiredMaterial> {
public:
        /// Конструктор
        WiredMaterial();

        /// Устанавливает/возвращает размер границы сетки
        void            setBorderColor(const Vec4& color)                       { m_borderColor = color; }
        const Vec4&     borderColor() const                                     { return m_borderColor; }

        /// Устанавливает/возвращает основной цвет
        void            setBaseColor(const Vec4& color)                         { m_baseColor = color; }
        const Vec4&     baseColor() const                                       { return m_baseColor; }

        /// Устанавливает/возвращает ширину границы
        void            setBorderWidth(float width)                             { m_borderWidth = width; }
        float           borderWidth()                                           { return m_borderWidth; }

        /// Устанавливает/возвращает масштаб сетки
        void            setScale(float scale)                                   { m_scale = scale; }
        float           scale() const                                           { return m_scale; }

        ///Перегруженные методы
        void            bind();
        void            unbind();

private:
        Vec4                    m_borderColor;
        Vec4                    m_baseColor;
        float                   m_borderWidth;
        float                   m_scale;
};

} //namespace imEngine

#endif // MATERIAL_H
