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
uniform vec2 		uDirection;

uniform int 		uNearRadius;
uniform int 		uFarRadius;
uniform float		uFocusStart;
uniform float 		uFocusEnd;

void main() {
	float depth = texture2D(uDepthTexture, vTexCoord).r;
	float ldepth = linearizeDepth(depth, uNearDistance, uFarDistance);

	int radius = 0;
	if (ldepth < uFocusStart) radius = int (uNearRadius * (1.0 - ldepth/uFocusStart));
	if (ldepth > uFocusEnd) radius = int (((ldepth - uFocusEnd)/(1.0 - uFocusEnd)) * uFarRadius);
	/*
	float rad1 = uMaxRadius * (1.0 - ldepth/focusStart);
	float rad2 = ((ldepth - focusEnd)/(1.0 - focusEnd)) * uMaxRadius;
	float radius = max(max(rad1, rad2), 0.0f);
	*/

	fResult = incrementalGaussian(uInputTexture, radius, uDirection, 1, vTexCoord);
}