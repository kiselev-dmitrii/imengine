///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;        // in [-1;1]x[-1;1]x[0;0]

out vec2 vTexCoord;

void main() {
        vTexCoord = 0.5*aPosition + 0.5;
        gl_Position = vec4(aPosition, 0.0, 1.0);
}


///### FRAGMENT SHADER ###///

in vec2 vTexCoord;

layout (location = 0) out vec4 fLightBuffer;

uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferMaterial;
uniform sampler2D uGBufferGeometry;
uniform sampler2D uGBufferDepth;

struct Light {
        vec3    diffuse;
        vec3    specular;
        float   attenuation;

        vec3    positionVS;
};
uniform Light uLight;
uniform mat4  uInvProjectionMatrix;
uniform float uNearDistance;
uniform float uFarDistance;

vec3 textureToViewSpace(in vec2 texCoord, in sampler2D depthBuffer, in float near, in float far, in mat4 invProjection) {
        float depth = texture2D(depthBuffer, texCoord).r;
        vec3 ndc = vec3(texCoord, depth) * 2.0 - 1.0;
        vec4 clip;
        clip.w = (2*near*far) / (near + far + ndc.z * (near - far));
        clip.xyz = ndc.xyz * clip.w;

        return (invProjection * clip).xyz;
}

void main() {
        /// Реконструируем позицию каждого пикселя
        vec3 positionVS = textureToViewSpace(vTexCoord, uGBufferDepth, uNearDistance, uFarDistance, uInvProjectionMatrix);
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