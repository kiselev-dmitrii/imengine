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

float chebyshevUpperBound(in float dist, in vec4 lightTextureCoords) {
        // We retrive the two moments previously stored (depth and depth*depth)
        vec2 moments = texture2D(uLight.shadowMap, lightTextureCoords.xy).xy;
                
        // Surface is fully lit. as the current fragment is before the light occluder
        if (dist<= moments.x) return 1.0 ;
        
        float variance = moments.y - (moments.x*moments.x);
        variance = max(variance,0.00002);
        
        float d = dist - moments.x;
        float p_max = variance / (variance + d*d);
        
        return p_max;
}

void main() {
        /// Реконструируем позицию каждого пикселя
        vec3 positionVS = textureToViewSpace(vTexCoord, uGBufferDepth, uNearDistance, uFarDistance, uInvProjectionMatrix);
        vec3 normalVS = decodeNormal(vTexCoord, uGBufferGeometry);

        vec3 s = normalize(uLight.positionVS - positionVS);             //вектор указывающий на источник 
        float angle = degrees(acos(dot(s, -uLight.directionVS)));       //если этот угол равен 0, то точка в центре конуса

        float innerAngle = uLight.cutoffAngle - uLight.falloffAngle;
        float outerAngle = uLight.cutoffAngle;

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


                /// Считаем тень
                /// Переводим positionVS в Texture Space источника света
                vec4 positionLTS = uLight.shadowMatrix * vec4(positionVS, 1.0);
                positionLTS /= positionLTS.w;
                float shadow = 1.0 - chebyshevUpperBound(positionLTS.z, positionLTS);

                fLightBuffer = vec4((diffuseColor + specularColor)*falloff*shadow, 1.0);
        } else {
                fLightBuffer = vec4(0.0);
        }

}