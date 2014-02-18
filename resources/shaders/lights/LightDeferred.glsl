///### VERTEX SHADER ###///
layout (location = 0) in vec2 aPosition;	// in [-1;1]x[-1;1]x[0;0]

uniform float uFarDistance;
uniform float uTanHalfFovy;
uniform float uAspectRatio;

out vec2 vTexCoord;
out vec3 vViewSpaceCameraRay;           //луч направленный от камеры т.е от (0,0) до текущей точки на дальней плоскости

void main() {
        gl_Position = vec4(aPosition, 0.0, 1.0);

	vTexCoord = 0.5*aPosition + vec2(0.5);
        vViewSpaceCameraRay = vec3(
                aPosition.x * uTanHalfFovy * uAspectRatio,
                aPosition.y * uTanHalfFovy, 
                1.0
        );
}


///### FRAGMENT SHADER ###///

in vec2 vTexCoord;
in vec3 vViewSpaceCameraRay;

uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferMaterial;
uniform sampler2D uGBufferGeometry;
uniform sampler2D uGBufferDepth;

uniform vec3 uDiffuseColor;
uniform vec3 uViewSpaceLightPosition;

layout (location = 0) out vec4 fLightBuffer;

void main() {
        float depth = texture(uGBufferDepth, vTexCoord).r;
        vec3 vViewSpacePosition = normalize(vViewSpaceCameraRay) * depth;
        vec3 vViewSpaceNormal = texture(uGBufferGeometry, vTexCoord).rgb*2.0 - 1.0;

        vec3 s = normalize(uViewSpaceLightPosition - vViewSpacePosition);
        vec3 v = normalize(-vViewSpacePosition);
        float sDOTn = max(dot(s, vViewSpaceNormal), 0.0);
        vec3 diffuse = uDiffuseColor * texture(uGBufferDiffuse, vTexCoord).rgb * sDOTn;
        vec3 specular = vec3(0.0);
        if (sDOTn > 0) {
                vec3 r = reflect(-s, vViewSpaceNormal);
                specular = uDiffuseColor * vec3(1.0) * pow(max(dot(r,v), 0.0), 80.0f);
        }

	fLightBuffer = vec4(vec3(diffuse + specular), 1.0);
}