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

uniform sampler2D 	uInputTexture;
uniform sampler2D 	uDepthTexture;
uniform float		uNearDistance;
uniform float 		uFarDistance;
uniform int 		uMaxRadius;

void main() {
	float depth = texture2D(uDepthTexture, vTexCoord).r;
	float ldepth = linearizeDepth(depth, uNearDistance, uFarDistance);
	int radius = int(ldepth * uMaxRadius);
#ifdef HPASS
	vec2 direction = vec2(1.0, 0.0);
#endif HPASS
#ifdef VPASS
	vec2 direction = vec2(0.0, 1.0);
#endif VPASS
	fResult = incrementalGaussian(uInputTexture, radius, direction, 1, vTexCoord);
}