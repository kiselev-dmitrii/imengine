///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;        // in [-1;1]x[-1;1]x[0;0]

out vec2 vTexCoord;

void main() {
        gl_Position = vec4(aPosition, 0.0, 1.0);
        vTexCoord = 0.5*aPosition + vec2(0.5);
}


///### FRAGMENT SHADER ###///

in vec2 vTexCoord;

uniform sampler2D       uLightBuffer;
uniform sampler2D       uGBuferGeometry;
uniform sampler2D       uGBufferDepth;

uniform float uTanHalfFovy;
uniform float uAspectRatio;
uniform mat4  uProjectionMatrix;

layout (location = 0) out vec4 fResult;

vec3 toViewSpace(vec2 texCoord, float depth, float tanHalfFovy, float aspectRatio) {
        vec3 viewSpaceCameraRay = vec3(
                (2.0*texCoord.x - 1.0) * tanHalfFovy * aspectRatio,
                (2.0*texCoord.y - 1.0) * tanHalfFovy, 
                1.0
        );
        return normalize(viewSpaceCameraRay) * depth;
}

void main() {
        float depth = texture(uGBufferDepth, vTexCoord).r;
        vec3 vViewSpacePosition = toViewSpace(vTexCoord, depth, uTanHalfFovy, uAspectRatio);
        vec3 vViewSpaceNormal = texture(uGBufferGeometry, vTexCoord).rgb*2.0 - 1.0;

        vec3 v = normalize(-vViewSpacePosition); 
        vec3 r = reflect(-v, vViewSpaceNormal);
        float step = 0.01;

        vec4 clipSpace = uProjectionMatrix * vec4(vViewSpacePosition, 1.0);
        clipSpace /= clipSpace.w;
        vec2 screenSpace = clipSpace.xy*0.5 + vec2(0.5);

	//fResult = texture2D(uGBuferGeometry, vTexCoord);
        fResult = vec4(screenSpace, 0.0, 1.0);
}