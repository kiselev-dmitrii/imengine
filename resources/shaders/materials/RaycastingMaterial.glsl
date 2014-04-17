///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;

out vec3 vVolumeTexcoords;

uniform mat4 uModelViewProjectionMatrix;

void main() {
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
    //любая координата у нас либо -0.5 либо 0.5 - прибавляя 0.5 получаем текстурные координаты 0.0-1.0
    vVolumeTexcoords = aPosition + vec3(0.5); 
}


///### FRAGMENT SHADER ###///
in vec3 vVolumeTexcoords;

layout (location = 0) out vec4 fResult;

uniform sampler3D 	uVolumeTexture;
uniform sampler2D	uDensityTexture;
uniform vec3 		uObjectSpaceCameraPosition;
uniform int 		uStep;

uniform float 		uMinDensity;
uniform float 		uMaxDensity; 
uniform vec4 		uClipPlane;

vec4 alphaBlending(in vec4 oldColor, in vec4 newColor) {
	float alpha = newColor.a - (newColor.a * oldColor.a);
	return vec4(oldColor.rgb + alpha*newColor.rgb, oldColor.a + alpha);
}

const int MAX_SAMPLES = 300;

void main() {
	// Получаем направление в текстурных координатах
	vec3 positionOS = vVolumeTexcoords - vec3(0.5);
	vec3 directionTS = positionOS - uObjectSpaceCameraPosition;		//directionOS = directionTS
	vec3 texelSize = 1.0/textureSize(uVolumeTexture, 0).xyz;

	// Трасировка
	vec3 positionTS = vVolumeTexcoords;
	vec3 dir = normalize(directionTS) * texelSize * uStep;

	vec4 result = vec4(0.0);
	for (int i = 0; i < MAX_SAMPLES; i++) {
		float density = texture3D(uVolumeTexture, positionTS).r;
		vec4 color;
		color.rgb = texture2D(uDensityTexture, vec2(density, 0.0)).rgb;
		color.a = density;

		// Если точка лежит с положительной стороны плоскости то набираем сумму
		float side = dot(uClipPlane, vec4(positionTS, 1.0));
		if (side > 0) {
			// Граница плоскости
			if (abs(side) < 3.0*length(texelSize)) {
				color = vec4(1,1,1, 0.05);
			}

			// Сумма
			if (density >= uMinDensity && density <= uMaxDensity) {
				result = alphaBlending(result, color);
			}
		}

		positionTS += dir;

		// Условия выхода - превышение суммы и выход за границы кубической текстуры
		if (result.a > 0.99) break;														
		if (any(greaterThan(positionTS, vec3(1.0))) || any(lessThan(positionTS, vec3(0.0)))) break;
	}

	fResult = result;
}
