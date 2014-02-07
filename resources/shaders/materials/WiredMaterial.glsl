///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;

out vec2 vTexcoords;

uniform mat4 uModelViewProjectionMatrix;

void main() {
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
    vTexcoords = aTexcoords;
}


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
