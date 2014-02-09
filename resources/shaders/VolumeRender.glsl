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
uniform vec3 		uObjectSpaceCameraPosition;

vec4 volumeRaycasting(vec3 vStartUV, vec3 vUVSpaceRayDirection, float stepSize, sampler3D volume) {
	const int 	MAX_SAMPLES = 300;

	vec3 currentUV = vStartUV;
	vec3 step = stepSize * normalize(vUVSpaceRayDirection);

	/*
	float sum = 0.0;
	for (int i = 0; i < MAX_SAMPLES; i++) {
		float sample = texture3D(volume, currentUV).r;
		sum += sample;

		if (sum >= 1.0) break;																			//превышение суммы
		if (any(greaterThan(currentUV, vec3(1.0))) || any(lessThan(currentUV, vec3(0.0)))) break;		//выход за границы

		currentUV += step;
	}
	return sum;
	*/
	vec4 result = vec4(0.0);
	for (int i = 0; i < MAX_SAMPLES; i++) {
		float sample = texture3D(volume, currentUV).r;

		float prevAlpha = sample - (sample * result.a);
		result.rgb += prevAlpha * vec3(sample);
		result.a += prevAlpha;

		if (result.a > 0.99) break;																			//превышение суммы
		if (any(greaterThan(currentUV, vec3(1.0))) || any(lessThan(currentUV, vec3(0.0)))) break;		//выход за границы

		currentUV += step;
	}
	return result;

}

void main() {
	vec3 vObjectSpacePosition = vVolumeTexcoords - vec3(0.5);		//позиция вершины в ObjectSpace
	vec3 vObjectSpaceRayDirection = vObjectSpacePosition - uObjectSpaceCameraPosition;

	fResult = volumeRaycasting(vVolumeTexcoords, vObjectSpaceRayDirection, 0.01, uVolumeTexture);
}
