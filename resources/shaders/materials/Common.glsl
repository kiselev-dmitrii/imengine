/** Общий вершинный шейдер для всех материалов.
    Так как материалы применяются перед визуализацией геометрии, то на вход подаются аттрибуты вершины
*/
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;

out vec2 vTexcoords;

uniform mat4 uModelViewProjectionMatrix;

void main() {
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
    vTexcoords = aTexcoords;
}