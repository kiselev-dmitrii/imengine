///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition;	//in model space

uniform mat4 uModelViewProjectionMatrix;	// model->clip

void main() {
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
}


///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 fLightBuffer;

uniform vec3 		uIntensity;

void main() {
	fLightBuffer = vec4(uIntensity, 1.0);
}
