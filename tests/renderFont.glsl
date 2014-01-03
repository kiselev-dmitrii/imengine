imEngine::String rednerFontSource = R"(

///### VERTEX SHADER ###///
in vec3 im_vPosition;

void main() {
        gl_Position.xyz = im_vPosition;
        gl_Position.w = 1.0;
}


///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 im_outColor;

void main() {
        im_outColor = vec4(1,0,0,1);
}

)";
