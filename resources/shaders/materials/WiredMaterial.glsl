///### VERTEX SHADER ###///
#include "Common.glsl"

///### FRAGMENT SHADER ###///
in vec2 vTexcoords;

uniform float 	uBorderWidth;
uniform vec4 	uBorderColor;
uniform vec4 	uBaseColor;
uniform float 	uScale;

layout (location = 0) out vec4 fResult;


void main() {
	bool isBorder = any(greaterThan(abs(fract(vTexcoords * uScale) - vec2(0.5)) - vec2(0.5 - (uBorderWidth*uScale)/2.0), vec2(0.0f,0.0f)));

	if (isBorder) fResult = uBorderColor;
	else fResult = uBaseColor;
}
