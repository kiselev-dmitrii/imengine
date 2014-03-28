///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;        // in [-1;1]x[-1;1]x[0;0]

out vec2 vTexCoord;

void main() {
        vTexCoord = 0.5*aPosition + 0.5;
        gl_Position = vec4(aPosition, 0.0, 1.0);
}


///### FRAGMENT SHADER ###///
#include "../common/functions.glsl"

in vec2 vTexCoord;
layout (location = 0) out vec4 fLightBuffer;

/// Входные данные для освещения
uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferMaterial;
uniform sampler2D uGBufferGeometry;
uniform sampler2D uGBufferDepth;

/// Для преобразования из Texture Space во View Space
uniform mat4  uInvProjectionMatrix;
uniform float uNearDistance;
uniform float uFarDistance;

struct Light {
        vec3    diffuse;
        vec3    specular;

        float   cutoffAngle;    //в градусах
        float   falloffAngle;   //в градусах

        vec3    directionVS;
        vec3    positionVS;

        sampler2D       shadowMap;
        mat4            shadowMatrix;
};
uniform Light uLight;

void main() {
        /// Реконструируем позицию каждого пикселя
        vec3 positionVS = textureToViewSpace(vTexCoord, uGBufferDepth, uNearDistance, uFarDistance, uInvProjectionMatrix);
        vec3 normalVS = decodeNormal(vTexCoord, uGBufferGeometry);

        vec3 s = normalize(uLight.positionVS - positionVS);             //вектор указывающий на источник 
        float angle = degrees(acos(dot(s, -uLight.directionVS)));       //если этот угол равен 0, то точка в центре конуса

        float innerAngle = uLight.cutoffAngle;
        float outerAngle = uLight.cutoffAngle + uLight.falloffAngle;

        if (angle < outerAngle) {
                float falloff = 1.0 - smoothstep(innerAngle, outerAngle, angle);

                /// Вычисляем освещение по фонгу для точек внутри конуса
                vec4 material = texture2D(uGBufferMaterial, vTexCoord);
                vec3 v = normalize(-positionVS);
                vec3 diffuseColor =     texture2D(uGBufferDiffuse, vTexCoord).rgb * 
                                        uLight.diffuse *
                                        max(dot(s, normalVS), 0.0);

                vec3 specularColor =    uLight.specular *
                                        max(pow(dot(v, reflect(-s, normalVS)), material.r*100.0), 0.0);

                fLightBuffer = vec4((diffuseColor + specularColor)*falloff, 1.0);
        } else {
                fLightBuffer = vec4(0.0);
        }

        /// Переводим positionVS в NDC пространство источника света
        vec4 positionLS = uLight.shadowMatrix * vec4(positionVS, 1.0);
        positionLS /= positionLS.w;
        vec4 positionLTS = positionLS*0.5 + 0.5;
        if (positionLS.z > texture2D(uLight.shadowMap, positionLTS.xy).r) fLightBuffer = vec4(0.0);
        else fLightBuffer = vec4(1.0);
}