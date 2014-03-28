///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition;	//in model space

uniform mat4 uModelViewProjectionMatrix;	// model->clip

void main() {
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
}


///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 fResult;

void main() {
	float z = gl_FragCoord.z;
	fResult = vec4(z, z*z, z, 1.0);
}