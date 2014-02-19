///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition; 	//[-1;1]x[-1;1]x[-1;1]

uniform mat3 uViewRotationMatrix;
uniform mat4 uProjectionMatrix;

out vec3 vPosition;


void main() {
    gl_Position = uProjectionMatrix * vec4(uViewRotationMatrix * aPosition, 1.0);
    vPosition = aPosition;
}

///### FRAGMENT SHADER ###///
in vec3 vPosition;

uniform samplerCube uEnvironment;

layout (location = 0) out vec4 fResult;

void main() {
	fResult = texture(uEnvironment, vPosition);
}
