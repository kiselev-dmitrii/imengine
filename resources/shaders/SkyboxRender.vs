///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition; 	//[-1;1]x[-1;1]x[-1;1]

uniform mat3 uViewRotationMatrix;

out vec2 vPosition;


void main() {
    gl_Position = uViewRotationMatrix * aPosition;
    vPosition = aPosition;
}

///### FRAGMENT SHADER ###///
in vec2 vPosition;

uniform samplerCube uEnvironment;

layout (location = 0) out vec4 fResult;

void main() {
	fResult = texture(uEnvironment, vPosition);
}
