///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition;	//in model space
layout (location = 1) in vec3 aNormal;		//in model space
layout (location = 2) in vec2 aTexcoords;	//in model space

uniform mat4 uModelViewProjectionMatrix;	// model->clip
uniform mat4 uModelViewMatrix;				// model->view
uniform mat3 uNormalMatrix;					// model->view for vectors

out vec3 vViewSpaceNormal;
out vec3 vViewSpacePosition;
out vec2 vTexCoords;

void main() {
	vViewSpacePosition = (uModelViewMatrix * vec4(aPosition, 1.0)).xyz;
	vViewSpaceNormal = uNormalMatrix * aNormal;
	vTexCoords = aTexcoords;
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
}


///### FRAGMENT SHADER ###///

in vec3 vViewSpaceNormal;
in vec3 vViewSpacePosition;

layout (location = 0) out vec4 fGBufferDiffuse;
layout (location = 1) out vec4 fGBufferMaterial;
layout (location = 2) out vec4 fGBufferGeometry;

uniform vec3 	uDiffuseColor = vec3(1.0);
uniform float 	uSpecularLevel = 1.0;
uniform float 	uSpecularExponent = 1.0;
uniform float 	uMetallicLevel = 1.0;

void main() {
	fGBufferDiffuse = vec4(uDiffuseColor, 1.0);
	fGBufferMaterial = vec4(uSpecularLevel, uSpecularExponent, uMetallicLevel, 1.0);
	fGBufferGeometry = vec4(vViewSpaceNormal*0.5 + 0.5, 1.0);
}
