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

/// Входные текстуры
uniform sampler2D 	uInputTexture;
uniform sampler2D	uNormalTexture;
uniform sampler2D 	uDepthTexture;

/// Для преобразования из View Space в Tex Space и обратно
uniform float 		uAspectRatio;
uniform float 		uTanHalfFovy;
uniform float 		uNearDistance;
uniform float 		uFarDistance;
uniform mat4 		uProjectionMatrix;

/// Параметры
uniform float 		uViewSpaceStep;
uniform int 		uMaxNumSamples;

void main() {
	vec2 texelSize = 1.0/textureSize(uDepthTexture, 0);

	/// Восстанавливаем позицию и нормаль
	vec3 positionVS = textureToViewSpace(vTexCoord, uDepthTexture, uAspectRatio, uTanHalfFovy, uNearDistance, uFarDistance);
	vec3 normalVS = decodeNormal(vTexCoord, uNormalTexture);

	vec3 v = -normalize(positionVS);
	vec3 r = reflect(-v, normalVS);
	vec3 dir = r*uViewSpaceStep;

	vec3 reflectColor = vec3(0.0);
	vec3 sampleVS = positionVS;
	for (int i = 0; i < uMaxNumSamples; ++i) {
		sampleVS += dir;
		vec2 sampleTS = viewToTextureSpace(sampleVS, uProjectionMatrix);

		if (any(greaterThan(sampleTS, vec2(1.0))) || any(lessThan(sampleTS, vec2(0.0)))) break;		//выход за границы

		float dist = depthToDistance(texture2D(uDepthTexture, sampleTS).r, uNearDistance, uFarDistance);
		if (dist < -sampleVS.z)	{ 		//поверхность ближе чем луч => луч врезался
			vec3 sampleNormal = decodeNormal(sampleTS, uNormalTexture);
			float orientation = dot(normalize(dir), sampleNormal);
			if (orientation < 0) reflectColor = texture2D(uInputTexture, sampleTS).xyz;
			break;
		}
	}

	vec3 color = texture2D(uInputTexture, vTexCoord).xyz;

	fResult = vec4(color * reflectColor + color, 1.0);
}