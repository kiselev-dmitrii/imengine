#ifndef BUFFEROBJECT_H
#define BUFFEROBJECT_H

#include <GL/glew.h>

namespace imEngine {

/** @brief Точка привязки буффера
 */
namespace BufferTarget {
enum Enum {
        ARRAY                   = GL_ARRAY_BUFFER,              ///< VBO
        ELEMENT_ARRAY           = GL_ELEMENT_ARRAY_BUFFER,      ///< IBO
        TEXTURE                 = GL_TEXTURE_BUFFER,            ///< TBO
        UNIFORM                 = GL_UNIFORM_BUFFER             ///< UBO
};
}

/** @brief Вспомогательная информация, помогающая OpenGL
 *  эффективно обращаеться с данными буфера
 *
 *  Каждый Hint имеет вид FREQUENCY_USAGE:
 *  FREQYENCY отвечает за частоту использования и может принимать значения:
 *      - STATIC - данные установлены в буфер один раз и не изменяются
 *      - DYNAMIC - данные периодически изменяются
 *      - STREAM - данные изменяются каждый кадр
 *  USAGE определяет режим доступа к данным:
 *      - DRAW - данные записываеются в буфер, но не читаются из него
 *      - READ - данные читаются из буфера
 *      - COPY - используется при копировании из одного буфера в другой
 */
namespace BufferUsage {
enum Enum {
        STATIC_DRAW             = GL_STATIC_DRAW,
        STATIC_READ             = GL_STATIC_READ,
        STATIC_COPY             = GL_STATIC_COPY,

        DYNAMIC_DRAW            = GL_DYNAMIC_DRAW,
        DYNAMIC_READ            = GL_DYNAMIC_READ,
        DYNAMIC_COPY            = GL_DYNAMIC_COPY,

        STREAM_DRAW             = GL_STREAM_DRAW,
        STREAM_READ             = GL_STREAM_READ,
        STREAM_COPY             = GL_STREAM_COPY
};
}

/** @brief Определяет способ использования данных при маппинге
 */
namespace BufferAccess {
enum Enum {
        READ_ONLY               = GL_READ_ONLY,
        WRITE_ONLY              = GL_WRITE_ONLY,
        READ_WRITE              = GL_READ_WRITE
};
}


/** @brief Базовый класс для всех буферных объектов.
 *
 *  Предполагается, что OpenGL контекст уже создан.
 *  Представляет собой хранилище данных в видеопамяти.
 *  От BufferObject отнаследованы VertexBuffer, IndexBuffer, UniformBuffer
 */
class BufferObject {
public:
        /// Конструктор. Создает буфер и привязывает его
        BufferObject(BufferTarget::Enum target);
        /// Деструктор. Разрушает буфер
        virtual ~BufferObject();

        /// Загружает данные data размера size в буфер, usage определяет цель использования данных
        void                    load(const void* data, GLsizeiptr size, BufferUsage::Enum usage);
        /// Заменяет часть данных [offset, offset+size] на данные из newData
        void                    replace(GLsizeiptr offset, GLintptr size, const void* newData);

        /// Отображает данные буфера в оперативную память и возвращает указатель на нее.
        /// После этой операции можно лишь использовать метод unmap()
        void*                   map(BufferAccess::Enum access);
        /// Отображает часть данных буфера [offset, offset+size] в оперативную память.
        /// После выполнения этой операции можно использовать лишь метод unmap()
        void*                   mapRange(GLsizeiptr offset, GLintptr size, BufferAccess::Enum access);
        /// Завершает режим отображения данных буфера. Возвращает true, если операция успешна, false если данные повреждены.
        bool                    unmap();
        /// Определяет, находится ли буфер в режиме отображения данных в память
        bool                    isMapped();

        /// Привязывает буфер
        virtual void            bind() = 0;
        /// Отвязывыет буфер
        virtual void            unbind() = 0;

        /// Возвращает OGL дескриптор буфера
        GLuint                  handle();
        /// Возвращает точку привязки буфера
        BufferTarget::Enum      target();
        /// Возвращает размер загруженных данных в буфер
        GLsizeiptr              size();
        /// Возвращает цель использования буфера
        BufferUsage::Enum       usage();

protected:
        GLuint                  m_handle;
        BufferTarget::Enum      m_target;
        GLsizeiptr              m_size;         //размер буфера
        BufferUsage::Enum       m_usage;

        bool                    m_isMapped;

};

} //namespace imEngine

#endif // BUFFEROBJECT_H
