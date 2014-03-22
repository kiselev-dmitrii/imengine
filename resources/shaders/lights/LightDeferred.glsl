///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;	// in [-1;1]x[-1;1]x[0;0]

out vec2 vTexCoord;
out vec3 vViewRay;

uniform float uFarDistance;
uniform float uAspectRatio;
uniform float uTanFovyDiv2;

void main() {
	vTexCoord = 0.5*aPosition + 0.5;

        // Создание луча направленного до очередной точки на farPlane
        vViewRay = vec3(
                aPosition.x * uAspectRatio * uFarDistance * uTanFovyDiv2,
                aPosition.y * uFarDistance * uTanFovyDiv2,
                -uFarDistance
        );
        vViewRay = normalize(vViewRay);

        gl_Position = vec4(aPosition, 0.0, 1.0);
}


///### FRAGMENT SHADER ###///

in vec2 vTexCoord;
in vec3 vViewRay;

layout (location = 0) out vec4 fLightBuffer;

uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferMaterial;
uniform sampler2D uGBufferGeometry;
uniform sampler2D uGBufferDepth;

struct Light {
        vec3 diffuse;
        vec3 specular;
        vec3 positionVS;
};
uniform Light uLight;

void main() {
        vec3 viewRay = normalize(vViewRay);
        float depth = texture2D(uGBufferDepth, vTexCoord).r;

        /// Реконструируем позицию каждого пикселя
        vec3 positionVS = vViewRay * depth;
        vec3 normalVS = texture2D(uGBufferGeometry, vTexCoord).xyz * 2.0 - 1.0;

        /// Вычисляем освещение по фонгу
        vec3 s = normalize(uLight.positionVS - positionVS);
        vec3 v = normalize(-positionVS);
        vec3 diffuseColor =     texture2D(uGBufferDiffuse, vTexCoord).rgb * 
                                uLight.diffuse *
                                max(dot(s, normalVS), 0.0);

	fLightBuffer = vec4(positionVS, 1.0);
}