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
uniform float 		uNearDistance;
uniform float 		uFarDistance;
uniform mat4 		uProjectionMatrix;
uniform mat4 		uInvProjectionMatrix;

/// Параметры
uniform float 		uScreenRadius;		//радиус в пикселях
uniform float 		uViewRadius;		//радиус во view space
uniform float 		uPower;				//четкость теней
uniform int  		uNumSamples;		//количество сэмплов <= 16

/// Случайное распределение Пуассона на диске радиуса 1
const vec2 poisson[] = vec2[](   
    vec2( -0.94201624,  -0.39906216 ),
    vec2(  0.94558609,  -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2(  0.34495938,   0.29387760 ),
    vec2( -0.91588581,   0.45771432 ),
    vec2( -0.81544232,  -0.87912464 ),
    vec2( -0.38277543,   0.27676845 ),
    vec2(  0.97484398,   0.75648379 ),
    vec2(  0.44323325,  -0.97511554 ),
    vec2(  0.53742981,  -0.47373420 ),
    vec2( -0.26496911,  -0.41893023 ),
    vec2(  0.79197514,   0.19090188 ),
    vec2( -0.24188840,   0.99706507 ),
    vec2( -0.81409955,   0.91437590 ),
    vec2(  0.19984126,   0.78641367 ),
    vec2(  0.14383161,  -0.14100790 )
);

void main() {
	vec2 texelSize = 1.0/textureSize(uDepthTexture, 0);

	/// Восстанавливаем позицию и нормаль
	vec3 positionVS = textureToViewSpace(vTexCoord, uDepthTexture, uNearDistance, uFarDistance, uInvProjectionMatrix);
	vec3 normalVS = decodeNormal(vTexCoord, uNormalTexture);

	/// Считаем затенение
	float occlusion = 0.0;
	for (int i = 0; i < uNumSamples; ++i) {
		/// Берем точку на поверхности 
		vec2 sampleTS = vTexCoord + poisson[i] * (uScreenRadius * texelSize);
		vec3 sampleVS = textureToViewSpace(sampleTS, uDepthTexture, uNearDistance, uFarDistance, uInvProjectionMatrix);

		/// Смотрим расстояние до этой точки и угол между вектором к ней и нормалью
		vec3 sampleDir = normalize(sampleVS - positionVS);
		float dist = distance(sampleVS, positionVS);
		float nDOTs = max(dot(sampleDir, normalVS), 0);

		/// Если расстояние до точки меньше uViewRadius, то считаем что она дает вклад в затенение этой точки
		/// Если больше двух радиусом, то точка не дает затенения
		float a = 1.0 - smoothstep(uViewRadius, uViewRadius*2.0, dist);
		/// Если точка не лежит в полусфере, ориентированной по нормали (dot(sampleDir, normalVS) < 0), то не учитываем эту точку
		float b = nDOTs;

		occlusion += a*b;
	}
	occlusion /= uNumSamples;

	vec4 color = texture2D(uInputTexture, vTexCoord);
	float shade = 1.0 - pow(occlusion, uPower);

	fResult = vec4(color * shade);
}