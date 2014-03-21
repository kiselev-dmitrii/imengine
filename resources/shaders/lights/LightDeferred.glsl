///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;	// in [-1;1]x[-1;1]x[0;0]

out vec2 vTexCoord;

void main() {
	vTexCoord = 0.5*aPosition + vec2(0.5);
        gl_Position = vec4(aPosition, 0.0, 1.0);
}


///### FRAGMENT SHADER ###///

in vec2 vTexCoord;

layout (location = 0) out vec4 fLightBuffer;

uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferMaterial;
uniform sampler2D uGBufferGeometry;
uniform sampler2D uGBufferDepth;

void main() {

	fLightBuffer = vec4(vTexCoord, 0.0, 1.0);
}