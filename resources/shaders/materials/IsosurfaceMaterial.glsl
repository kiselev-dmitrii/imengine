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
uniform float 		uStep;
uniform float 		uThresholdDensity;

/** Функция получает градиент в объеме volume c первым приближением
  */
vec3 gradient(vec3 p, sampler3D volume) {
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

/** Функция поулчает на вход начальную точку в текстурных координатах, и вектор направления в этой же системе координат
    Луч проходит через объем volume с шагом stepSize до тех пор, пока не дойдет до пространства с плотностью density.
    В результате, функция возвращает нормаль и позицию в текстурном пространстве к изоповрехности с плотностью density в точке 
    соприкосновения луча с этой поверхностью.
    Если луч не встречается с изоповерхностью такой плотности, то возаращается false, иначе true
*/
bool isosurfaceVolumeRaycasting(vec3 vTexSpaceStart, vec3 vTexSpaceDirection, float stepSize, sampler3D volume, float density,
								out vec3 outTexSpaceNormal, out vec3 outTexSpacePosition) {
	const int 	MAX_SAMPLES = 300;	

	vec3 samplePoint = vTexSpaceStart;
	vec3 stepVector = stepSize * normalize(vTexSpaceDirection);

	float prevDensity = texture3D(volume, samplePoint).r;
	samplePoint += stepVector;
	for (int i = 0; i < MAX_SAMPLES; ++i) {
		float curDensity = texture3D(volume, samplePoint).r;


		if (prevDensity < density && curDensity >= density) {
			vec3 prevPoint = samplePoint - stepVector;
			vec3 curPoint = samplePoint;

			float percent = (density - prevDensity)/(curDensity - prevDensity);		//приводим density к [0,1]
			vec3 p = mix(prevPoint, curPoint, percent);								//считая что p плотность меняется между точками линейно
			vec3 n = -normalize(gradient(p, volume));								//градиент показывает наибольшее возрастание функции
																					//так как плотность у нас возрастает, то берем -gradient
			outTexSpacePosition = p;
			outTexSpaceNormal = n;
			return true;
		}

		if (any(greaterThan(samplePoint, vec3(1.0))) || any(lessThan(samplePoint, vec3(0.0)))) break; 	//вышли за границы [0-1]
		samplePoint += stepVector;
		prevDensity = curDensity;
	}
	return false;
}

vec4 phongShading(vec3 L, vec3 N, vec3 V, float specPower, vec3 diffuseColor) {
	float diffuse = max(dot(L,N),0.0);
	vec3 halfVec = normalize(L+V);
	float specular = pow(max(0.00001,dot(halfVec,N)),specPower);	
	return vec4(((diffuse*diffuseColor + specular)),1.0);
}

void main() {
	vec3 vObjectSpacePosition = vVolumeTexcoords - vec3(0.5);		//позиция вершины в ObjectSpace
	vec3 vObjectSpaceRayDirection = vObjectSpacePosition - uObjectSpaceCameraPosition;

	vec3 vTexSpacePosition;
	vec3 vTexSpaceNormal;
	bool isSurface = isosurfaceVolumeRaycasting(vVolumeTexcoords, vObjectSpaceRayDirection, uStep, uVolumeTexture, uThresholdDensity, 
										        vTexSpaceNormal, vTexSpacePosition);
	if (isSurface) {
		vec3 vTexSpaceCameraPosition = uObjectSpaceCameraPosition + vec3(0.5);
		vec3 V = normalize(vTexSpaceCameraPosition - vTexSpacePosition);
		vec3 L = V;
		fResult = phongShading(L, vTexSpaceNormal, V, 30.0, vec3(0.4));
	} else {
		fResult = vec4(0.0);
	}
}
