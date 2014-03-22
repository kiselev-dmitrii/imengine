///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;	// in [-1;1]x[-1;1]x[0;0]
out vec2 vTexCoord;

void main() {
        gl_Position = vec4(aPosition, 0.0, 1.0);
	vTexCoord = 0.5*aPosition + vec2(0.5);
}


///### FRAGMENT SHADER ###///

#include "../common/functions.glsl"

in vec2 vTexCoord;
layout (location = 0) out vec4 fResult;

uniform sampler2D 	uTexture;
uniform float 		uThreshold;

void main() {
        vec4 value = texture2D(uTexture, vTexCoord);
        fResult = value * 
        		  clamp(luma(value.rgb) - uThreshold, 0.0, 1.0) *
        		  (1.0 / (1.0 - uThreshold));
}