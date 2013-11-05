///### VERTEX SHADER ###///
in vec3 im_vPosition;

void main() {
        gl_Position.xyz = im_vPosition;
        gl_Position.w = 1.0;
}


///### FRAGMENT SHADER ###///
#include "testInclude.glsl"
layout (location = 0) out vec4 im_outColor;
uniform vec3 uColor;

void main() {
        im_outColor = generateColor(uColor);
}