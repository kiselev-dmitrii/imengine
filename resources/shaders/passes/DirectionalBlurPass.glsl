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
uniform vec2 		uDirection;
uniform int 		uRadius;
uniform int 		uStep;

vec4 incrementalGaussian(in sampler2D texture, in int radius, in vec2 direction, in int step, in vec2 origin) {
	int numSamples = (radius/2)/step;
	vec2 texelSize = 1.0 / vec2(textureSize(texture, 0));	
	float sigma = float(radius/step) / 8.0;

	if (numSamples == 0) return texture2D(texture, origin);
		
	vec3 gaussInc;
	gaussInc.x = 1.0 / (sqrt(TWO_PI) * sigma);
	gaussInc.y = exp(-1/(2*sigma*sigma));
	gaussInc.z = gaussInc.y * gaussInc.y;
	
	vec4 result = texture2D(texture, origin) * gaussInc.x;	
	for (int i = 1; i < numSamples; ++i) {
		gaussInc.xy *= gaussInc.yz;
		
		vec2 offset = float(i) * direction * texelSize * float(step);
		result += texture2D(texture, origin - offset) * gaussInc.x;
		result += texture2D(texture, origin + offset) * gaussInc.x;
	}
	
	return result;
}

void main() {
	fResult = incrementalGaussian(uTexture, uRadius, uDirection, uStep, vTexCoord);
}