imEngine::String showGeometrySource = R"(

///### VERTEX SHADER ###///
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexcoords;

uniform mat4 uModelViewProjectionMatrix;

void main() {
        gl_Position = uModelViewProjectionMatrix * vec4(vPosition, 1.0);
}


///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 fResult;

void main() {
        fResult = vec4(1,0,0,1);
}

)";
