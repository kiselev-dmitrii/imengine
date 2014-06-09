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
uniform int 		uNumRefinements;	

vec3 calculateReflection(in float stepVS, in int maxNumSamples, in int numRefinements) {
	vec2 texelSize = 1.0/textureSize(uDepthTexture, 0);

	/// Восстанавливаем позицию и нормаль
	vec3 positionVS = textureToViewSpace(vTexCoord, uDepthTexture, uAspectRatio, uTanHalfFovy, uNearDistance, uFarDistance);
	vec3 normalVS = decodeNormal(vTexCoord, uNormalTexture);

	vec3 v = -normalize(positionVS);
	vec3 r = reflect(-v, normalVS);
	vec3 dir = r * stepVS;

	vec3 prevPositionVS = positionVS;
	vec3 curPositionVS = prevPositionVS + dir;
	for (int i = 0; i < maxNumSamples; ++i) {
		vec2 curPositionTS = viewToTextureSpace(curPositionVS, uProjectionMatrix);
		if (any(greaterThan(curPositionTS, vec2(1.0))) || any(lessThan(curPositionTS, vec2(0.0)))) break;

		/// Если расстояние до поверхности меньше чем расстояние до луча, то мы внутри поверхности
		float dist = depthToDistance(texture2D(uDepthTexture, curPositionTS).r, uNearDistance, uFarDistance);
		bool wasIntersected = dist < -curPositionVS.z;	
		if (wasIntersected)	{ 					
			vec3 startVS = prevPositionVS;
			vec3 endVS = curPositionVS;
			vec3 centerVS = (startVS + endVS)/2.0f;
			vec2 centerTS = viewToTextureSpace(centerVS, uProjectionMatrix);
			for (int j = 0; j < numRefinements; ++j) {
				float dst = depthToDistance(texture2D(uDepthTexture, centerTS).r, uNearDistance, uFarDistance);
				bool intersected = dst < -centerVS.z;	

				if (intersected) endVS = centerVS;
				else startVS = centerVS;
				centerVS = (startVS + endVS)/2.0f;
				centerTS = viewToTextureSpace(centerVS, uProjectionMatrix);
			}

			vec3 curNormalVS = decodeNormal(centerTS, uNormalTexture);
			float orientation = dot(normalize(dir), curNormalVS);
			if (orientation < 0) return texture2D(uInputTexture, centerTS).rgb;
			else return vec3(0,0,0);
		}

		prevPositionVS = curPositionVS;
		curPositionVS = prevPositionVS + dir;
	}

	return vec3(0,0,0);
}

void main() {
	vec3 reflectColor = calculateReflection(uViewSpaceStep, uMaxNumSamples, uNumRefinements);
	vec3 color = texture2D(uInputTexture, vTexCoord).xyz;

	fResult = vec4(color * reflectColor + color, 1.0);
}