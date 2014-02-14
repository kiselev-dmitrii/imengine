///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;	// in [-1;1]x[-1;1]x[0;0]

out vec2 vTexCoord;

void main() {
        gl_Position = vec4(aPosition, 0.0, 1.0);
	vTexCoord = 0.5*aPosition + vec2(0.5);
}


///### FRAGMENT SHADER ###///

in vec2 vTexCoord;

layout (location = 0) out vec4 fResult;

uniform sampler2D 	uTexture;
uniform float 		uSigma;

vec4 blurHorizontal(sampler2D texture, vec2 texCoord, float sigma) {
        float dx = 1.0/textureSize(texture, 0).x;

        /// Вычисляем веса и их сумму
        float weights[10];
        float summ = 0;
        for(int i=0; i<10; ++i) {
                weights[i] = exp(-(i*i)/(2.0*sigma*sigma)) * (1.0/(sqrt(2.0*3.1415)*sigma));
                summ += weights[i];
        }

        /// 19 сэмплов
        vec4 result = texture2D(texture, texCoord) * weights[0]/summ;
        for(int i=1; i<10; ++i) {
                result += texture2D(texture, texCoord + vec2(dx*i,0.0)) * weights[i]/(2.0*summ);
                result += texture2D(texture, texCoord + vec2(-dx*i,0.0)) * weights[i]/(2.0*summ);
        }
        return result;
}

void main() {
	fResult = blurHorizontal(uTexture, vTexCoord, uSigma);
}