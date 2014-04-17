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
uniform sampler2D 	uDensityTexture;
uniform vec3 		uObjectSpaceCameraPosition;
uniform int 		uStep;

uniform float 		uThresholdDensity;

/** Функция возвращает градиент в объеме volume c первым приближением
  */
vec3 gradient(in vec3 p, in sampler3D volume) {
	const float delta = 0.01;
	const vec3 dx = vec3(delta, 0.0, 0.0);
	const vec3 dy = vec3(0.0, delta, 0.0);
	const vec3 dz = vec3(0.0, 0.0, delta);

	vec3 g;
	g.x = texture3D(volume, p + dx).r - texture3D(volume, p - dx).r;
	g.y = texture3D(volume, p + dy).r - texture3D(volume, p - dy).r;
	g.z = texture3D(volume, p + dz).r - texture3D(volume, p - dz).r;
	g /= 2.0*delta;

	return g;
}

vec4 phongShading(vec3 L, vec3 N, vec3 V, float specPower, vec3 diffuseColor) {
	float diffuse = max(dot(L,N),0.0);
	vec3 halfVec = normalize(L+V);
	float specular = pow(max(0.00001,dot(halfVec,N)),specPower);	
	return vec4(((diffuse*diffuseColor + 0.2*specular)),1.0);
}

const int 	MAX_SAMPLES = 300;	

void main() {
	// Получаем направление в текстурных координатах
	vec3 positionOS = vVolumeTexcoords - vec3(0.5);
	vec3 directionTS = positionOS - uObjectSpaceCameraPosition;		//directionOS = directionTS
	vec3 texelSize = 1.0/textureSize(uVolumeTexture, 0).xyz;

	/// Текущая позиция и направление
	vec3 positionTS = vVolumeTexcoords;
	vec3 dir = normalize(directionTS) * texelSize * uStep;

	// Будем искать точку и нормаль в ней, где плотность равна uThresholdDensity
	vec3 pointTS = vec3(0.0);
	vec3 normalTS = vec3(0.0);
	bool foundPoint = false;

	/// Первый шаг
	float prevDensity = texture3D(uVolumeTexture, positionTS).r;
	positionTS += dir;

	for (int i = 0; i < MAX_SAMPLES; ++i) {
		float curDensity = texture3D(uVolumeTexture, positionTS).r;

		// Нашли границу
		if (prevDensity < uThresholdDensity && curDensity >= uThresholdDensity) {
			vec3 prevPoint = positionTS - dir;
			vec3 curPoint = positionTS;

			/// Считаем точку pointTS как линейную интерполяцию между соседними точками по плотности
			/// А нормаль считаем как градиент
			float percent = (uThresholdDensity - prevDensity)/(curDensity - prevDensity);
			pointTS = mix(prevPoint, curPoint, percent);							
			normalTS = -normalize(gradient(pointTS, uVolumeTexture));							
			foundPoint = true;							

			break;
		}

		positionTS += dir;
		prevDensity = curDensity;

		/// Условие выхода - вышли за границы текстуры
		if (any(greaterThan(positionTS, vec3(1.0))) || any(lessThan(positionTS, vec3(0.0)))) break;
	}

	/// Если нашли границу - то считаем освещение поверхности
	if (foundPoint) {
		vec3 cameraPositionTS = uObjectSpaceCameraPosition + vec3(0.5);
		vec3 V = normalize(cameraPositionTS - pointTS);
		vec3 L = V;
		vec3 diffuseColor = texture2D(uDensityTexture, vec2(uThresholdDensity, 0.0)).rgb;
		fResult = phongShading(L, normalTS, V, 30.0, diffuseColor);
	} else {
		fResult = vec4(0.0);
	}
}
