///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;        // in [-1;1]x[-1;1]x[0;0]

out vec2 vTexCoord;
out vec3 vViewRay;

/// Для генерирования vViewRay
uniform float uAspectRatio;
uniform float uTanHalfFovy;

void main() {
        vTexCoord = 0.5*aPosition + 0.5;
        vViewRay = vec3(
                aPosition.x * uAspectRatio * uTanHalfFovy,
                aPosition.y * uTanHalfFovy,
                -1
        );
        gl_Position = vec4(aPosition, 0.0, 1.0);
}


///### FRAGMENT SHADER ###///
#include "../common/functions.glsl"

in vec2 vTexCoord;
in vec3 vViewRay;
layout (location = 0) out vec4 fLightBuffer;

/// Входные данные для освещения
uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferMaterial;
uniform sampler2D uGBufferGeometry;
uniform sampler2D uGBufferDepth;

/// Для преобразования из Texture Space во View Space
uniform float uNearDistance;
uniform float uFarDistance;

struct Light {
        vec3    diffuse;
        vec3    specular;
        float   attenuation;

        vec3    positionVS;
};
uniform Light uLight;

void main() {
        /// Реконструируем позицию каждого пикселя
        float depth = texture2D(uGBufferDepth, vTexCoord).r;
        vec3 positionVS = vViewRay * depthToDistance(depth, uNearDistance, uFarDistance);
        vec3 normalVS = texture2D(uGBufferGeometry, vTexCoord).xyz * 2.0 - 1.0;

        /// Вычисляем освещение по фонгу
        vec4 material = texture2D(uGBufferMaterial, vTexCoord);

        vec3 s = normalize(uLight.positionVS - positionVS);
        vec3 v = normalize(-positionVS);
        vec3 diffuseColor =     texture2D(uGBufferDiffuse, vTexCoord).rgb * 
                                uLight.diffuse *
                                max(dot(s, normalVS), 0.0);

        vec3 specularColor =    uLight.specular *
                                max(pow(dot(v, reflect(-s, normalVS)), material.r*100.0), 0.0);

        /// Вычисляем коэффециент угасания света с дистанцией
        float d = distance(uLight.positionVS, positionVS);
        float attenuation = 1.0f/(1.0f + uLight.attenuation * d*d);

        fLightBuffer = vec4((diffuseColor + specularColor)*attenuation, 1.0);
}