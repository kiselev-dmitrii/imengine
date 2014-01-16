#ifndef PRIMITIVES_H
#define PRIMITIVES_H

namespace imEngine {

/** @brief Удобный класс для представления 1D сегоментов
 */
class Segment {
public:
        /// Конструктор
        Segment(float start, float end) : m_start(start), m_end(end)            { }

        /// Конечные точки
        float   start() const                                                   { return m_start; }
        float   end() const                                                     { return m_end; }

        /// Определяет, попадает ли точка внутрь сегмента
        bool    includes(float point) const                                     { return point  >= m_start && point <= m_end; }
        /// Определяет, попадает ли сегмент в данный сегмент
        bool    includes(const Segment& segment) const                          { return includes(segment.start()) && includes(segment.end()); }

private:
        float   m_start;
        float   m_end;
};

} //namespace imEngine

#endif // PRIMITIVES_H
