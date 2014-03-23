
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

	if (numSamples == 0) return texture2D(texture, origin);
		
	vec3 gaussInc;
	gaussInc.x = 1.0 / (sqrt(TWO_PI) * sigma);
	gaussInc.y = exp(-1/(2*sigma*sigma));
	gaussInc.z = gaussInc.y * gaussInc.y;
	
	vec4 result = texture2D(texture, origin) * gaussInc.x;	
	for (int i = 1; i < numSamples; ++i) {
		gaussInc.xy *= gaussInc.yz;
		
		vec2 offset = float(i) * direction * texelSize * float(step);
		result += texture2D(texture, origin - offset) * gaussInc.x;
		result += texture2D(texture, origin + offset) * gaussInc.x;
	}
	
	return result;
}