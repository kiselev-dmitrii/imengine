///### VERTEX SHADER ###///
#include "Common.glsl"

///### FRAGMENT SHADER ###///
in vec2 vTexcoords;

layout (location = 0) out vec4 fResult;

uniform sampler2D uTextureBase;


void main() {
	fResult = texture2D(uTextureBase, vTexcoords);
}
