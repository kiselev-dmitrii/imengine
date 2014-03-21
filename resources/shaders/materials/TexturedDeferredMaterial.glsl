///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition;	//in model space
layout (location = 1) in vec3 aNormal;		//in model space
layout (location = 2) in vec2 aTexcoords;	//in texture space
layout (location = 3) in vec3 aTangent;		//in model space

uniform mat4 uModelViewProjectionMatrix;	// model->clip
uniform mat4 uModelViewMatrix;				// model->view
uniform mat3 uNormalMatrix;					// model->view for vectors

struct Vertex {
	vec3 	viewSpacePosition;
	vec2	texCoords;
	mat3	tangentToViewMatrix;		//tangent -> view
};
out Vertex v;

void main() {
	v.viewSpacePosition = (uModelViewMatrix * vec4(aPosition, 1.0)).xyz;
	v.texCoords = aTexcoords;
	// строим матрицу перехода от tangentSpace во viewSpace
	vec3 viewSpaceNormal = uNormalMatrix * aNormal;
	vec3 viewSpaceTangent = uNormalMatrix * aTangent;
	vec3 viewSpaceBitangent = cross(viewSpaceTangent, viewSpaceNormal);
	v.tangentToViewMatrix = mat3(viewSpaceTangent, viewSpaceBitangent, viewSpaceNormal);

    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
}


///### FRAGMENT SHADER ###///

struct Vertex {
	vec3 	viewSpacePosition;
	vec2	texCoords;
	mat3	tangentToViewMatrix;		//tangent -> view
};
in Vertex v;

layout (location = 0) out vec4 fGBufferDiffuse;
layout (location = 1) out vec4 fGBufferMaterial;
layout (location = 2) out vec4 fGBufferGeometry;

uniform sampler2D	uDiffuseTexture;
uniform sampler2D	uSpecularTexture;
uniform sampler2D 	uNormalTexture;

void main() {
	/// Fill diffuse buffer
	fGBufferDiffuse = texture2D(uDiffuseTexture, v.texCoords);

	/// Fill material buffer
	float specularLevel = texture2D(uSpecularTexture, v.texCoords).r;
	float specularExponent = 1.0;
	float metallicLevel = 1.0;
	float ambientOcclusion = 1.0;
	fGBufferMaterial = vec4(specularLevel, specularExponent, metallicLevel, ambientOcclusion);

	/// Fill geometry buffer
	vec3 n = texture2D(uNormalTexture, v.texCoords).xyz * 2.0 - 1.0;	// tangent space normal
	n = vec3(0,0,1);		//remove it
	n = v.tangentToViewMatrix * n;										// view space normal
	fGBufferGeometry = vec4(n*0.5 + 0.5, 1.0);
}
