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

void main() {
        vec4 color = 
        	textureOffset(uTexture, vTexCoord, ivec2(-1,-1)) +
        	textureOffset(uTexture, vTexCoord, ivec2(-1,+1)) +
        	textureOffset(uTexture, vTexCoord, ivec2(+1,-1)) +
        	textureOffset(uTexture, vTexCoord, ivec2(+1,+1));
        fResult = color/4.0f;
}