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

/// For tex space <- view space convertations
uniform float 		uNearDistance;
uniform float 		uFarDistance;
uniform mat4 		uProjectionMatrix;
uniform mat4 		uInvProjectionMatrix;

/// Unit vectors
uniform vec3 		uOffsets[MAX_SAMPLES];
uniform int 		uNumSamples;

/// Artist variables
uniform float 		uRadius;
uniform float 		uPenumbra;

void main() {
	vec3 positionVS = textureToViewSpace(vTexCoord, uDepthTexture, uNearDistance, uFarDistance, uInvProjectionMatrix);
	vec3 normalVS = decodeNormal(vTexCoord, uNormalTexture);

	float occlusion = 0.0;
	for (int i = 0; i < uNumSamples; ++i) {
		vec3 sampleVS = positionVS + uOffsets[i] * uRadius;					//отодвигаемся от данной точки
		vec2 sampleTS = viewToTextureSpace(sampleVS, uProjectionMatrix);	//получаем ее текстурные координаты
		vec3 surfaceVS = textureToViewSpace(sampleTS, uDepthTexture, uNearDistance, uFarDistance, uInvProjectionMatrix);	//получаем точку на поверхности с этими текстурными коориданатами

		vec3 surfaceDir = normalize(surfaceVS - positionVS);				//вектор от данной точки, до точки на поверхности
		float nDOTs = max(dot(normalVS, surfaceDir), 0);					//Чем более вектора сонаправлены, тем больше учет
		float dist = distance(positionVS, surfaceVS);						//расстояние от данной точки до точки на поверхности

		float a = 1.0 - smoothstep(uRadius - uPenumbra, uRadius, dist);		//не учитываем вектора, которые лежат дальше радиуса
		float b = nDOTs;													//не учитываем вектора, лежащие сзади нормали
		occlusion += a*b;
	}
	occlusion /= uNumSamples;

	vec4 color = texture2D(uInputTexture, vTexCoord);

	fResult = vec4(color - occlusion);
}