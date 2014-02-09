///### VERTEX SHADER ###///
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;

out vec3 vVolumeTexcoords;

uniform mat4 uModelViewProjectionMatrix;

void main() {
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
    //любая координата у нас либо -0.5 либо 0.5 - прибавляя 0.5 получаем текстурные координаты 0.0-1.0
    vVolumeTexcoords = aPosition + vec3(0.5); 
}


///### FRAGMENT SHADER ###///
in vec3 vVolumeTexcoords;

layout (location = 0) out vec4 fResult;

uniform sampler3D uVolumeTexture;

void main() {
	fResult = texture3D(uVolumeTexture, vVolumeTexcoords);
}
