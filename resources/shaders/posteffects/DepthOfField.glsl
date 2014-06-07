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

uniform sampler2D 	uInputTexture;
uniform sampler2D 	uDepthTexture;

uniform float		uNearDistance;
uniform float 		uFarDistance;
uniform vec2 		uDirection;

uniform int 		uNearMaxRadius;
uniform int 		uFarMaxRadius;
uniform float		uFocusStart;
uniform float 		uFocusEnd;

vec4 dofGaussian(in sampler2D texture, in sampler2D depthTexture, in int radius, in vec2 direction, in int step, in vec2 origin) {
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
  	vec4 centerSample = texture2D(texture, origin);
  	vec4 result = centerSample * gaussInc.x;
  	float sum = gaussInc.x;
  	gaussInc.xy *= gaussInc.yz;

  	/// Расходимся от центра по оси direction и считаем суммы
  	for (float i = 1.0f; i <= numSamples; i++) { 
  		vec2 offset = i * direction * texelSize * float(step);

  		vec2 point; 
  		float depth;

  		point = origin - offset;
  		depth = linearizeDepth(texture2D(depthTexture, point).r, uNearDistance, uFarDistance);
  		if (depth > uFocusEnd) result += texture2D(texture, point) * gaussInc.x;
  		else result += centerSample * gaussInc.x;

		point = origin + offset;
  		depth = linearizeDepth(texture2D(depthTexture, point).r, uNearDistance, uFarDistance);
  		if (depth > uFocusEnd) result += texture2D(texture, point) * gaussInc.x;
  		else result += centerSample * gaussInc.x;

    	sum += 2 * gaussInc.x;
    	gaussInc.xy *= gaussInc.yz;
  	}

  	return  result / sum;
}

void main() {
	float depth = texture2D(uDepthTexture, vTexCoord).r;
	float ldepth = linearizeDepth(depth, uNearDistance, uFarDistance);

	int radius = 0;
	if (ldepth < uFocusStart) {
		radius = int (uNearMaxRadius * (1.0 - ldepth/uFocusStart));
		fResult = incrementalGaussian(uInputTexture, radius, uDirection, 1, vTexCoord);

	} else if (ldepth > uFocusEnd) {
		radius = int (((ldepth - uFocusEnd)/(1.0 - uFocusEnd)) * uFarMaxRadius);
		fResult = dofGaussian(uInputTexture, uDepthTexture, radius, uDirection, 1, vTexCoord);

	} else {
		fResult = texture2D(uInputTexture, vTexCoord);
	}

}