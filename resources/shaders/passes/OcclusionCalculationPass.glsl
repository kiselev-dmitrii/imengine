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

const int MAX_SAMPLES = 128;

uniform sampler2D 	uInputTexture;
uniform sampler2D	uNormalTexture;
uniform sampler2D 	uDepthTexture;

uniform float 		uRadius;

uniform float 		uNearDistance;
uniform float 		uFarDistance;
uniform mat4 		uProjectionMatrix;
uniform mat4 		uInvProjectionMatrix;

uniform vec3 		uOffsets[MAX_SAMPLES];
uniform int 		uNumSamples;

void main() {
	float depth = texture2D(uDepthTexture, vTexCoord).r;
	
	vec3 positionVS = textureToViewSpace(vTexCoord, uDepthTexture, uNearDistance, uFarDistance, uInvProjectionMatrix);
	vec2 positionTS = viewToTextureSpace(positionVS, uProjectionMatrix);

	fResult = vec4(uOffsets[0], 1.0);
}