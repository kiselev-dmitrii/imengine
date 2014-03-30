
const float PI =		3.1415926535898;
const float TWO_PI =	6.2831853071795;
const float E = 		2.7182818284590;

/// Возвращает яркость пикселя
float luma( vec3 color ) {
	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

/// Линеаризует глубину
float linearizeDepth(in float depth, in float near, in float far) {
        return (2.0*near) / (far+near - depth*(far-near));
}

/// Размывает по Гауссу точку origin в текстуре texture по направлению direction с радиусом radius. 
/// Параметр step влияет на качество исходного изображения. Для лучшего резульата следует принять step = 1
vec4 incrementalGaussian(in sampler2D texture, in int radius, in vec2 direction, in int step, in vec2 origin) {
	int numSamples = (radius/2)/step;
	vec2 texelSize = 1.0 / vec2(textureSize(texture, 0));	
	float sigma = float(radius/step) / 8.0;

	/// Ранний выход, если радиус нулевой или мал
	if (numSamples == 0) return texture2D(texture, origin);
		
	/// Вектор для хранения коэф. гауссовского распредления
	vec3 gaussInc;
	gaussInc.x = 1.0 / (sqrt(TWO_PI) * sigma);
	gaussInc.y = exp(-1/(2*sigma*sigma));
	gaussInc.z = gaussInc.y * gaussInc.y;

  	/// Центральный первый сэмпл
  	vec4 result = texture2D(texture, origin) * gaussInc.x;
  	float sum = gaussInc.x;
  	gaussInc.xy *= gaussInc.yz;

  	/// Расходимся от центра по оси direction и считаем суммы
  	for (float i = 1.0f; i <= numSamples; i++) { 
  		vec2 offset = i * direction * texelSize * float(step);
    	result += texture2D(texture, origin - offset) * gaussInc.x;         
    	result += texture2D(texture, origin + offset) * gaussInc.x;

    	sum += 2 * gaussInc.x;
    	gaussInc.xy *= gaussInc.yz;
  	}

  return  result / sum;
}

/// Трансформирует texture space координаты во view space координаты
/// Для данной операции используется обратная матрица проекции, а также расстояние до ближней и дальней плоскости отсечения
vec3 textureToViewSpace(in vec2 texCoord, in sampler2D depthBuffer, in float near, in float far, in mat4 invProjection) {
        float depth = texture2D(depthBuffer, texCoord).r;
        vec3 ndc = vec3(texCoord, depth) * 2.0 - 1.0;
        vec4 clip;
        clip.w = (2*near*far) / (near + far + ndc.z * (near - far));
        clip.xyz = ndc.xyz * clip.w;

        return (invProjection * clip).xyz;
}

/// Трансформирует view space координаты в texture space координаты
vec2 viewToTextureSpace(in vec3 viewCoord, in mat4 projectionMatrix) {
	vec4 clip = projectionMatrix * vec4(viewCoord, 1.0);
	vec3 ndc = clip.xyz / clip.w;
	return (ndc.xy * 0.5 + 0.5);
}

/// Декодирует нормаль из текстуры нормалей в точке texCoord
vec3 decodeNormal(in vec2 texCoord, in sampler2D geometryBuffer) {
	return texture2D(geometryBuffer, texCoord).xyz * 2.0 - 1.0;
}