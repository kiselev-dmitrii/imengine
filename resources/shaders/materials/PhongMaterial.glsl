///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition;	//in model space
layout (location = 1) in vec3 aNormal;		//in model space
layout (location = 2) in vec2 aTexcoords;	//in model space

uniform mat4 uModelViewProjectionMatrix;	// model->clip
uniform mat4 uModelViewMatrix;				// model->view
uniform mat3 uNormalMatrix;					// model->view for vectors

out vec3 vViewSpaceNormal;
out vec3 vViewSpacePosition;

void main() {
	vViewSpacePosition = (uModelViewMatrix * vec4(aPosition, 1.0)).xyz;
	vViewSpaceNormal = uNormalMatrix * aNormal;
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
}


///### FRAGMENT SHADER ###///


struct Material {
	vec3	ambientColor;
	vec3	diffuseColor;
	vec3 	specularColor;
	float	shininess;
};

struct Light {
	vec3 	vViewSpacePosition;
	vec3	color;
};

in vec3 vViewSpaceNormal;
in vec3 vViewSpacePosition;

layout (location = 0) out vec4 fResult;

uniform Material 	uMaterial;
uniform Light 		uLight;	

vec3 phongShading(Material material, Light light, vec3 vViewSpacePosition, vec3 vViewSpaceNormal) {
	vec3 s = normalize(light.vViewSpacePosition - vViewSpacePosition);
	vec3 v = normalize(-vViewSpacePosition);

	float sDOTn = max(dot(s,vViewSpaceNormal), 0.0);		//cos(угол между нормалью и вектором к свету)
	vec3 diffuse = light.color * material.diffuseColor * sDOTn;

	vec3 specular = vec3(0.0);
	if (sDOTn > 0) {										// если угол между n и s < 90 градусов
		vec3 r = reflect(-s, vViewSpaceNormal);
		specular = light.color * material.specularColor * pow(max(dot(r,v), 0.0), material.shininess);
	}

	return diffuse + specular;
}

void main() {
	fResult = vec4(phongShading(uMaterial, uLight, vViewSpacePosition, vViewSpaceNormal), 1.0);
}